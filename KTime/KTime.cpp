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

#include "KTime.hh"

using namespace std;
using namespace KNM;

KTime::KTime(){}

void 		KTime::refresh()
{
	this->stime = time(NULL);
	this->ltime = *localtime(&(this->stime));
}

signed int 	KTime::epoch()
{
	refresh();
	return((signed int)this->stime);
}

signed int 	KTime::sec()
{
	refresh();
	return(this->ltime.tm_sec);
}

signed int 	KTime::min()
{
	refresh();
	return(this->ltime.tm_min);
}

signed int 	KTime::hour()
{
	refresh();
	return(this->ltime.tm_hour);
}

signed int 	KTime::day()
{
	refresh();
	return(this->ltime.tm_mday);
}

signed int 	KTime::month()
{
	refresh();
	return(this->ltime.tm_mon + 1);
}

signed int 	KTime::year()
{
	refresh();
	return(this->ltime.tm_year + 1900);
}