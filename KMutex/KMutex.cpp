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

#include "KMutex.hh"

using namespace std;
using namespace KNM;

KMutex::KMutex()
{
	pthread_mutex_init(&this->mutex, NULL);
}

/**
 * @param _attr Mutex structure for initialization
 */
KMutex::KMutex(const pthread_mutexattr_t *_attr = NULL)
{
	pthread_mutex_init(&this->mutex, _attr);
}

KMutex::~KMutex()
{
	unlock();
	pthread_mutex_destroy(&this->mutex);
}

int 	KMutex::lock()
{
	return pthread_mutex_lock(&this->mutex);
}

/**
 * @param _sec Seconds
 * @param _nsec Nano seconds
 */
int 	KMutex::lock(int _sec, long int _nsec)
{
	this->timeout.tv_sec = _sec;
	this->timeout.tv_nsec = _nsec;
	return pthread_mutex_timedlock(&this->mutex, &this->timeout);
}

int 	KMutex::trylock()
{
	return pthread_mutex_trylock(&this->mutex);
}

int 	KMutex::unlock()
{
	return pthread_mutex_unlock(&this->mutex);
}