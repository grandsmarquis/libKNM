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

#include "KConfig.hh"

using namespace std;
using namespace KNM;

KConfig::KConfig()
{
	this->source = "";
}

/**
 * @param _source Source file to load
 */
KConfig::KConfig(const string &_source)
{
	this->source = _source;
	this->openSource();
	this->loadSource();
}

/**
 * @param _source Source file to load
 */
void 		KConfig::setSource(const string &_source)
{
	this->source = _source;
	this->loadSource();
}

bool 		KConfig::openSource()
{
	if (this->stream.is_open())
		this->stream.close();

	this->stream.open(this->source.c_str());
	if (!this->stream.is_open())
		return(false);
	return(true);
}

void			KConfig::loadSource()
{
	string 		line;
	string 		name;
	string 		value;
	size_t 		i;
	size_t 		j;

	if (!this->openSource())
		#if !defined KCONFIG_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
			throw(new KError("Could not open source file", this->source));
		#else
			return;
		#endif

	while (this->stream)
	{
		getline(this->stream, line);			// Read a configuration line

		if (line[0] == '#'
		|| line[0] == ';' 
		|| line[0] == '\n' 
		|| line[0] == '\r') continue;			// If line is a comment, go next

		// Escape whitespaces and tab
		i = 0;
		while (i < line.length() && line[i] == ' ' && line[i] == '\t')
			i++;

		// Read option name
		j = i;
		while (j < line.length() && line[j] != ' ' && line[j] != '\t' && line[j] != '=')
			j++;
		name = line.substr(i, j);

		// Escape whitespaces and tab
		i = j;
		while (i < line.length() && (line[i] == ' ' || line[i] == '\t' || line[i] == '='))
			i++;

		// Read option value
		j = i;
		while (j < line.length() && line[j] != ' ' && line[j] != '\t')
			{
				if (line[j] == '"' || line[j] == '\'')
				{
					i++;
					j++;
					while (j < line.length() && line[j] != '"' && line[j] != '\'')
						j++;
					break;
				}
				j++;
			}
		value = line.substr(i, j-i);
		setOption(name, value);
	}
	this->stream.close();
}

/**
 * @param _key  Option name to found
 * @param _dflt Option default value (if not found)
 */
string		KConfig::getOption(const string &_key, const string &_dflt)
{
	this->itr = this->options.begin();
	while (itr != this->options.end())
	{
		if ((*itr)->key == _key)
			return((*itr)->value);
		itr++;
	}
	return(_dflt);
}

/**
 * @param _key  Option name to found
 * @param _dflt Option default value (if not found)
 */
int 		KConfig::getIOption(const string &_key, int _dflt)
{
	string 	opt;

	/*opt = getOption(_key);
	if (opt.length() < 1)
		return _dflt;
	else
		return atoi(opt.c_str());*/

	this->itr = this->options.begin();
	while (itr != this->options.end())
	{
		if ((*itr)->key == _key)
		{
			opt = (*itr)->value;
			return(atoi(opt.c_str()));
		}
		itr++;
	}
	return(_dflt);
}

/**
 * @param _key Option anme to set
 * @param _value Value to give to the option
 */
void 					KConfig::setOption(const string &_key, const string &_value)
{
	KConfig::Option 	*option; 

	this->itr = this->options.begin();
	while (itr != this->options.end())		// If option exists
	{
		if ((*itr)->key == _key)
		{
			(*itr)->value = _value;
			return;
		}
		itr++;
	}
	option = new Option();					// If option doesn't exists
	option->key = _key;
	option->value = _value;
	this->options.push_back(option);
}

/**
 * @param ac Number of items in av
 * @param av Table of options (a C array)
 * @param _needle Option name to found
 */
string 		KConfig::getopt(int ac, char **av, const string &_needle, const string &_dflt)
{
	int 	i;

	if (ac > 0 && av && _needle.length() >= 1)
	{
		for (i = 0; i < ac; ++i)
		{
			if (_needle == av[i] && i + 1 < ac)
				return(av[i+1]);
		}
	}
	return(_dflt);
}

int 		KConfig::getiopt(int ac, char **av, const string &_needle, int _dflt)
{
	string 	res;

	res = KConfig::getopt(ac, av, _needle);
	if (res.length() < 1)
		return(_dflt);
	else
		return(atoi(res.c_str()));
}

/**
 * @param ac Number of items in av
 * @param av Table of options (a C array)
 * @param _needle Option name to found
 */
bool		KConfig::isopt(int ac, char **av, const string &_needle)
{
	int 	i;

	if (ac > 0 && av && _needle.length() >= 1)
	{
		for (i = 0; i < ac; ++i)
		{
			if (_needle == av[i])
				return(true);
		}
	}
	return(false);
}