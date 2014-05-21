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

#include "KLog.hh"

using namespace std;
using namespace KNM;

KLog::KLog()
{
	this->deflvl = KLogLevel(INFO);
	this->minlvl = KLogLevel(INFO);
}

KLog::KLog(const string &_name)
{
	setFile(_name);
	this->deflvl = KLogLevel(INFO);
	this->minlvl = KLogLevel(INFO);
}

KLog::~KLog()
{
	this->logfile.close();
}

/**
 * @param _name configuration file path
 */
void 	KLog::setFile(const string &_name)
{
	lock();
	this->logfile.open(_name.c_str(), ios::out | ios::app);
	if (!this->logfile.is_open())
	{
		unlock();
		throw new KError("KLog::KLog()", "Cannot open logfile");
	}
	unlock();
}

/**
 * @param lvl Log level
 */
void 	KLog::setLevel(const enum KLogLevel lvl)
{
	lock();
	this->deflvl = KLogLevel(lvl);
	unlock();
}

void 	KLog::setMinLevel(const enum KLogLevel lvl)
{
	lock();
	this->minlvl = KLogLevel(lvl);
	unlock();
}

/**
 * @param _data entry (text) to write
 */
void 				KLog::write(const string &_data)
{
	this->write(this->deflvl, _data);
}

/**
 * @param level Entry level
 * @param _data entry (text) to write
 */
void 				KLog::write(const enum KLogLevel level, const string &_data)
{
	if (level > this->minlvl)
		return;

	lock();
	time.refresh();
	if (logfile.is_open() && logfile.good())
	{
		logfile << "[" << time.day() << "/" << time.month() << "/" << time.year();
		logfile << ", " << time.hour() << ":" << time.min() << ":" << time.sec() << "] ";
	}
	switch (level)
	{
		case INFO:
			if (logfile.is_open()){logfile << "[INFO] ";}
			break;
		case WARNING:
			if (logfile.is_open()){logfile << "[WARNING] ";}
			break;
		case ERROR:
			if (logfile.is_open()){logfile << "[ERROR] ";}
			break;
		case CRITICAL:
			if (logfile.is_open()){logfile << "[CRITICAL] ";}
			break;
		default:
			if (logfile.is_open()){logfile << "[?] ";}
			break;
	}

	if (logfile.is_open() && logfile.good())
		logfile << _data << endl;
	unlock();
}