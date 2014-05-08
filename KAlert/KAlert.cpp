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

#include "KAlert.hh"

using namespace std;
using namespace KNM;

KAlert::KAlert()
{
	kalert_init(&this->alert);
}

KAlert::KAlert(const KAlert &_ref)
{
	if (&_ref != this)
	{
		setLevel(_ref.alert.level);
		setOrig(_ref.alert.orig);
		setType(_ref.alert.type);
		setData(_ref.alert.data);
	}
}

KAlert 	&KAlert::operator=(const KAlert &_ref)
{
	if (&_ref != this)
	{
		setLevel(_ref.alert.level);
		setOrig(_ref.alert.orig);
		setType(_ref.alert.type);
		setData(_ref.alert.data);
	}
	return *(this);
}

KAlert::~KAlert(){}

void 	KAlert::setLevel(enum kalert_level _level)
{
	kalert_setLevel(&this->alert, _level);
}

void 	KAlert::setOrig(const string &_orig)
{
	kalert_setOrig(&this->alert, _orig.c_str());
}

void 	KAlert::setType(const string &_type)
{
	kalert_setType(&this->alert, _type.c_str());
}

void 	KAlert::setData(const string &_data)
{
	kalert_setData(&this->alert, _data.c_str());
}

void 	KAlert::set(enum kalert_level _level, 
					const string &_orig, 
					const string &_type, 
					const string &_data)
{
	kalert_setLevel(&this->alert, _level);
	kalert_setOrig(&this->alert, _orig.c_str());
	kalert_setType(&this->alert, _type.c_str());
	kalert_setData(&this->alert, _data.c_str());
}

enum kalert_level 		KAlert::getLevel()
{
	return alert.level;
}

const char 			*KAlert::getOrig()
{
	return (alert.orig);
}

const char 			*KAlert::getType()
{
	return (alert.type);
}

const char 			*KAlert::getData()
{
	return (alert.data);
}

const struct kalert_s 	&KAlert::getStruct()
{
	return alert;
}

KAlert::list::list(): limit(KALERT_LIST_UNLIMITED)
{

}

KAlert::list::list(int _limit): limit(_limit)
{

}

void 	KAlert::list::insert(KAlert &alert)
{
	mutex.lock();
	if (limit != -1 && (int)queue.size() >= limit)
	{
		queue.pop_front();
	}
	queue.push_back(alert);
	mutex.unlock();
}

KAlert 		KAlert::list::getLast()
{
	KAlert 	alertRet;

	mutex.lock();
	if (queue.size() > 0)
	{
		alertRet = queue.back();
		queue.pop_back();
	}
	mutex.unlock();
	return (alertRet);
}

KAlert 							KAlert::list::getPrior()
{
	KAlert 						alertRet;
	std::list<KAlert>::iterator	itrem, it;

	mutex.lock();
	if (queue.size() > 0)
	{
		itrem = queue.begin();
		for (it = queue.begin(); it != queue.end(); ++it)
		{
			if (it->getLevel() >= itrem->getLevel())
			itrem = it;
		}
		alertRet = *itrem;
		queue.erase(itrem);
	}
	mutex.unlock();
	return (alertRet);
}

int	KAlert::list::getLimit()
{
	return (limit);
}

void	KAlert::list::setLimit(int _limit)
{
	limit = _limit;
}
