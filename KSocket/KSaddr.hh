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

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <string>

using namespace std;

#define IPV4 	0
#define IPV6 	1

namespace KNM
{
	/**
	 * KSaddr represent an IP endpoint structure.
	 * Suitable for IPv4 and IPv6.
	 */
	class 	KSaddr
	{
	public:
		KSaddr();											/**< Default constructor. */
		KSaddr(int);										/**< Specify the domain (== family) type. */
		KSaddr(const KSaddr &);								/**< Construct by copy. */
		KSaddr &operator=(const KSaddr &);					/**< Assignment operator. */

		void 				setDomain(int);					/**< Set domain (== family) type. */
		void 				setPort(int);					/**< Set port. */
		void 				setAddress(const string &);		/**< Set IP address manually. */
		const string 		&getAddress();					/**< Return the IP address in a human-readable form. */
		void 				setName(const string &);		/**< Set IP address or hostname. */
		int 				getDomain();					/**< Return domain (== family) type. */
		int 				getPort();						/**< Return port. */
		struct sockaddr 	*getSaddr();					/**< Return a pointer on the internal structure. */
		int 				getSaddrSize();					/**< Return the size of the internal structure. */

	private:
		int 					type;						/**< IPV4 or IPV6. */
		struct sockaddr_in 		addr4;						/**< IPV4 internal structure. */
		struct sockaddr_in6		addr6;						/**< IPV6 internal structure. */
		string 					address;					/**< User-friendly IP address container. */
	};
}