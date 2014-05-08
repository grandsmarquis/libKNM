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

#ifndef KSOCKET_POOL_HH
#define KSOCKET_POOL_HH

#include <sys/select.h>
#include <list>
#include <KSocket.hh>

#define KSOCKET_POOL_MAXFD_DFLT 	-1
#define KSOCKET_POOL_DFLT_SEC 		-1
#define KSOCKET_POOL_DFLT_USEC		-1

using namespace std;

namespace KNM
{
	/**
	 * A pool of KSockets, mainly used to perform select() operations.
	 * Compilations flags:
	 * - LIBKNM_NO_EXCEPTIONS  : Disable exceptions
	 * - KSOCKET_NO_EXCEPTIONS : Disable exceptions
	 */
	class 	KSocket::pool
	{
	public:
		pool();													/**< Default constructor. */
		pool(int _sec, long _usec = KSOCKET_POOL_DFLT_USEC);	/**< Constructu and set the timeout. */
		pool(const KSocket::pool&);								/**< Construct by copy. */
		pool &operator=(const KSocket::pool &);					/**< Assignement operator. */
		~pool();												/**< Destructor. */

	public:
		/**
		 * Enumeration for FD_SET add operation.
		 */
		enum mode {READ, WRITE, ERROR, ALL};

	public:
		void 	setTimeout(int _sec, long _usec = 0);			/**< Set the timeout manually. */
		void 	add(KSocket *, enum pool::mode);				/**< Add a KSocket to the pool. */
		void 	remove(KSocket *, enum pool::mode);				/**< Remove a KSocket from the pool. */
		void	select();										/**< Perform a select operation. */
		bool 	isset(KSocket *, enum pool::mode);				/**< Check if a socket is ready for READ, WRITE or ERROR. */

	private:
		void 			update_maxfd(int);						/**< Update th maxfd value. */

		int 			maxfd;									/**< Maximum file descriptor number. */
		struct timeval 	timeout;								/**< select() timeout structure. */

		fd_set 			set_read;								/**< Sockets monitored for read operations.  */
		fd_set 			set_write;								/**< Sockets monitored for write operations. */
		fd_set 			set_error;								/**< Sockets monitored for errors detection. */
		fd_set 			set_read_used;
		fd_set 			set_write_used;
		fd_set 			set_error_used;

	};
}

#endif