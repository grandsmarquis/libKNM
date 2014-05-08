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

#ifndef KNM_ALERT
#define KNM_ALERT

#include <string.h>

#define KALERT_ORIG_LEN 	16
#define KALERT_TYPE_LEN 	16
#define KALERT_DATA_LEN		32
#define KALERT_DFLTLVL		1

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * Alert level.
	 */
	enum kalert_level
	{
		LOG,INFO,WARNING,ERROR,CRITICAL
	};

	/**
	 * Alert structure.
	 */
	struct kalert_s
	{
		enum kalert_level 	level;							/**< Alert level. */
		char 				orig[KALERT_ORIG_LEN];			/**< Agent / module who emit the alert. */
		char 				type[KALERT_TYPE_LEN];			/**< Help to define which module(s) should handle the alert. */
		char 				data[KALERT_DATA_LEN];			/**< Alert data. */

	};
	typedef struct kalert_s kalert;

#ifdef __cplusplus
}
#endif

void 	kalert_init(struct kalert_s *);
void 	kalert_setLevel(struct kalert_s *, int );
void 	kalert_setOrig(struct kalert_s *, const char *);
void 	kalert_setType(struct kalert_s *, const char *);
void 	kalert_setData(struct kalert_s *, const char *);
void 	kalert_set(struct kalert_s *, int , const char *, const char *, const char *);

#endif