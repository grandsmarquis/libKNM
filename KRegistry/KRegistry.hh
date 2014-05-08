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

#ifndef	KREGISTRY_HH
#define	KREGISTRY_HH

#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <libgen.h>
#include <KError.hh>
#include <KMutex.hh>
#include "node.hpp"
#include "leaf.hpp"

using namespace std;
using namespace registry;

namespace KNM
{
	/**
	 * Store and access informations in a tree.
	 * This class focuses on keys as string.
	 */
	class 	KRegistry: public KMutex
	{
	public:
		KRegistry();										/**< Default constructor. */
		KRegistry(const string &);							/**< Define a file to backup the registry. */
		~KRegistry();										/**< Destructor. */

		// Setting
		void 	setFile(const string &);					/**< Set the file to use to backup registry. */
		void 	closeFile();								/**< Close file. */
		void 	load(const string &);						/**< Load keys from a file. */

		// Registry access methods
		void 	insert(const string &);						/**< Insert a key. */
		void 	insert(const string &, const string &);		/**< Insert a key and its value. */
		void 	mknode(const string &);						/**< Insert a node. */
		void 	update(const string &, const string &);		/**< Update a key value. */
		string 	access(const string &);						/**< Return a copy of a key value. */
		void 	sync(const string &, const string &);		/**< Add changes to a file. */

	private:
		node<string> 	root;
		fstream 		file;
	};
}


#endif