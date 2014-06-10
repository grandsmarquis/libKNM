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
	scope = ".";
}

KConfig::KConfig(const KConfig &_ref)
{
	if (&_ref != this)
		*this = _ref;
}

KConfig &KConfig::operator=(const KConfig &_ref)
{
	list<string*>::const_iterator	itr;

	if (&_ref != this)
	{
		reset();
		for (itr = _ref.sources.begin(); itr != _ref.sources.end(); ++itr)
			sources.push_back((*itr));
	}
	return *this;
}

KConfig::KConfig(const string &_source)
{
	if (_source.length() > 0)
		sources.push_back(new string(_source));
	scope = ".";
}

void 	KConfig::reset()
{
	sources.clear();
	scope = ".";
	stream.close();
}

KConfig::~KConfig()
{
	stream.close();
}

void 	KConfig::load()
{
	for (src_itr = sources.begin(); src_itr != sources.end(); ++src_itr)
	{
		openSource(*(src_itr));
		loadSource();
	}
}

void 	KConfig::load(const string &_source)
{

}

void 	KConfig::load(int ac, char **av)
{

}

void 	KConfig::include(const string &_source)
{

}

void 	KConfig::include(int ac, char **av)
{

}

bool 	KConfig::openSource(string *_source)
{
	if (stream.is_open())
		stream.close();

	stream.open(_source->c_str());
	if (!stream.is_open())
		return(false);
	return(true);
}

void 	KConfig::loadSource()
{
	while (stream){readToken();}
}

void 	KConfig::clearSpaces(string &token)
{
	size_t 	i         = 0;
	int 	escape_sq = 0;
	int 	escape_dq = 0;

	// Clear initial spaces
	while ((token[i] == ' ' || token[i] == '\t'))
		token.erase(i, 1);

	// Clear interspaces
	while (i <= token.size())
	{
		while (token[i] == ' ' && i <= token.size())
		{
			if (token[i+1] == ' ')
				token.erase(i, 1);
			i++;
		}
		i++;
	}

}

bool 	KConfig::readToken()
{
	cout << "---" << endl;

	getline(stream, token);
	cout << "readToken - 1: token: [" << token << "]" << endl;

	clearToken();
	cout << "readToken - 2: token: [" << token << "]" << endl;

	parseToken();

	cout << "---" << endl;

	return true;
}

void 	KConfig::clearToken()
{
	clearSpaces(token);
}

bool 	KConfig::parseToken()
{
	if (handle_comment()){return true;}
	if (handle_command()){return true;}
	if (handle_assignation()){return true;}
	if (handle_scope()){return true;}
}

bool 	KConfig::handle_comment()
{
	size_t 	i = 0;

	if (token[i] == '#')
	{
		cout << "handle_comment: token [" << token << "] is a comment" << endl;
		return true;
	}
}

bool	KConfig::handle_command()
{
	size_t 	i = 0;
	size_t 	end   = 0;
	string 	cmd;
	string 	arg;

	if (token[i] == '@')
	{
		end = token.find_first_of(' ');
		cmd = token.substr(1, end);
		arg = token.substr(end, token.length());
		cout << "handle_command: command : [" << cmd << "]" << endl;
		cout << "handle_command: argument: [" << arg << "]" << endl;
		return true;
	}
}

bool 	KConfig::handle_assignation()
{

}

bool 	KConfig::handle_scope()
{

}