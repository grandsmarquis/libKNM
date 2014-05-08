/*
* libKNM
* Copyright (C) 2014 Jean-Philippe Clipffel, Mathieu Cransac
* Email: jp.clipffel@gmail.com, cransa_m@epitech.eu
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

#ifndef KALERT_HH
#define KALERT_HH

#include <string>
#include <list>
#include "kalert.h"
#include <KMutex.hh>

using namespace std;

#define KALERT_LIST_UNLIMITED 	-1

namespace KNM
{
	/**
	 * Represent a kalert in C++.
	 */
	class 	KAlert
	{
	public:
		KAlert();									/**< Default constructor. */
		KAlert(const KAlert &);						/**< Construct by copy. */
		KAlert &operator=(const KAlert &);			/**< Assignment operator. */
		~KAlert();									/**< Destructor. */

		void 		setLevel(enum kalert_level);	/**< Set alert level. */
		void 		setOrig(const string &);		/**< Set alert origin / source. */
		void 		setType(const string &);		/**< Set alert type. */
		void 		setData(const string &);		/**< Set alert data. */
		void 		set(enum kalert_level, 			/**< Set everyting at once. */
					const string &, 
					const string &, 
					const string &);

		enum kalert_level		getLevel();			/**< Return the alert level. */
		const char 				*getOrig();			/**< Return the alert origin. */
		const char 				*getType();			/**< Return the alert type. */
		const char 				*getData();			/**< Return the alert data. */

		const struct kalert_s 	&getStruct();		/**< Return the alert structure. */

	private:
		struct kalert_s 		alert;				/**< Local alert structure. */

	public:

		/**
		 * Netsed list class.
		 * Used to store KAlert and access them by date / priority.
		 */
		class 	list
		{
		public:
			list();							/**< Default constructor. */
			list(int);						/**< Construct with a limited size. */

			void 	insert(KAlert &);		/**< Insert a new KAlert. */
			KAlert 	getLast();				/**< get last KAlert*/
			KAlert 	getPrior();				/**< get the more last important KAlert*/
			int 	getLimit();				/**< get the limit */
			void 	setLimit(int);			/**< set the limit*/

		private:
			int 				limit;		/**< List size limit. */
			std::list<KAlert>	queue;		/**< List. */
			KMutex				mutex;		/**< List access mutex. */
		};

	    static list lst;
	};
}

#endif
