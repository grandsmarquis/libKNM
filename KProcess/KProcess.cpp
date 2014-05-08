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

#include "KProcess.hh"

using namespace std;
using namespace KNM;

KProcess::KProcess()
{
	this->args = NULL;
	this->env = NULL;
}

/**
 * @param _name Process name (command to execute)
 */
KProcess::KProcess(const string &_name)
{
	this->name = _name;
	this->args = NULL;
	this->env = NULL;
}

/**
 * @param _PID Process PID to attach
 */
KProcess::KProcess(int _PID)
{
	this->args = NULL;
	this->env  = NULL;
	this->PID  = _PID;
}

bool				KProcess::start()
{
	unsigned int 	i = 0;

	// Building process options table
	this->options.push_front(new string(this->name));
	this->args = new char*[this->options.size() + 1];
	this->itr_options = this->options.begin();
	for (i = 0; i < this->options.size(); ++i)
	{
		//cout << "Adding option " << (*itr_options)->c_str() << endl;
		this->args[i] = new char[(*itr_options)->length() +1];
		memset(this->args[i], 0, (*itr_options)->length() + 1);
		strncpy(args[i], (*itr_options)->c_str(), (*itr_options)->length());
		this->itr_options++;
	}
	args[i] = NULL;
	//cout << "Options list done" << endl;


	// Calling new process
	this->PID = fork();
	if (this->PID == 0)
	{
		setsid();
		::signal(SIGHUP, SIG_IGN);
		//cout << args[1] << endl;
		//cout << args[2] << endl;
		exit(execve(this->name.c_str(), this->args, this->env));
	}
	else
	{
		// Cleanup memory
		this->options.clear();
		//delete this->args;	// WARNING ! NEED TO DEBUG ASAP (GLIBC STACK DUMP IS RAISED)
		return(true);
	}
	return true;
}

bool 	KProcess::signal(int _signal)
{
	if (this->status() == true)
	{
		kill(this->PID, _signal);
		return(true);
	}
	return false;
}

bool	KProcess::stop()
{
	if (this->status() == true)
	{
		kill(this->PID, SIGSYS);
		if (this->status() == false)
			return(true);
		else
			return(false);
	}
	return(true);
}

bool 	KProcess::pause()
{
	if (this->status() == true)
	{
		kill(this->PID, SIGSTOP);
		return true;
	}
	return(false);
}

bool 	KProcess::resume()
{
	if (this->status() == true)
	{
		kill(this->PID, SIGCONT);
		return true;
	}
	return(false);
}

bool 	KProcess::status()
{
	if (kill(this->PID, 0) == 0)
		return(true);
	return(false);
}

/**
 * @param _pid Process PID to attach
 */
void 	KProcess::attach(int _pid)
{
	this->PID = _pid;
}

/**
 * @param _name Set process name (command)
 */
void 		KProcess::setName(const string &_name)
{
	this->name = _name;
}

/**
 * @param _opt Set a process option (ex: "-l" for a ls process)
 */
void 		KProcess::addOption(const string &_opt)
{
	//cout << "Adding option in process: [" << _opt << "]" << endl;
	this->options.push_back(new string(_opt));
}

/**
 * @param _size Options number
 * @param _opts Options list (C array)
 */
void 		KProcess::getOption(const int _size, char **_opts)
{
	int 	i;

	for (i = 2; i < _size; i++)
		this->options.push_back(new string(_opts[i]));
}

/**
 * @param _env C array
 */
void 		KProcess::getEnv(char **_env)
{
	this->env = _env;
}
