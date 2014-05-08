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

#ifndef REGISTRY_NODE_HH
#define REGISTRY_NODE_HH

#include <iostream>

#include <string>
#include <list>
#include "leaf.hpp"
#include <KError.hh>

using namespace std;

namespace registry
{
	/**
	 * A node designed to be used in a registry tree.
	 */
	template <class T>
	class 		node
	{
	public:
		node(): name("/"){}
		node(const string &_name): name(_name){};

		/**
		 * Return the node's name.
		 */
		const string 	&getName()
		{
			//...
			return this->name;
		}

		/**
		 * Rename the node.
		 *
		 * @param _name Node new name.
		 */
		void 			rename(const string &_name)
		{
			//...
			this->name = _name;
		}

		/**
		 * Insert a node.
		 * Path is created if necessary.
		 *
		 * @param _path New node path.
		 * @param _name New node name.
		 */
		void 			mknode(const string &_path, const string &_name)
		{
			//...
			travel(_path + _name);
		}

		/**
		 * Insert a leaf.
		 * Path is created if necessary; If the leaf already exists, nothing is performed.
		 *
		 * @param _path New leaf path.
		 * @param _name New leaf name.
		 */
		void 			insert(const string &_path, const string &_name)
		{
			node<T> 	*root;
			leaf<T> 	*key;

			root = travel(_path);
			key  = root->getLeaf(_name);
			if (!key)
				root->leafs.push_back(new leaf<T>(_name));
		}

		/**
		 * Insert a leaf with a data.
		 * Path is created if necessay; If the leaf already exists, data is updated.
		 *
		 * @param _path New leaf path.
		 * @param _name New leaf name.
		 * @param _data New leaf data.
		 */
		void 			insert(const string &_path, const string &_name, const T &_data)
		{
			node<T> 	*root;
			leaf<T> 	*key;

			root = travel(_path);
			key  = root->getLeaf(_name);
			if (!key)
				root->leafs.push_back(new leaf<T>(_name, _data));
			else
				key->assign(_data);
		}

		/**
		 * Return the specified leaf data.
		 * Throw a KError is leaf is not found.
		 *
		 * @param _path Leaf path.
		 * @param _name Leaf name.
		 */
		const T 		&find(const string &_path, const string &_name)
		{
			node<T> 	*root;
			leaf<T> 	*lf;

			root = travel(_path);

			//cout << "node::find: searching in node: " << root->getName() << endl;
			lf = root->getLeaf(_name);

			if (!lf)
			{
				//cout << "leaf not found" << endl;
				throw new KNM::KError("registry::node::find()", "leaf not found");
			}
			else
			{
				//cout << "Leaf found in node: " << root->getName() << endl;
				return lf->getData();
			}
		}

		/**
		 * "Travel" the registry, and build needed nodes.
		 *
		 * @param _path Path to follow.
		 */
		node<T> 		*travel(const string &_path)
		{
			size_t      next    = 0;
    		size_t      prev    = 0;
    		node<T> 	*root   = this;
    		node<T>		*parent = NULL;
    		node<T> 	*child  = NULL;

    		if (_path.length() < 1) {return NULL;}

			while ((next = _path.find_first_of("/", prev)) != std::string::npos)
			{
				if ((next - prev != 0))
				{
					if (root)
					{
						parent = root;
						root   = root->getNode(_path.substr(prev, next - prev));
					}
					if (!root)
					{
						child = new node<T>(_path.substr(prev, next - prev));
						parent->nodes.push_back(child);
						root = child;
					}
				}
				prev = next + 1;
			}

			if (prev < _path.size())
		    {
		        if (root)
		        {
		            parent = root;
		            root   = root->getNode(_path.substr(prev, next - prev));
		        }
		        if (!root)
		        {
		            child = new node<T>(_path.substr(prev, next - prev));
		            parent->nodes.push_back(child);
					root = child;
		        }
		    }
			return(root);
		}

		/**
		 * Search for a node in the current path.
		 *
		 * @param _name Node name.
		 */
		node<T> 		*getNode(const string &_name)
		{
			class list< node<T>* >::iterator 	itr;

			for (itr = this->nodes.begin(); itr != this->nodes.end(); ++itr)
				if ((*itr)->name == _name)
					return (*itr);
			return NULL;
		}

		/**
		 * Search for a leaf in the current path.
		 *
		 * @param _name Leaf name.
		 */
		leaf<T> 		*getLeaf(const string &_name)
		{
			class list< leaf<T>* >::iterator 	itr;

			for (itr = this->leafs.begin(); itr != this->leafs.end(); ++itr)
				if ((*itr)->getName() == _name)
					return (*itr);
			return NULL;
		}

	private:
		string 				name;
		list< leaf<T>* > 	leafs;
		list< node<T>* >	nodes;
	};
}

#endif