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

#ifndef KTHREAD_POOL_HPP
#define KTHREAD_POOL_HPP

#include <list>
#include <KThread.hpp>

namespace KNM
{
	/**
	 * A thread pool using KThread.
	 * Create and manage thread by itself. The pool can be viewed
	 * as a single thread from the user side.
	 */
	template <class T = KThread_void>
	class 	KThread::pool
	{
	public:
		pool()
		{

		}

		pool(int _size, int _limit)
		{

		}

		void 	setSize(int _size)
		{

		}

		void 	setLimit(int _limit)
		{

		}

		int 	getSize()
		{

		}

		int 	getLimit()
		{

		}

		/**
		 * (re)Start the first available thread.
		 * It no thread is available, action is  placed in a waiting queue.
		 */
		int 	start(void *_startargs = NULL)
		{
			T 	*thread = new T;
		}

		/**
		 * Stop all current threads.
		 */
		void 	stop()
		{

		}

		
	private:
		int 		size;		/**< Maximum number of threads. */
		int 		limit;		/**< Orders limit. */
		list<T>		slots;		/**< Threads slots. */
	};
}

#endif