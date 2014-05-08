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

#ifndef KMUTEX_HH
#define KMUTEX_HH

#include <iostream>
#include <string>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

namespace KNM
{
	/**
	 * Mutex wrapper.
	 * A C++ mutex class wrapper with standard operations.
	 */
	class 	KMutex
	{
	public:
		KMutex();										/**< Create a self-initialized mutex; mode is PT. */
		KMutex(const pthread_mutexattr_t *_attr);		/**< Create a mutex initalized with _attr; mode is PT. */
		~KMutex();										/**< Destroy the mutex after unlocking it. */

		int 	lock();									/**< Lock the mutex. Function is blocking. */
		int 	lock(int, long int);					/**< Lock the mutex for some time. Function is blocking. */
		int 	trylock();								/**< Try to lock the mutex. Function is non-blocking. */
		int 	unlock();								/**< Unlock the mutex. */

	private:
		pthread_mutex_t 	mutex;						/**< pthread_mutex mutex. */
		struct timespec 	timeout;					/**< Time strcucture. */
	};
}

#endif