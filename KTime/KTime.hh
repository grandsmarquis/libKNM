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

#ifndef KTIME_HH
#define KTIME_HH

#include <time.h>

namespace KNM
{
	/**
	 * Get and manage date and time.
	 */
	class 			KTime
	{
	public:
		KTime();					/**< Default constructor. */

		void 		refresh();		/**< Refresh time. Implicitly called by other methods. */

		signed int 	epoch();		/**< Seconds since epoch. */
		signed int 	sec();			/**< Current seconds. */
		signed int 	min();			/**< Current minutes. */
		signed int 	hour();			/**< Current hour. */
		signed int 	day();			/**< Current day (numeric). */
		signed int 	month();		/**< Current month (numeric). */
		signed int 	year();			/**< Current year. */

	private:
		time_t 		stime;
		struct tm 	ltime;
	};
}

#endif