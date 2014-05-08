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

#include "KFS.hh"

using namespace std;
using namespace KNM;

KFS::KFS()
{
	this->retstate = true;
	this->retval   = RETVAL_SUCCESS;
}

bool	KFS::success()
{
	//...
	return (this->retstate);
}

int 	KFS::getReturn()
{
	//...
	return(this->retval);	
}

const string 	&KFS::getError()
{
	//...
	return(this->error);
}

const string 	&KFS::pwd()
{
	char 		*wd_cstr   = NULL;
	long 		pathlen    = 0;

	pathlen = pathconf(".", _PC_PATH_MAX);
	wd_cstr = new char[pathlen + 1];
	if (getcwd(wd_cstr, pathlen) == NULL)
	{
		delete(wd_cstr);
		storestate(false, RETVAL_FAIL);
		return(this->wd);
	}
	this->wd       = wd_cstr;
	storestate(true, RETVAL_SUCCESS);
	delete(wd_cstr);
	return(this->wd);
}

/**
* @param _dir Directory to change for
*/
bool 			KFS::chdir(const string &_dir)
{
	this->retval = ::chdir(_dir.c_str());
	if (this->retval != 0)
	{
		this->retstate = false;
		this->error    = strerror(errno);
		return(false);
	}
	return(true);
}

/**
 * @param _src Source file to copy
 * @param _dst New file name
 */
bool 			KFS::copy(const string &_src, const string &_dst)
{
	int			src;
	int 		dst;
	int 		size;
	char 		buffer[COPY_L1_SIZE];
	ssize_t 	bytes;
	int 		count;

	src = open(_src.c_str(), O_RDONLY);
	if (src < 0) {return(this->storestate(false, src));}
	dst = open(_dst.c_str(), O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
	if (dst < 0) {return(this->storestate(false, dst));}

	/**
	 * Inform the kernel about the next reading operations.
	 * Return value is meaningless here, so not handled.
	 */
	posix_fadvise(src, 0, 0, POSIX_FADV_SEQUENTIAL|POSIX_FADV_WILLNEED);

	/**
	 * Get the file size.
	 */
	size = lseek(src, 0, SEEK_END);
	if (size < 0) {return(this->storestate(false, size));}
	size = lseek(src, 0, SEEK_SET);
	if (size != 0) {return(this->storestate(false, size));}

	/**
	 * Prepare the file size.
	 * Useful in case of low space storage, to avoid erros during writing.
	 * Once again, return is not important (adn function can fails sometimes, probably permissions error.)
	 */
	posix_fallocate(dst, 0, size);

	/**
	 * Effective copy operation.
	 * Note: copy is made through the CPU l1 cache
	 * (that's why buffer size is low).
	 */
	count = 0;
	while (1)
	{
		bytes = read(src, &buffer[0], COPY_L1_SIZE);
		if (bytes == 0) break;
		if (bytes < 0) {return(this->storestate(false, bytes));}
		bytes = write(dst, &buffer[0], bytes);
		count += bytes;
		if (bytes == 0) break;
		if (bytes < 0) {return(this->storestate(false, bytes));}
	}
	return(this->storestate(false, count));
}

/**
 * @param _src Source file to move
 * @param _dst new file name (and optionnaly path)
 */
bool			KFS::move(const string &_src, const string &_dst)
{
	this->retval = ::rename(_src.c_str(), _dst.c_str());
	if (this->retval != 0)
	{
		this->retstate = false;
		this->error    = strerror(errno);
		return(false);
	}
	return(true);
}

/**
 * @param _src Filename to remove
 */
bool			KFS::remove(const string &_src)
{
	this->retval = ::remove(_src.c_str());
	if (this->retval != 0)
	{
		this->retstate = false;
		this->error    = strerror(errno);
		return(false);
	}
	return(true);
}

/**
 * @param _src Directory name
 */
bool			KFS::mkdir(const string &_src)
{
	this->retval = ::mkdir(_src.c_str(), S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP);
	if (this->retval != 0)
	{
		this->retstate = false;
		this->error    = strerror(errno);
		return(false);
	}
	return(true);
}

/**
 * @param _src File name
 */
bool		KFS::mkfile(const string &_src)
{
	int 	fd;

	fd = open(_src.c_str(), O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
	if (fd < 0) {return(this->storestate(false, fd));}
	close(fd);
	return(true);
}

bool 			KFS::storestate(bool _state, int _retval)
{
	this->retstate = _state;
	this->retval   = _retval;
	if (_state == false)
		this->error = strerror(errno);
	return _state;
}