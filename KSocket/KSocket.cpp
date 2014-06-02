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

#include <KSocket.hh>

using namespace std;
using namespace KNM;

KSocket::KSocket()
{
	fd = KSOCKET_STATE_CLOSE;
	addr.setPort(KSOCKET_DFLT_PORT);
	setType(TCP4);
	backlog = KSOCKET_DFLT_BACKLOG;
}

/**
 * @param _type Socket domain (family)
 */
KSocket::KSocket(enum KSocket::type _type)
{
	fd = KSOCKET_STATE_CLOSE;
	addr.setPort(KSOCKET_DFLT_PORT);
	setType(_type);
	backlog = KSOCKET_DFLT_BACKLOG;
}

/**
 * @param _type    Socket domain (family)
 * @param _address Address
 * @param _port    Port
 */
KSocket::KSocket(enum KSocket::type _type, const string &_address, int _port)
{
	fd = KSOCKET_STATE_CLOSE;
	setType(_type);
	addr.setAddress(_address);
	addr.setPort(_port);
	backlog = KSOCKET_DFLT_BACKLOG;
}

/**
 * @param _ref Reference to copy from
 */
KSocket::KSocket(const KSocket &_ref)
{
	if (&_ref != this)
	{
		backlog = _ref.backlog;
		type    = _ref.type;
		addr    = _ref.addr;
	}
}

/**
 * @param _ref Reference to copy from
 */
KSocket &KSocket::operator=(const KSocket &_ref)
{
	if (&_ref != this)
	{
		#if !defined KSOCKET_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
		try{disconnect();}
		catch (KError *){}
		#else
		disconnect();
		#endif

		backlog = _ref.backlog;
		type    = _ref.type;
		addr    = _ref.addr;
	}
	return *(this);
}

/**
 * @param _ref Reference to copy from
 */
bool KSocket::operator==(const KSocket &_ref)
{
	if (&_ref == this){return true;}
	return false;
}

/**
 * @param _ref Reference to copy from
 */
bool KSocket::operator==(KSocket *_ref)
{
	if (_ref == this){return true;}
	return false;
}


KSocket::~KSocket()
{
	disconnect();
}

/**
 * @param _type Socket domain (family)
 */
void 				KSocket::setType(enum KSocket::type _type)
{
	type = _type;
	if (type == UDP4 || type == TCP4)
		addr.setDomain(AF_INET);
	else if (type == UDP6 || type == TCP6)
		addr.setDomain(AF_INET6);
}

/**
 * @param _address Address (IPv4 or IPv6)
 */
void 				KSocket::setAddress(const string &_address)
{
	addr.setAddress(_address);
}

/**
 * @param _name Name (Address or hostname)
 */
void 				KSocket::setName(const string &_name)
{
	addr.setName(_name);
}

/**
 * @param _port Port
 */
void 				KSocket::setPort(int _port)
{
	addr.setPort(_port);
}

/**
 * @param _mode Blocking flag
 */
void 				KSocket::setBlock(bool _mode)
{
	int 			bmode = 1;

	if (_mode)
		bmode = 0;
	if (ioctl(fd, FIONBIO, &bmode) < 0)
		#if !defined KSOCKET_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
			throw(new KError(errno, "KSocket::setBlock()"));
		#else
			return;
		#endif
}

/**
 * @param _backlog Listen() function backlog
 */
void 				KSocket::setBacklog(int _backlog)
{
	backlog = _backlog;
}

KSaddr 				*KSocket::getSaddr()
{
	return &addr;
}

void				KSocket::socket()
{
	int 	optval 	= 1;
	//int 	mode 	= 0;

	if (type == TCP4 || type == TCP6)
		fd = ::socket(addr.getDomain(), SOCK_STREAM, 0);
	else if (type == UDP4 || type == UDP6)
		fd = ::socket(addr.getDomain(), SOCK_DGRAM, 0);
	if (this->fd < 0)
		#if !defined KSOCKET_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
			throw(new KError(errno, "KSocket::socket::socket()"));
		#else
			return;
		#endif

	// setsockopt() allow to reuse socket immediately after its close
	if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
		#if !defined KSOCKET_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
			throw(new KError(errno, "KSocket::socket::setsockopt()"));
		#else
			return;
		#endif
}

void				KSocket::bind()
{
	if (this->fd == KSOCKET_STATE_CLOSE)
		socket();

	if (::bind(fd, addr.getSaddr(), addr.getSaddrSize()) != 0)
		#if !defined KSOCKET_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
			throw(new KError(errno, "KSocket::bind()"));
		#else
			return;
		#endif
}

/**
 * @param _port Port to bind to
 */
void 				KSocket::bind(int _port)
{
	addr.setPort(_port);
	bind();
}

void				KSocket::listen()
{
	if (::listen(fd, backlog) != 0)
		#if !defined KSOCKET_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
			throw(new KError(errno, "KSocket::listen()"));
		#else
			return;
		#endif
}

void 				KSocket::connect()
{
	if (fd == KSOCKET_STATE_CLOSE)
		socket();

	if (::connect(fd, addr.getSaddr(), addr.getSaddrSize()) != 0)
		#if !defined KSOCKET_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
			throw(new KError(errno, "KSocket::connect()"));
		#else
			return;
		#endif
}

/**
 * @param _address Address to connect to
 * @param _port Port to connect to
 */
void 				KSocket::connect(const string &_address, int _port)
{
	addr.setAddress(_address);
	addr.setPort(_port);
	connect();
}

void 				KSocket::disconnect()
{
	shutdown('a');
	// SHOULD read until an error is raised
	if (fd != KSOCKET_STATE_CLOSE)
	{
		::close(fd);
		fd = KSOCKET_STATE_CLOSE;
	}
}

void 				KSocket::close()
{
	disconnect();
}

/**
 * @param c shutdown mode
 */
void 				KSocket::shutdown(char c)
{
	if (fd != KSOCKET_STATE_CLOSE)
	{
		switch(c)
		{
			case 'w':
				::shutdown(fd, SHUT_WR);
				break;
			case 'W':
				::shutdown(fd, SHUT_WR);
				break;
			case 'r':
				::shutdown(fd, SHUT_RD);
				break;
			case 'R':
				::shutdown(fd, SHUT_RD);
				break;
			default:
				::shutdown(fd, SHUT_RDWR);
				break;
		}
	}
}

KSocket 			*KSocket::accept()
{
	KSocket 		*client = new KSocket();
	unsigned int 	clen 	= client->addr.getSaddrSize();

	client->fd = ::accept(fd, client->addr.getSaddr(), &clen);
	if (client->fd < 0)
		#if !defined KSOCKET_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
			throw(new KError(errno, "KSocket::accept()"));
		#else
			return NULL;
		#endif
	else
		return(client);
}

bool 				KSocket::isReadable()
{
	int 			pending;
	int 			ret;

	ret = ioctl(fd, FIONREAD, &pending);

	if (ret < 0)
		#if !defined KSOCKET_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
			throw(new KError(errno, "KSocket::isReadable()"));
		#else
			return false;
		#endif
	if(ret >= 0)
	{
		if (pending > 0)
			return true;
		return false;
	}
	return false;
}

/**
 * @param _buffer Write received data in this buffer
 * @param _size   Write _size bytes maximum
 */
int 				KSocket::receive(void *_buffer, int _size)
{
	int 			ret;

	ret = recv(fd, _buffer, _size, 0);
	if (ret < 0)
		#if !defined KSOCKET_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
			throw(new KError(errno, "KSocket::receive()"));
		#else
			return ret;
		#endif
	else if (ret == 0)
		disconnect();
	return ret;
}

/**
 * @param _buffer Write the received integer in this buffer
 */
int 				KSocket::receive(int *_buffer)
{
	int 			htbuffer;
	int 			ret;

	htbuffer = 0;
	ret = receive(&htbuffer, sizeof(int));
	*_buffer = ntohl(htbuffer);
	return ret;
}

/**
 * @param _buffer Write received data in this buffer
 * @param _size   Write _size bytes maximum
 * @param _addr   Receive from this addr
 */
int 				KSocket::receiveFrom(void *_buffer, int _size, KSaddr &_addr)
{
	socklen_t 		from;
	int 			ret;

	from = _addr.getSaddrSize();
	ret = recvfrom(fd, _buffer, _size, 0, _addr.getSaddr(), &from);
	if (ret < 0)
		#if !defined KSOCKET_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
			throw(new KError(errno, "KSocket::receiveFrom()"));
		#else
			return ret;
		#endif
	else if (ret == 0)
		disconnect();
	return ret;
}

/**
 * @param _buffer Write the received integer in this buffer
 * @param _addr   Receive from this addr
 */
int 				KSocket::receiveFrom(int *_buffer, KSaddr &_addr)
{
	int 			htbuffer;
	int 			ret;

	htbuffer = 0;
	ret = receiveFrom(&htbuffer, sizeof(int), _addr);
	*_buffer = ntohl(htbuffer);
	return ret;
}

/*int 				KSocket::receiveFile(const string &_file, size_t _len = 0)
{
	return 0;
}*/

/**
 * @param _buffer Content to send
 * @param _size   Bytes to send
 */
int 				KSocket::send(void *_buffer, int _size)
{
	int 			ret;

	ret = ::send(fd, _buffer, _size, 0);
		if (ret < 0)
			#if !defined KSOCKET_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
				throw(new KError(errno, "KSocket::send()"));
			#else
				return ret;
			#endif
	else if (ret == 0)
		disconnect();
	return ret;
}

/**
 * @param _buffer String to send
 */
int 				KSocket::send(const string &_buffer)
{
	const char 		*cstr;

	cstr = _buffer.c_str();
	return send((void*)cstr, _buffer.size());
}

/**
 * @param _buffer Integer to send
 */
int 				KSocket::send(int _buffer)
{
	int 			htbuffer;

	htbuffer = htonl(_buffer);
	return send(&htbuffer, sizeof(int));
}

/**
 * @param _buffer Content to send
 * @param _size   Bytes to send
 * @param _addr   Send to this addr
 */
int 				KSocket::sendTo(void *_buffer, int _size, KSaddr &_addr)
{
	int 			ret;

	ret = ::sendto(fd, _buffer, _size, 0, _addr.getSaddr(), _addr.getSaddrSize());
		if (ret < 0)
			#if !defined KSOCKET_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
				throw(new KError(errno, "KSocket::send()"));
			#else
				return ret;
			#endif
	else if (ret == 0)
		disconnect();
	return ret;
}

/**
 * @param _buffer String to send
 * @param _addr   Send to this addr
 */
int 				KSocket::sendTo(const string &_buffer, KSaddr &_addr)
{
	const char 		*cstr;

	cstr = _buffer.c_str();
	return sendTo((void*)cstr, _buffer.size(), _addr);	
}

/**
 * @param _buffer Integer to send
 * @param _addr   Send to this addr
 */
int 				KSocket::sendTo(int _buffer, KSaddr &_addr)
{
	int 			htbuffer;

	htbuffer = htonl(_buffer);
	return sendTo(&htbuffer, sizeof(int), _addr);
}

/*int 				KSocket::sendFile(const string &_file, size_t _len = 0)
{
	return 0;
}*/

void 				KSocket::endpoint()
{
	bind();
	listen();
}

/**
 * @param _type Socket domain (family)
 * @param _address Address
 * @param _port Port
 */
void 				KSocket::endpoint(enum KSocket::type _type,
									  const string &_address, int _port)
{
	setType(_type);
	setAddress(_address);
	setPort(_port);
	endpoint();
}
