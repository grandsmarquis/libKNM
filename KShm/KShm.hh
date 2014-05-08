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

#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <KError.hh>

#define 	KSHM_DFLT_OFLAGS	O_CREAT|O_RDWR
#define 	KSHM_DFLT_MODE		S_IRWXU|S_IRWXG|S_IRWXO

using namespace std;

namespace KNM
{
	/**
	 * Manage shared memory.
	 */
	class 		KShm
	{
	public:
		KShm();								/**< Default constructor. */
		KShm(const string &);				/**< Specify a name during construction. */
		KShm(const KShm&);					/**< Construct by copy. */
		KShm &operator=(const KShm&);		/**< Assignment operator. */
		~KShm();							/**< Destructor. */

		void 	setName(const string &);	/**< Set SHM name. */
		void 	open();						/**< Open the current SHM. */
		void 	open(const string &);		/**< Open the SHM pointed by name. */
		void 	close();					/**< Close the SHM. */
		bool 	isOpen();					/**< Check if SHM is open. */
		void 	unlink();					/**< Unlink the SHM. */


	private:
		string 	name;						/**< SHM name. */
		int 	fd;							/**< SHM file descriptor. */
	};
}