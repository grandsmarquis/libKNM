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
#include <list>
#include <iostream>
#include <fstream>
#include <KTree.hpp>

#if !defined KCONFIG_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
	#include <KError.hh>
#endif

using namespace std;

namespace KNM
{
	/**
	 * Configuration file and list management.
	 * Load configuration files, process them and store options.
	 *
	 * Compilations flags:
	 * - LIBKNM_NO_EXCEPTIONS  : Disable exceptions
	 * - KCONFIG_NO_EXCEPTIONS : Disable exceptions
	 */
	class 	KConfig
	{
	public:
		/**
		 * Nested option's types enum.
		 * Define option type; ALL match with any type.
		 */
		enum types{ALL, BOOL, INTEGER, FLOAT, STRING, LIST, ARRAY};

		/**
		 * Nested option class.
		 * Store option name and value, to be used in a list.
		 */
		struct option
		{
			string 		name;	/**< Option name. */
			string 		data;	/**< Option data. */
			enum types 	type;	/**< Option type. */
		};


	public:
		KConfig();															/**< Default constructor. */
		KConfig(const KConfig &);											/**< Copy constructor. */
		KConfig &operator=(const KConfig &);								/**< Assignment operator. */
		KConfig(const string &);											/**< Construct with a source. */
		void reset();														/**< Reset object to its initial state. */
		~KConfig();															/**< Destructor. */

		// Source control
		void 			load();												/**< Load all sources. */
		void 			load(const string &);								/**< Load all sources. */
		void 			load(int ac, char **av);							/**< Load all sources. */
		void 			include(const string &);							/**< Add a source file. */
		void 			include(int ac, char **av);							/**< Add an option array. */

		// Options configuration
		void 			dflt(const string &, bool);							/**< Set option default bool value. */
		void 			dflt(const string &, int);							/**< Set option default integer value. */
		void 			dflt(const string &, float);						/**< Set option default float value. */
		void 			dflt(const string &, const string &);				/**< Set option default string value. */

		// Options aliases
		void 			alias(const string &, const string &);				/**< Add alias to an option. */

		// Options access
		bool 			is_scope(const string &, const string & = ""); 		/**< Check scope existence and type. */
		bool 			is_option(const string &, enum types = ALL);		/**< Check option existence and type. */
		bool 			find_b(const string &, bool = false);				/**< Return option value as bool. */
		int 			find_i(const string &, int = 0);					/**< Return option value as integer. */
		float 			find_f(const string &, float = 0);					/**< Return option value as float. */
		const string 	&find_s(const string &, const string & = "");		/**< Return option value as string. */
		list<option*> 	&find_l(const string &);							/**< Return option list. */
		option 			**find_a(const string &);							/**< Return option array. */

	private:
		// Source control
		bool	openSource(string *);										/**< Open a source file. */
		void	loadSource();												/**< Start source file processing. */

		// String control
		void 	clearSpaces(string &);										/**< Remove spaces and tabs. */
		void 	clearComment(string &);										/**< Remove inline comments. */
		void 	clearBreaks(string &);										/**< Remove escaped line breaks. */

		// Token control
		bool 	readToken();												/**< Read a token from current source. */
		void 	clearToken();												/**< Remove token impurties. */
		bool 	parseToken();												/**< Start token processing. */
		bool 	handle_comment();											/**< Check if token is a comment. */
		bool 	handle_command();											/**< Check if token is a command. */
		bool 	handle_assignation();										/**< Check if token is an assignation. */
		bool 	handle_scope();												/**< Check if token is a scope declaration. */

		// Members
		KTree<string*>						options;						/**< Options tree. */
		list<string*>						sources;						/**< Sources files list. */
		list<string*>::iterator 			src_itr;						/**< Sources list iterator. */
		string 								source;							/**< Source file name / path. */
		ifstream 							stream;							/**< Current file stream. */
		string 								token; 							/**< Current token. */
		string 								scope; 							/**< Current scope name. */
	};
}

#endif