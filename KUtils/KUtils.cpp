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

#include "KUtils.hh"

using namespace KNM;

string 			KUtils::quote(const string &_line, const string &_start, const string &_end)
{
	string 		line = _line;

	line.insert(0, _start);
	line.insert(line.size(), _end);

	return line;
}

string 			KUtils::unquote(const string &_line, const string &_start, const string &_end)
{
	string 		line = _line;
	size_t 		pos;

	pos = line.find_first_of(_start);
	if (pos != string::npos)line.erase(pos, 1);
	pos = line.find_last_of(_end);
	if (pos != string::npos)line.erase(pos, 1);

	return line;
}

string 			KUtils::replace(const string &_line, const string &_needle, const string &_value)
{
	string 		line = _line;
	size_t		pos;

	while (true)
	{
		pos = line.find(_needle);
		if (pos == string::npos){break;}
		line.erase(pos, _needle.size());
		line.insert(pos, _value);
	}

	return line;
}

string 			KUtils::interpret(const string &_line)
{
	string 		line = _line;

	line = replace(line, "\n\r", "");
	line = replace(line, "\n", "");
	return line;
}

string 			KUtils::epur(const string &_line)
{
	size_t 		i           = 0;
	bool 		escape_st   = false;
	bool 		escape_sq   = false;
	bool 		escape_dq   = false;
	string 		line        = _line;

	while (i <= line.size())
	{
		// Ignore strings
		     if (line[i] == '\'' && escape_sq == false){escape_sq = true; escape_st = true;}
		else if (line[i] == '\'' && escape_sq == true){escape_sq = false; escape_st = false;}
		else if (line[i] == '\"' && escape_dq == false){escape_dq = true; escape_st = true;}
		else if (line[i] == '\"' && escape_dq == true){escape_dq = false; escape_st = false;}

		// Remove superfluous space
		else while (
			(  (i == 0 && (line[i] == ' ' || line[i] == '\t'))
			|| (line[i] == ' ' && line[i + 1] == ' ')
			|| (line[i] == ' ' && line[i + 1] == '\t')
			|| (line[i] == '\t' && line[i + 1] == ' ')
			|| (line[i] == '\t' && line[i + 1] == '\t')
			)
			&& escape_st == false)
		{
			if (line[i + 1] == '\t')
				line[i + 1] = ' ';
			line.erase(i, 1);
		}
		i++;
	}
	return line;
}

string 			KUtils::endlike_unix(const string &_line)
{
	string 		line = _line;

	// DOS mode
	if (line[line.size() - 2] == '\r' && line[line.size() - 1] == '\n')
		return replace(line, "\r\n", "\n");

	// UNIX mode
	else if (line[line.size() - 1] == '\n')
		return line;

	// No mode
	else
		line.insert(line.size(), "\n");
	return line;
}

string 			KUtils::endlike_dos(const string &_line)
{
	string 		line = _line;

	// UNIX mode
	if (line[line.size() - 1] == '\n' && line[line.size() - 2] != '\r')
	{
		line.insert(line.size() - 1, "\r");
		return line;
	}

	// DOS mode
	else if (line[line.size() - 2] == '\r' && line[line.size() - 1] == '\n')
		return line;

	// No mode
	else
		line.insert(line.size(), "\r\n");
	return line;
}

string 			KUtils::endlike_none(const string &_line)
{
	string 		line = _line;

	// UNIX mode
	if (line[line.size() - 1] == '\n' && line[line.size() - 2] != '\r')
		line.erase(line.size() - 1, 1);

	// DOS mode
	else if (line[line.size() - 2] == '\r' && line[line.size() - 1] == '\n')
		line.erase(line.size() - 2, 2);

	return line;
}

int 			KUtils::to_int(const string &_num)
{
	return atoi(_num.c_str());
}

float 			KUtils::to_float(const string &_num)
{
	return strtof(_num.c_str(), NULL);
}

bool 			KUtils::to_bool(const string &_num)
{
	if (_num == "TRUE" || _num == "true" || _num == "YES" || _num == "yes" ||
		_num == "ENABLE" || _num == "enable" || _num == "OK" || _num == "ok")
		return true;
	return false;
}

string 				KUtils::to_string(int _num)
{
	string 			num;
	ostringstream 	convert;

	convert << _num;
	num = convert.str();
	return num;
}

string 				KUtils::to_string(float _num)
{
	string 			num;
	ostringstream 	convert;

	convert << _num;
	num = convert.str();
	return num;
}

string 				KUtils::to_string(bool _num)
{
	string 			num;
	ostringstream 	convert;

	convert << _num;
	num = convert.str();
	return num;
}
