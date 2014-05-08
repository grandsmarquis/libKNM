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

#include "KRegistry.hh"

using namespace KNM;

KRegistry::KRegistry()
{
    root.rename("/");
    root.insert("/", "__KRegistry", "default");
}

KRegistry::KRegistry(const string &_file)
{
    setFile(_file);
}

KRegistry::~KRegistry()
{
    if (file.is_open())
        file.close();
}

void        KRegistry::setFile(const string &_file)
{
    if (file.is_open())
        file.close();

    file.open(_file.c_str());
    if (!file.is_open())
        throw new KError("KRegistry::KRegistry()", "Cannot open registry file: " + _file);
}

void        KRegistry::closeFile()
{
    if (file.is_open())
        file.close();
}

void        KRegistry::load(__attribute__((unused))const string &_file)
{
    return;
}

void        KRegistry::insert(const string &_key)
{
    insert(_key, "");
}

void        KRegistry::insert(const string &_key, const string &_data)
{
    char    *tmp1;
    char    *tmp2;
    string  path;
    string  key;

    tmp1  = strdup(_key.c_str());
    tmp2  = strdup(_key.c_str());

    path = dirname(tmp1);
    key  = basename(tmp2);
    if (path[0] == '.') {path[0] = '/';}

    //cout << path << " - " << key << endl;
    root.insert(path, key, _data);
    sync(_key, _data);
}

void        KRegistry::mknode(const string &_path)
{
    char    *tmp1;
    char    *tmp2;
    string  node_path;
    string  node_name;

    tmp1  = strdup(_path.c_str());
    tmp2  = strdup(_path.c_str());

    node_path = dirname(tmp1);
    node_name  = basename(tmp2);
    if (node_path[0] == '.') {node_path[0] = '/';}

    //cout << path << " - " << key << endl;
    root.mknode(node_path, node_name);
}

void        KRegistry::update(const string &_key, const string &_data)
{
    insert(_key, _data);
}

string      KRegistry::access(const string &_key)
{
    char    *tmp1;
    char    *tmp2;

    string  path;
    string  key;
    string  data;

    tmp1  = strdup(_key.c_str());
    tmp2  = strdup(_key.c_str());

    path = dirname(tmp1);
    key  = basename(tmp2);
    if (path[0] == '.') {path[0] = '/';}


    //cout << path << " - " << key << endl;
    data = root.find(path, key);
    return(data);
}

void        KRegistry::sync(const string &_key, const string &_data)
{
    if (file.is_open())
        file << _key << " " << _data << endl;
}