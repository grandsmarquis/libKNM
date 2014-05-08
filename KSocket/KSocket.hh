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

#ifndef KSOCKET_HH
#define KSOCKET_HH

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <string>

#if !defined KSOCKET_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
	#include <KError.hh>
#endif

#include <KSaddr.hh>

using namespace std;

#define KSOCKET_STATE_CLOSE		-1
#define KSOCKET_DFLT_ADDRESS	"127.0.0.1"
#define KSOCKET_DFLT_PORT 		0
#define KSOCKET_DFLT_BACKLOG 	128

namespace KNM
{
	/**
	 * A socket class.
	 * Handle TCP and UDP, in IPv4 and IPv6.
	 *
	 * Compilations flags:
	 * - LIBKNM_NO_EXCEPTIONS  : Disable exceptions
	 * - KSOCKET_NO_EXCEPTIONS : Disable exceptions
	 */
	class 	KSocket
	{
	public:
		/**
		 * Socket type.
		 */
		enum type {UDP4, UDP6, TCP4, TCP6};

	public:
		KSocket();											/**< Default constructor. */
		KSocket(enum KSocket::type);						/**< Construct a socket based on predefined types. */
		KSocket(enum KSocket::type, const string &, int);	/**< IPv4 TCP socket, with address and port. */
		KSocket(const KSocket &);							/**< Construct by copy. */
		KSocket &operator=(const KSocket &);				/**< Assignement. */
		bool     operator==(const KSocket &);				/**< True if the both instances are the same. */
		bool     operator==(KSocket *);						/**< True if the both instances are the same. */
		~KSocket();											/**< Destructor. */

		void 		setType(enum KSocket::type);			/**< Set a KSocket type. */
		void 		setAddress(const string &);				/**< Set the IP address. */
		void 		setName(const string &);				/**< Set the IP address or hostname. */
		void 		setPort(int);							/**< Set the endpoint port number. */
		void 		setBlock(bool);							/**< true:socket become blockant, false: non-blockant. */
		void 		setBacklog(int);						/**< Set listen's backlog. */
		KSaddr 		*getSaddr();							/**< Give access to the internal KSaddr class. */

		void 		socket();								/**< Create a socket. */
		void		bind();									/**< Bind to KSocket::port. */
		void 		bind(int);								/**< Bind to specified port. */
		void		listen();								/**< Place the socket in listen mode. */
		void		connect();								/**< Connect to KSocket::address and KSocket::port */
		void 		connect(const string &, int);			/**< Connect to the specified address and port. */
		void 		disconnect();							/**< Close the socket. */
		void 		close();								/**< Close the socket. Synonym to KSocket::disconnect(). */
		void 		shutdown(char = 'a');					/**< Shutdown the socket. */
		KSocket 	*accept();								/**< Accept a connection (blocking). */
		bool 		isReadable();							/**< Return true of we can read from socket. */

		int 		receive(void *, int);					/**< Receive some bytes. */
		int 		receive(int*);							/**< Receive an integer. */
		int 		receiveFrom(void *, int, KSaddr &);		/**< Receive some bytes. */
		int 		receiveFrom(int *, KSaddr &);			/**< Receive an integer. */

		int 		send(void *, int);						/**< Send some bytes. */
		int 		send(const string &);					/**< Send a string.   */
		int 		send(int);								/**< Send an integer. */
		int 		sendTo(void *, int, KSaddr &);			/**< Send some bytes. */
		int 		sendTo(const string &, KSaddr &);		/**< Send a string.   */
		int 		sendTo(int, KSaddr &);					/**< Send an integer. */

		void 		endpoint();								/**< Create and endpoint using current configuration. */
		void 		endpoint(enum KSocket::type,
							 const string &, int);			/**< Create an endpoint using specicied type, address and port. */

	private:
		int 				fd;								/**< Socket file descriptor. */
		int 				backlog; 						/**< listen's backlog. */
		enum KSocket::type 	type;							/**< Connection type. */
		KSaddr 				addr;							/**< IP-version agnostic sockaddr. */

	public:
		class 				pool;
	};
}

#endif