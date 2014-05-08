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

#ifndef KCONFIG_HH
#define KCONFIG_HH

#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#if !defined KCONFIG_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
	#include <KError.hh>
#endif

using namespace std;

namespace KNM
{
	/**
	 * Configuration file and list management.
	 *
	 * Compilations flags:
	 * - LIBKNM_NO_EXCEPTIONS  : Disable exceptions
	 * - KCONFIG_NO_EXCEPTIONS : Disable exceptions
	 */
	class 	KConfig
	{
	public:
		KConfig();																			/**< Default constructor. */
		KConfig(const string &);															/**< Open and load a file during construction. */

		void 			setSource(const string &);											/**< Use argument as source file. */
		string 			getOption(const string &, const string &_dflt = "");				/**< Return option's value as string. */
		int 			getIOption(const string &, int _dflt = 0);							/**< Return option's value as integer. */
		void 			setOption(const string &, const string &);							/**< Set an option with a value. */

		static string	getopt(int, char **, const string &, const string &_dflt = "");		/**< Get an option from a bytes array table. */
		static bool		isopt(int, char **, const string &);								/**< Check if an option exists in a bytes array table. */

	private:
		
		/**
		 * Nested option class.
		 * Store option name and value, to be used in a list.
		 */
		struct Option
		{
			string key;											/**< Option name. */
			string value;										/**< Option value. */
		};

		bool	openSource();									/**< Open a source file. */
		void	loadSource();									/**< Load a source file. */

		ifstream 							stream;
		string 								source;				/** Source file name / path. */
		vector<KConfig::Option*>			options;			/**< Options list. */
		vector<KConfig::Option*>::iterator	itr;				/**< Options list iterator. */
	};
}

#endif