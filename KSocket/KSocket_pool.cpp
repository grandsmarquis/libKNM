/*
* libKNM
* Copyright (C) 2014 Jean-Philippe Clipffel
* Email: jp.clipffel@gmail.com
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
* USA
*/

#include <KSocket_pool.hh>

using namespace std;
using namespace KNM;

KSocket::pool::pool(): maxfd(KSOCKET_POOL_MAXFD_DFLT)
{
	FD_ZERO(&set_read);
	FD_ZERO(&set_write);
	FD_ZERO(&set_error);
	timeout.tv_sec  = KSOCKET_POOL_DFLT_SEC;
	timeout.tv_usec = KSOCKET_POOL_DFLT_USEC;
}

KSocket::pool::pool(int _sec, long _usec)
{
	FD_ZERO(&set_read);
	FD_ZERO(&set_write);
	FD_ZERO(&set_error);
	timeout.tv_sec  = _sec;
	timeout.tv_usec = _usec;
}

/**
 * @param _ref Reference to copy from
 */
KSocket::pool::pool(const KSocket::pool &_ref)
{
	if (&_ref != this)
	{
		set_read        = _ref.set_read;
		set_write       = _ref.set_write;
		set_error       = _ref.set_error;
		timeout.tv_sec  = _ref.timeout.tv_sec;
		timeout.tv_usec = _ref.timeout.tv_usec;
	}
}

/**
 * @param _ref Reference to copy from
 */
KSocket::pool &KSocket::pool::operator=(const KSocket::pool &_ref)
{
	if (&_ref != this)
	{
		set_read        = _ref.set_read;
		set_write       = _ref.set_write;
		set_error       = _ref.set_error;
		timeout.tv_sec  = _ref.timeout.tv_sec;
		timeout.tv_usec = _ref.timeout.tv_usec;
	}
	return *(this);	
}

KSocket::pool::~pool()
{

}

void 	KSocket::pool::setTimeout(int _sec, long _usec)
{
	timeout.tv_sec  = _sec;
	timeout.tv_usec = _usec;
}

/**
 * @param _socket Socket to add to the set
 * @param _mode   The set to use
 */
void 	KSocket::pool::add(KSocket *_socket, enum pool::mode _mode)
{
	if (!_socket)
		return;

	switch(_mode)
	{
		case READ:
			FD_SET(_socket->fd, &set_read);
			//update_maxfd(_socket->fd);
			break;
		case WRITE:
			FD_SET(_socket->fd, &set_write);
			//update_maxfd(_socket->fd);
			break;
		case ERROR:
			FD_SET(_socket->fd, &set_error);
			//update_maxfd(_socket->fd);
			break;
		case ALL:
			FD_SET(_socket->fd, &set_read);
			FD_SET(_socket->fd, &set_write);
			FD_SET(_socket->fd, &set_error);
			//update_maxfd(_socket->fd);
		default:
			break;
	}
}

/**
 * @param _socket Socket to remove from the set
 * @param _mode   The set to use
 */
void 		KSocket::pool::remove(KSocket *_socket, enum pool::mode _mode)
{
	if (!_socket)
		return;

	switch(_mode)
	{
		case READ:
			FD_CLR(_socket->fd, &set_read);
			break;
		case WRITE:
			FD_CLR(_socket->fd, &set_write);
			break;
		case ERROR:
			FD_CLR(_socket->fd, &set_error);
			break;
		case ALL:
			FD_CLR(_socket->fd, &set_read);
			FD_CLR(_socket->fd, &set_write);
			FD_CLR(_socket->fd, &set_error);
		default:
			break;
	}
}

void		KSocket::pool::select()
{
	int 	__attribute__((unused)) ret;
	struct timeval 					tv;

	set_read_used = set_read;
	set_write_used = set_write;
	set_error_used = set_error;

	if (timeout.tv_sec > 0 && timeout.tv_usec >= 0)
	{
		tv.tv_sec = timeout.tv_sec;
		tv.tv_usec = timeout.tv_usec;
		ret = ::select(FD_SETSIZE, &set_read_used, &set_write_used, &set_error_used, &tv);
	}
	else
		ret = ::select(FD_SETSIZE, &set_read_used, &set_write_used, &set_error_used, NULL);

	#if !defined KSOCKET_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
	switch(ret)
	{
		case -1:
			throw new KError(errno, "KSocket::pool::select");
			break;
		case 0:
			throw new KError("KSocket::pool::select", "select() timed out");
			break;
		default:
			break;
	}
	#else
	return;
	#endif
}

/**
 * @param _socket Socket to test
 * @param _mode   Set to search in
 */
bool 		KSocket::pool::isset(KSocket *_socket, enum pool::mode _mode)
{
	if (!_socket)
		return false;

	switch(_mode)
	{
		case READ:
			if (FD_ISSET(_socket->fd, &set_read_used))
				return true;
			else
				return false;
			break;

		case WRITE:
			if (FD_ISSET(_socket->fd, &set_write_used))
				return true;
			else
				return false;
			break;

		case ERROR:
			if (FD_ISSET(_socket->fd, &set_error_used))
				return true;
			else
				return false;
			break;

		case ALL:
			if (FD_ISSET(_socket->fd, &set_read_used)
				&& FD_ISSET(_socket->fd, &set_write_used)
				&& FD_ISSET(_socket->fd, &set_error_used))
				return true;
			else
				return false;
			break;

		default:
			return false;
			break;
	}
	return false;
}

/**
 * @param _fd New file descriptor
 */
void 	KSocket::pool::update_maxfd(int _fd)
{
	if (_fd > maxfd)
		maxfd = _fd;
}