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

#ifndef KDYLIB_HH
#define KDYLIB_HH

#include <string>
#include <dlfcn.h>
#include <KError.hh>

using namespace std;

namespace KNM
{
	/**
	 * Manage a dynamic library.
	 * A wrapper arround dl* libc functions.
	 *
	 * Compilations flags:
	 * - LIBKNM_NO_EXCEPTIONS  : Disable exceptions
	 * - DKDYLIB_NO_EXCEPTIONS : Disable exceptions
	 */
	class 	KDylib
	{
	public:
		KDylib();									/**< Default constructor. */
		KDylib(const string &);						/**< Construct with a library path. */
		KDylib(const KDylib &);						/**< Construct by copy. */
		KDylib 	&operator=(const KDylib &);			/**< Assignment operator. */
		~KDylib();									/**< Default destructor. */

		void 			setPath(const string &);	/**< Set library path. */
		const string 	&getPath();					/**< Return the path. */
		void 			open(int flag = RTLD_LAZY);	/**< Open library. Can raise a KError. */
		void 			close();					/**< Close the library (if open). */
		const string 	&error();					/**< Return the dl* error. */
		void 			*symbol(const string &);	/**< Get the symbol (open the lib if needed). Can raise a KError. */
		bool 			isOpen();					/**< Return true if lib is open. */

	private:
		string 		path;							/**< Library path. */
		void 		*libfd;							/**< Library handler. */
		string 		lastError;						/**< dl* error string. */
	};
}

#endif