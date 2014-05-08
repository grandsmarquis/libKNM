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

#include "KSem.hh"
//#include <iostream>

using namespace std;
using namespace KNM;

/**
 * @param _value Semaphore value (or KSEM_DFLT_VALUE is not specified)
 */
KSem::KSem(int _value)
{
	open(_value);
}

/**
 * @param _name Semaphore name, should start with a '/' (automatically add if not found)
 * @param _value Semaphore value (or KSEM_DFLT_VALUE is not specified)
 */
KSem::KSem(const string &_name, int _value)
{
	open(_name, _value);
}

/**
 * @param _other KSem instance to use as a model
 */
KSem::KSem(const KSem &_other)
{
	if (_other.name.length() > 0)	// Named semaphore
		open(_other.name, _other.value);
	else							// Unnamed semaphore
	{
		this->value = _other.value;
		this->sem   = _other.sem;
	}
}

/**
 * @param _other KSem instance to use as a model
 */
KSem 	&KSem::operator=(const KSem &_other)
{
	if (&_other == this)
		return *this;

	if (_other.name.length() > 0)	// Named semaphore
		open(_other.name, _other.value);
	else							// Unnamed semaphore
	{
		this->value = _other.value;
		this->sem   = _other.sem;
	}

	return *this;
}

KSem::~KSem()
{

}

/**
 * @param _value Semaphore value (or KSEM_DFLT_VALUE is not specified)
 */
void 	KSem::open(int _value)
{
	this->value = _value;
	this->sem   = new sem_t;

	if (sem_init(this->sem, 0, _value) != 0)
		throw new KError(errno, "KSem::open()");
}

/**
 * @param _name Semaphore name, should start with a '/' (automatically add if not found)
 * @param _value Semaphore value (or KSEM_DFLT_VALUE is not specified)
 */
void 	KSem::open(const string &_name, int _value)
{
	this->name  = _name;
	this->value = _value;

	if (name[0] != '/') {name.insert(0, "/");}
	if (name.length() > NAME_MAX-5) {name.resize(NAME_MAX-5);}
	this->sem = sem_open(name.c_str(), KSEM_DFLT_OFLAGS, KSEM_DFLT_MODE, _value);
	if (this->sem == SEM_FAILED)
		throw new KError(errno, "KSem::open()");
}

int 	KSem::lock()
{
	return sem_wait(this->sem);
}

/**
 * @param _sec Seconds
 * @param _nsec Nano seconds
 */
int 	KSem::lock(int _sec, long int _nsec)
{
	this->timeout.tv_sec  = _sec;
	this->timeout.tv_nsec = _nsec;
	return sem_timedwait(this->sem, &this->timeout);
}

int 	KSem::trylock()
{
	return sem_trywait(this->sem);
}

int 	KSem::unlock()
{
	return sem_post(this->sem);
}

int 	KSem::close()
{
	lock();
	if (this->name.length() > 0)
	{
		if (sem_close(this->sem) == 0)
			return sem_unlink(this->name.c_str());
		return -1;
	}
	return sem_destroy(this->sem);
}

/**
 * @param _value Semaphore value (or KSEM_DFLT_VALUE is not specified)
 */
void 	KSem::reset(int _value)
{
	close();
	open(_value);
}

/**
 * @param _name Semaphore name, should start with a '/' (automatically add if not found)
 * @param _value Semaphore value (or KSEM_DFLT_VALUE is not specified)
 */
void 	KSem::reset(const string &_name, int _value)
{
	close();
	open(_name, _value);
}