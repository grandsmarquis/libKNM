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

#ifndef KSEM_HH
#define KSEM_HH

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <limits.h>
#include <errno.h>
#include <string>
#include <KError.hh>

#define 	KSEM_DFLT_OFLAGS	O_CREAT|O_RDWR
#define 	KSEM_DFLT_MODE		S_IRWXU|S_IRWXG|S_IRWXO
#define 	KSEM_DFLT_VALUE		1

using namespace std;

namespace KNM
{
	/**
	 * A POSIX semaphore class.
	 * Create, control and destroy POSIX semaphore.
	 */
	class 		KSem
	{
	public:
		KSem(int _value = KSEM_DFLT_VALUE);								/**< Default constructor (unamed semaphore). */
		KSem(const string &, int _value = KSEM_DFLT_VALUE);				/**< Construct a named semaphore. */
		KSem(const KSem&);												/**< Copy constructor. */
		KSem &operator=(const KSem&);									/**< Assignment operator. */
		~KSem();														/**< Destructor. */

		void 	open(int _value = KSEM_DFLT_VALUE);						/**< Open (init) an unnamed semaphore. */
		void 	open(const string &, int _value = KSEM_DFLT_VALUE);		/**< Open (create and/or open) a named semaphore. */
		int 	lock();													/**< Lock (wait) the semaphore (blockant). */
		int 	lock(int, long int);									/**< Lock (wait) the semaphore (blockant until timeout). */
		int 	trylock();												/**< Lock (wait) the semaphore (non-blockant) .*/
		int 	unlock();												/**< Unlock (post) the semaphore. */
		void 	reset(int _value = KSEM_DFLT_VALUE);					/**< Reset (destroy an init again) an unnamed semaphore. */
		void 	reset(const string &, int _value = KSEM_DFLT_VALUE);	/**< Reset (close, unlink and open again) a named semaphore. */
		int 	close();												/**< Destroy (unnamed) or unlink (named) a semaphore. */

	private:
		string 				name;										/**< Semaphore name. */
		int 				value;										/**< Semaphore value. */
		sem_t 				*sem;										/**< Semaphore structure. */
		struct timespec		timeout;									/**< Time strcucture. */
	};
}

#endif