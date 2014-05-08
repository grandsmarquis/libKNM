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

#include "kalert.h"

void 	kalert_init(struct kalert_s *_alert)
{
	if (!_alert){return;}

	_alert->level = (enum kalert_level)KALERT_DFLTLVL;
	memset(_alert->orig, 0, KALERT_ORIG_LEN);
	memset(_alert->type, 0, KALERT_TYPE_LEN);
	memset(_alert->data, 0, KALERT_DATA_LEN);
}

void 	kalert_setLevel(struct kalert_s *_alert, int _level)
{
	if (!_alert){return;}

	if (_level >= INFO && _level <= CRITICAL)
		_alert->level = (enum kalert_level)_level;
}

void 	kalert_setOrig(struct kalert_s *_alert, const char *_orig)
{
	if (!_alert || !_orig){return;}

	memcpy(_alert->orig, _orig, KALERT_ORIG_LEN - 1);
}

void 	kalert_setType(struct kalert_s *_alert, const char *_type)
{
	if (!_alert || !_type){return;}

	memcpy(_alert->type, _type, KALERT_TYPE_LEN - 1);
}

void 	kalert_setData(struct kalert_s *_alert, const char *_data)
{
	if (!_alert || !_data){return;}

	memcpy(_alert->data, _data, KALERT_DATA_LEN - 1);
}

void 	kalert_set(struct kalert_s *_alert, int _level, const char *_orig, const char *_type, const char *_data)
{
	if (!_alert || !_orig || !_type || !_data){return;}

	kalert_setLevel(_alert, _level);
	kalert_setOrig(_alert, _orig);
	kalert_setType(_alert, _type);
	kalert_setData(_alert, _data);
}