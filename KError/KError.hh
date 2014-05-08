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

#ifndef KERROR_HH
#define KERROR_HH

#include <string>
#include <string.h>
#include <iostream>

#define KERROR_CODE_DFLT	0

using namespace std;

namespace KNM
{
	/**
	 * Generic exception class.
	 * Allow to build complete and easy-to-use exceptions.
	 */
	class KError
	{
	public:
		KError();									/**< New empty KError. */
		KError(int);								/**< New KError with a return code. */
		KError(const string &);						/**< New KError with a description (typically a class name). */
		KError(int, const string &);				/**< New KError with a return code and a description (typically a class name). */
		KError(const string &, const string &);		/**< New KError with 2 descriptions. */
		//KError(const KError &);						/**< Construct by copy. */
		//KError &operator=(const KError &);			/**< Assignement. */
		//~KError();									/**< Destructor. */

		void 			dump();						/**< Print error content. */
		string 			what();						/**< Return error string. */
		int 			errnum();					/**< Return error number. */

	private:
		string 			from;
		string 			info;
		int 			code;
	};
}

#endif