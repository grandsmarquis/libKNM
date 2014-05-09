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

#include "KShm.hh"

using namespace KNM;

KShm::KShm():fd(-1), oflags(O_CREAT|O_RDWR), omode(S_IRWXU|S_IRWXG|S_IRWXO){}

/**
 * @param _ref Object to copy from
 */
KShm::KShm(const KShm &_ref):fd(-1)
{
	if (&_ref != this)
	{
		name   = _ref.name;
		oflags = _ref.oflags;
		omode  = _ref.omode;
	}
}

/**
 * @param _name SHM name to create / open
 */
KShm::KShm(const string &_name):fd(-1), name(_name), oflags(O_CREAT|O_RDWR), omode(S_IRWXU|S_IRWXG|S_IRWXO){}

/**
 * @param _ref Object to copy from
 */
KShm &KShm::operator=(const KShm &_ref)
{
	if (&_ref != this)
	{
		close();
		fd     = -1;
		name   = _ref.name;
		oflags = _ref.oflags;
		omode  = _ref.omode;
	}
	return *this;
}

KShm::~KShm()
{
	close();
}

/**
 * @param _name SHM name.
 */
void 	KShm::setName(const string &_name)
{
	name = _name;
}

/**
 * @param _flags Open flags (see shm_open(2))
 */
void 	KShm::setOFlags(int _flags)
{
	oflags = _flags;
}

/**
 * @param _mode Open mode (see shm_open(2))
 */
void 	KShm::setOMode(int _mode)
{
	omode = _mode;
}

void 	KShm::open()
{
	close();
	if (name[0] != '/') {name.insert(0, "/");}
	if (name.length() > NAME_MAX) {name.resize(NAME_MAX);}
	fd = shm_open(name.c_str(), oflags, omode);
	if (fd < 0)
		throw new KError(errno, "KShm::open()");
}

/**
 * @param _name SHM name to open / create
 */
void 	KShm::open(const string &_name)
{
	name = _name;
	open();
}

bool 	KShm::isOpen()
{
	if (fcntl(fd, F_GETFD) < 0)
		return false;
	return true;
}

void 	KShm::close()
{
	if (isOpen())
		::close(fd);
}

void 	KShm::unlink()
{
	close();
	shm_unlink(name.c_str());
}

/**
 * @param _buffer Read from SHM to buffer.
 * @param _len Read len bytes
 */
int 		KShm::read(void *_buffer, size_t _len)
{
	int 	ret;

	ret = ::read(fd, _buffer, _len);
	if (ret < 0 || (ret <= 0 && _len > 0) )
		#if !defined KSHM_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
			throw(new KError(errno, "KShm::read(void*)"));
		#else
			return ret;
		#endif
	return ret;
}


/**
 * @param _buffer Read in this buffer
 */
int 		KShm::read(int &_buffer)
{
	int 	ret;
	int 	buffer;

	ret = ::read(fd, &buffer, sizeof(int));
	if (ret != sizeof(int))
		#if !defined KSHM_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
			throw(new KError(errno, "KShm::read(int)"));
		#else
			return ret;
		#endif
	_buffer = buffer;
	return ret;
}

/**
 * @param _buffer Write this buffer content
 * @param _len Write len bytes
 */
int 		KShm::write(const void *_buffer, size_t _len)
{
	int 	ret;

	ret = ::write(fd, _buffer, _len);
	if (ret < 0 || (ret <= 0 && _len > 0) )
		#if !defined KSHM_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
			throw(new KError(errno, "KShm::write(void*)"));
		#else
			return ret;
		#endif
	return ret;
}

/**
 * @param _buffer Write this buffer
 */
int 	KShm::write(const int _buffer)
{
	int 	ret;

	ret = ::write(fd, &_buffer, sizeof(int));
	if (ret != sizeof(int))
		#if !defined KSHM_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
			throw(new KError(errno, "KShm::write(int)"));
		#else
			return ret;
		#endif
	return ret;
}

/**
 * @param _buffer Write this buffer
 */
int 		KShm::write(const string &_buffer)
{
	int 	ret;

	ret = ::write(fd, _buffer.c_str(), _buffer.size());
	if (ret < 0 || (ret <= 0 && _buffer.size() > 0) )
		#if !defined KSHM_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
			throw(new KError(errno, "KShm::write(string)"));
		#else
			return ret;
		#endif
	return ret;
}

/**
 * @param _offset lseek offset
 * @param _whence lseek whence directive
 */
off_t 		KShm::seek(off_t _offset, int _whence)
{
	off_t 	ret;

	ret = lseek(fd, _offset, _whence);
	if (ret < 0)
		#if !defined KSHM_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
			throw(new KError(errno, "KShm::seek(enum offset)"));
		#else
			return ret;
		#endif
	return ret;
}

/**
 * @param _offset BEGIN or END of SHM
 */
off_t 		KShm::seek(enum KShm::offset _offset)
{
	if (_offset == BEGIN)
		return seek(0, SEEK_SET);
	return seek(0, SEEK_END);
}