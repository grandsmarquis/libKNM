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

#ifndef REGISTRY_LEAF_HH
#define REGISTRY_LEAF_HH

#include <string>

using namespace std;

namespace registry
{
	/**
	 * A leaf designed to be used in a registry tree.
	 */
	template <class T>
	class 		leaf
	{
	public:
		leaf(){}
		leaf(const string &_name): name(_name){};
		leaf(const string &_name, const T &_data): name(_name), data(_data){};

		const string 	&getName(){return this->name;}
		const T 		&getData(){return this->data;}
		void 			rename(const string &_name){this->name = _name;}
		void 			assign(const T &_data){this->data = _data;}
		void 			operator=(const T &_data){assign(_data);}

	private:
		string 			name;
		T 				data;
	};
}


#endif