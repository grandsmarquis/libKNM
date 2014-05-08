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

#ifndef KPROCESS_HH
#define KPROCESS_HH

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <string>
#include <list>
#include <iostream>

using namespace std;

namespace KNM
{
	/**
	 * External process manager.
	 * Configure, start, stop and control an process detached from the current one.
	 */

	class 	KProcess
	{
	public:
		KProcess();									/**< Create (not start) a empty process, not configured. */
		KProcess(const string &);					/**< Create (not start) a process using the specified command. */
		KProcess(int);								/**< Construct attached to a running process. */

		void	attach(int);						/**< Attach to a running process. */

		bool	start();							/**< Start the process. */
		bool 	signal(int);						/**< Send a signal. */
		bool	stop();								/**< Stop the process. */
		bool 	pause();							/**< Pause the process. */
		bool 	resume();							/**< Resume the process. */
		bool	status();							/**< Return the process state. true: running, false: not running. */

		void 	setName(const string &);			/**< Set process name (==command to execute). */
		void 	addOption(const string &);			/**< Add an option to process before execution. */
		void 	getOption(const int, char **);		/**< Set process options from C standrad options table. */
		void 	getEnv(char **);					/**< Set process environment using C standrad options table. */

	private:
		pid_t 						PID;			/**< Process PID. */
		string 						name;			/**< Process name. */
		list<string*>				options;		/**< Process option list. */
		list<string*>::iterator		itr_options;	/**< Process option list iterator. */
		char 						**args;			/**< Process option table. */
		char 						**env;			/**< Process env. rules table. */
	};
}

#endif