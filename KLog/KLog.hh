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

#ifndef KLOG_HH
#define KLOG_HH

#include <string>
#include <iostream>
#include <fstream>

#include <KError.hh>
#include <KTime.hh>
#include <KMutex.hh>

using namespace std;

namespace KNM
{
	enum KLogLevel {CRITICAL, ERROR, WARNING, INFO};
}

namespace KNM
{
	/**
	 * Control log writing.
	 * KLog write log in a thread-safe way (KLog work in append mode).
	 * Because AI use it's own log file location, KLog doesn't use the syslog() API.
	 */
	class 	KLog : public KMutex
	{
	public:
		KLog();													/**< Default constructor. */	
		KLog(const string &);									/**< Construct with a log file to use as parameter. */
		~KLog();												/**< Default destructor. */

		void	setFile(const string &);						/**< Set the file to be used to write log entries. */
		void 	setLevel(const enum KLogLevel);					/**< Set the default level for new entries (default is INFO). */
		void 	setMinLevel(const enum KLogLevel);				/**< Set minimum alerts level. */
		void	write(const string &);							/**< Write a new log entry with default level. */
		void	write(const enum KLogLevel, const string &); 	/**< Write a new log entry, with the specified level. */

	private:
		ofstream 			logfile;							/**< Log file stream. */
		enum KLogLevel 		deflvl;								/**< New entries default level. */
		enum KLogLevel 		minlvl;								/**< Do not log even with a level < to this. */
		KTime 				time;								/**< Time object. */
	};
}


#endif
