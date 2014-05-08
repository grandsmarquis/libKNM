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

#ifndef KTHREAD_HPP
#define KTHREAD_HPP

#include <pthread.h>
#include <unistd.h>
//#include <KThread_pool.hpp>

using namespace std;

namespace KNM
{
	/**
	 * Default KThread templated class.
	 */
	class KThread_void
	{
	public:
		/**
		 * Default tmain function for not-templated KThread instance.
		 * @param args Thread function arguments
		 */
		void 	*tmain(void __attribute__((unused))*args) {return NULL;}
		void 	*tend(){return NULL;}
	};

	/**
	 * Thread wrapper.
	 * Pthread C library wrapper, with template support.
	 */
	template <class T = KThread_void>
	class 	KThread
	{
	public:

		/**
		 * Default constructor.
		 */
		KThread()
		{
			this->obj 		  = NULL;
			this->generic_fct = NULL;
		}

		/**
		 * Constructor with templated object.
		 * @param _obj Templated object pointer
		 */
		KThread(T *_obj)
		{
			this->obj = _obj;
			this->generic_fct = NULL;
		}

		// Gneric function pointer thread
		/**
		 * Constructor with generic function pointer.
		 * @param _fct Generic function pointer
		 */
		KThread(void *(*_fct)(void*))
		{
			this->generic_fct = _fct;
			this->obj = NULL;
		}

		/**
		 * Start the thread.
		 * @param _startargs Thread functions start argument(s)
		 */
		int 	start(void *_startargs = NULL)
		{
			this->startargs = _startargs;
			return pthread_create(&ID, NULL, &KThread::_start, this);
		}

		/**
		 * Stop the thread.
		 */
		int 	stop()
		{
			if (!this->obj && !this->generic_fct)
				this->tend();
			return pthread_cancel(ID);
		}

		/**
		 * Join (wait) for the thread to end.
		 * @param retval Return value
		 */
		int 	join(void __attribute__((unused))**retval = NULL)
		{
			return pthread_join(ID, NULL);
		}

		/**
		 * Link the KThread to a generic function pointer.
		 * @param _fct Generic function pointer
		 */
		void 	link(void *(*_fct)(void*))
		{
			if (_fct)
				this->generic_fct = _fct;
		}

		/**
		 * Return thread ID.
		 */
		pthread_t 	&getID()
		{
			return this->ID;
		}

		/**
		 * Thread start fnuction wrapper.
		 * @param _context Emulated 'this' pointer
		 */
		static void 	*_start(void *_context)
		{
			KThread 	*self = ((KThread *)_context);

			// If thread handle a generic function pointer
			if (self->generic_fct != NULL)
			{
				//cout << "generic" << endl;
				return self->generic_fct(self->startargs);
			}

			// If thread handle an object
			else if (self->obj != NULL)
			{
				//cout << "template" << endl;
				return self->obj->tmain(self->startargs);
			}

			// If thread is used in inheritance sheme
			else
			{
				//cout << "inheritance" << endl;
				return self->tmain(self->startargs);
			}
		}

		/**
		 * Thread function to call by default (should be overloaed by templated class).
		 * @param _args Function arguments
		 */
		virtual void 	*tmain(void __attribute__((unused))*_args)
		{
			return(NULL);
		}

		/**
		 ** Stop thread (should be overloaed by templated class).
		 */
		virtual void 	*tend()
		{
			return NULL;
		}

	private:
		pthread_t 		ID;
		T 				*obj;
		void 			*(*generic_fct)(void*);
		void 			*startargs;

	/*public:
		Kthread_pool 	pool;*/
	};
}

#endif