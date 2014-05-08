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

#include <stdio.h>
#include "KDylib.hh"

using namespace KNM;

KDylib::KDylib(): libfd(NULL){}

KDylib::KDylib(const string &_path): path(_path), libfd(NULL){}

KDylib::KDylib(const KDylib &_ref)
{
	if (&_ref != this)
	{
		path  = _ref.path;
		libfd = _ref.libfd;
	}
}

KDylib 	&KDylib::operator=(const KDylib &_ref)
{
	if (&_ref != this)
	{
		close();
		path = _ref.path;
		open();
	}
	return *(this);
}

KDylib::~KDylib()
{
	close();
}

void 			KDylib::setPath(const string &_path)
{
	path = _path;
}

const string 	&KDylib::getPath()
{
	return path;
}

void 			KDylib::open(int flag)
{
	if (!libfd)
		libfd = dlopen(path.c_str(), flag);

	if (!libfd)
	{
		#if !defined KDYLIB_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
			throw new KError("KDylib::open", dlerror());
		#else
			lastError = dlerror();
		#endif
	}
}

void 			KDylib::close()
{
	if (libfd)
	{
		if (dlclose(libfd) != 0)
		{
			#if !defined KDYLIB_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
				throw new KError("KDylib::close", dlerror());
			#else
				lastError = dlerror();
			#endif
		}
	}
}

void 			*KDylib::symbol(const string &_name)
{
	void 		*sym;

	if (!libfd)
		open();
	
	sym = dlsym(libfd, _name.c_str());
	if (!sym)
	{
		#if !defined KDYLIB_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
			throw new KError("KDylib::symbol", dlerror());
		#else
			lastError = dlerror();
		#endif
	}
	return sym;
}

const string 	&KDylib::error()
{
	return lastError;
}

bool 			KDylib::isOpen()
{
	if (libfd)
		return true;
	return false;
}