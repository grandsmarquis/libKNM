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

KShm::KShm():fd(-1){}

/**
 * @param _ref Object to copy from
 */
KShm::KShm(const KShm &_ref):fd(-1)
{
	if (&_ref != this)
	{
		name = _ref.name;
	}
}

/**
 * @param _name SHM name to create / open
 */
KShm::KShm(const string &_name):fd(-1), name(_name){}

/**
 * @param _ref Object to copy from
 */
KShm &KShm::operator=(const KShm &_ref)
{
	if (&_ref != this)
	{
		close();
		fd = -1;
		name = _ref.name;
	}
	return *this;
}

KShm::~KShm()
{
	close();
}

void 	KShm::open()
{
	close();
	if (name[0] != '/') {name.insert(0, "/");}
	if (name.length() > NAME_MAX) {name.resize(NAME_MAX);}
	fd = shm_open(name.c_str(), KSHM_DFLT_OFLAGS, KSHM_DFLT_MODE);
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