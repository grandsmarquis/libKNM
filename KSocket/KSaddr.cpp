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

#include <KSaddr.hh>

using namespace KNM;

KSaddr::KSaddr()
{
	addr4.sin_addr.s_addr = INADDR_ANY;
	addr6.sin6_addr       = in6addr_any;
	setDomain(IPV4);
}

/**
 * @param _domain Socket domain (family)
 */
KSaddr::KSaddr(int _domain)
{
	addr4.sin_addr.s_addr = INADDR_ANY;
	addr6.sin6_addr       = in6addr_any;
	setDomain(_domain);
}

/**
 * @param _ref Object to build from
 */
KSaddr::KSaddr(const KSaddr &_ref)
{
	if (&_ref != this)
	{
		type = _ref.type;
		if (type == IPV4)
			addr4 = _ref.addr4;
		else if (type == IPV6)
			addr6 = _ref.addr6;
	}
}

/**
 * @param _ref Object to build from
 */
KSaddr &KSaddr::operator=(const KSaddr &_ref)
{
	if (&_ref != this)
	{
		type = _ref.type;
		if (type == IPV4)
			addr4 = _ref.addr4;
		else if (type == IPV6)
			addr6 = _ref.addr6;
	}
	return *(this);
}

/**
 * @param _domain Socket domain (family)
 */
void 					KSaddr::setDomain(int _domain)
{
	if (_domain == AF_INET)
	{
		type = IPV4;
		addr4.sin_family = AF_INET;
	}

	else if (_domain == AF_INET6)
	{
		type = IPV6;
		addr6.sin6_family = AF_INET6;
	}

	else
	{
		type = IPV4;
		addr4.sin_family = AF_INET;
	}
}

/**
 * @param _port Port
 */
void 					KSaddr::setPort(int _port)
{
	if (type == IPV4)
		addr4.sin_port = htons(_port);
	else if (type == IPV6)
		addr6.sin6_port = htons(_port);
}

int 					KSaddr::getPort()
{
	if (type == IPV4)
		return ntohs(addr4.sin_port);
	else if (type == IPV6)
		return ntohs(addr6.sin6_port);
	return 0;
}

/**
 * @param _address Address in IPv4 or IPv6
 */
void 					KSaddr::setAddress(const string &_address)
{
	if (type == IPV4)
		inet_pton(AF_INET, _address.c_str(), &addr4.sin_addr.s_addr);
	else if (type == IPV6)
		inet_pton(AF_INET6, _address.c_str(), &addr6.sin6_addr.s6_addr);
}

const string 			&KSaddr::getAddress()
{
	char 				buffer[INET6_ADDRSTRLEN];

	if (type == IPV4)
		inet_ntop(AF_INET, &addr4.sin_addr, buffer, sizeof(buffer));
	else if (type == IPV6)
		inet_ntop(AF_INET6, &addr6.sin6_addr, buffer, sizeof(buffer));

	address.assign(buffer);
	return address;
}

/**
 * @param _name IPv4 or IPv6 address, or hostanme
 */
void 					KSaddr::setName(const string &_name)
{
	struct addrinfo 		*hits = NULL;
	struct sockaddr_in 		*dest4;
	struct sockaddr_in6 	*dest6;
	int 					ret;

	ret = getaddrinfo(_name.c_str(), NULL, NULL, &hits);
	if (ret == 0)
	{
		if (type == IPV4)
		{
			dest4 = (struct sockaddr_in*)hits->ai_addr;
			addr4.sin_addr = dest4->sin_addr;
		}
		else if (type == IPV6)
		{
			dest6 = (struct sockaddr_in6*)hits->ai_addr;
			addr6.sin6_addr = dest6->sin6_addr;
		}
	}
}

int 					KSaddr::getDomain()
{
	if (type == IPV6)
		return addr6.sin6_family;
	return addr4.sin_family;
}

struct sockaddr			*KSaddr::getSaddr()
{
	if (type == IPV6)
		return (struct sockaddr*)&this->addr6;
	else
		return (struct sockaddr*)&this->addr4;
}

int 					KSaddr::getSaddrSize()
{
	if (type == IPV6)
		return sizeof(this->addr6);
	return sizeof(this->addr4);
}
