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

#include "KError.hh"

using namespace std;
using namespace KNM;

KError::KError()
{
	this->from = "()";
	this->code = KERROR_CODE_DFLT;
	this->info = "";
}

/**
 * @param _code Error code (ex: errno)
 */
KError::KError(int _code)
{
	this->from = "()";
	this->code = _code;
}

/**
 * @param _from Which class/funtion has send throw the error
 */
KError::KError(const string &_from)
{
	if (_from.length() > 0)
		this->from = _from;
	else
		this->from = "()";
	this->code = KERROR_CODE_DFLT;
	this->info = "";
}

/**
 * @param _code Error code (ex: errno)
 * @param _from Which class/funtion has send throw the error
 */
KError::KError(int _code, const string &_from)
{
	if (_from.length() > 0)
		this->from = _from;
	else
		this->from = "()";
	this->code = _code;
	this->info = "";
}

/**
 * @param _from Which class/funtion has send throw the error
 * @param _info Information about error (source, arguments, etc.)
 */
KError::KError(const string &_from, const string &_info)
{
	if (_from.length() > 0)
		this->from = _from;
	else
		this->from = "()";
	this->code = KERROR_CODE_DFLT;
	if (_info.length() > 0)
		this->info = _info;
	else
		this->info = "()";
}

void	KError::dump()
{
	if (this->code == KERROR_CODE_DFLT)
		cerr << this->from << ": " << this->info << endl;
	else
		cerr << this->from << ": " << strerror(this->code) << endl;
}

string 	KError::what()
{
	if (this->code == KERROR_CODE_DFLT)
		return this->from + ": " + this->info;
	else
		return this->from + ": " + strerror(this->code);
}

int 	KError::errnum()
{
	return code;
}