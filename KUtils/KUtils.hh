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

#ifndef KUTILS_HH
#define KUTILS_HH

#include <stdlib.h>
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

namespace KNM
{
	/**
	 * Text utils.
	 * Avoid rewritting small snippets every two hours.
	 */
	class 	KUtils
	{
	public:
		static string 			quote(const string &, const string & = "\"",
									  const string & = "\"");								/**< Wrap the string between quotes. */
		static string 			unquote(const string &, const string & = "\"",
										const string & = "\"");								/**< Unwrap the string between quotes. */
		static string 			replace(const string &, const string &, const string &);	/**< Replace occurence in string. */
		static string 			interpret(const string &);									/**< Replace '\*' characters with their ASCII equivalent. */
		static string 			epur(const string &);										/**< Remove string multuiples spaces, return and tabs. */
		static string 			endlike_unix(const string &);								/**< Ensure that the string end with "\n". */
		static string 			endlike_dos(const string &);								/**< Ensure that the string end with "\n\r". */
		static string 			endlike_none(const string &);								/**< Ensure that the string doesnt have a line-terminaison. */

		static int 				to_int(const string &);										/**< Convert a string to an integer. */
		static float 			to_float(const string &);									/**< Convert a string to a float. */
		static bool 			to_bool(const string &);									/**< Convert a string to a boolean. */
		static string 			to_string(int);												/**< Convert an integer to a string. */
		static string 			to_string(float);											/**< Convert a float to a string. */
		static string 			to_string(bool);											/**< Convert a boolean to a string. */

		//static list<string>		build_list(const string &, const string & = ",");			/**< Build a list string from a single string. */
		//static vector<string>	build_vector(const string &, const string & = ",");			/**< Build a vector of string from a single string. */
	};
}

#endif