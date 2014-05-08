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

#ifndef KTREE_HPP
#define KTREE_HPP

#include <string>
#include <list>
#include <KError.hh>

using namespace std;

namespace KNM
{
	/**
	 * A tree containing leafs.
	 *
	 * Compilations flags:
	 * - LIBKNM_NO_EXCEPTIONS : Disable exceptions
	 * - KTREE_NO_EXCEPTIONS  : Disable exceptions
	 */
	template <class T>
	class 		KTree
	{
	public:
		/**
		* A leaf nested class.
		* Contains a named data.
		*/
		template <class U>
		class 		leaf
		{
		public:
			leaf(){}
			leaf(const string &_name): name(_name){};
			leaf(const string &_name, const U &_data): name(_name), data(_data){};

			const string 	&getName(){return this->name;}
			const U 		&getData(){return this->data;}
			void 			rename(const string &_name){this->name = _name;}
			void 			assign(const U &_data){this->data = _data;}
			void 			operator=(const U &_data){assign(_data);}

		private:
			string 			name;
			T 				data;
		};

	public:
		/**
		 * Default constructor.
		 */
		KTree(): name("/"), separator('/'){}

		/**
		 * Construct while defining the tree name.
		 *
		 * @param _name Tree name
		 */
		KTree(const string &_name): name(_name), separator('/'){};

		/**
		 * Set the separator between nodes.
		 *
		 * @param _separator Seprator symbol
		 */
		void 			setSeparator(char _separator)
		{
			separator = _separator;
		}

		/**
		 * Sef default leaf value.
		 *
		 * @param _data Default data.
		 */
		void 			setDefault(const T &_data)
		{
			leaf_default.assign(_data);
		}

		/**
		 * Return the tree name.
		 */
		const string 	&getName()
		{
			//...
			return this->name;
		}

		/**
		 * Rename the tree.
		 *
		 * @param _name New tree name.
		 */
		void 			rename(const string &_name)
		{
			if (_name.size() > 0)
				this->name = _name;
		}

		/**
		 * Insert a node.
		 * Path is created if necessary.
		 *
		 * @param _path New node path and name.
		 */
		void 			mknode(const string &_path)
		{
			//...
			travel(_path);
		}

		/**
		 * Insert a leaf.
		 * Path is created if necessary; If the leaf already exists, nothing is performed.
		 *
		 * @param _path New leaf path and name.
		 */
		void 			insert(const string &_path)
		{
			KTree<T> 	*root;
			leaf<T> 	*key;
			size_t 		found;

			found = _path.find_last_of(separator);
			root = travel(_path.substr(0,found));
			key  = root->getLeaf(_path.substr(found+1));
			if (!key)
				root->leafs.push_back(new leaf<T>(_path.substr(found+1)));
		}

		/**
		 * Insert a leaf with a data.
		 * Path is created if necessay; If the leaf already exists, data is updated.
		 *
		 * @param _path New leaf path and name.
		 * @param _data New leaf data.
		 */
		void 			insert(const string &_path, const T &_data)
		{
			KTree<T> 	*root;
			leaf<T> 	*key;
			size_t 		found;

			found = _path.find_last_of(separator);
			root = travel(_path.substr(0,found));
			key  = root->getLeaf(_path.substr(found+1));
			if (!key)
				root->leafs.push_back(new leaf<T>(_path.substr(found+1), _data));
			else
				key->assign(_data);
		}

		/**
		 * Return the specified leaf data.
		 * Throw a KError is leaf is not found.
		 *
		 * @param _path Leaf path and name.
		 */
		const T 		&find(const string &_path)
		{
			KTree<T> 	*root;
			leaf<T> 	*lf;
			size_t 		found;

			found = _path.find_last_of(separator);
			root = travel(_path.substr(0,found));
			lf = root->getLeaf(_path.substr(found+1));
			if (!lf)
				#if !defined KTREE_NO_EXCEPTIONS && !defined LIBKNM_NO_EXCEPTIONS
					throw new KNM::KError("registry::node::find()", "leaf not found");
				#else
					return leaf_default.getData();
				#endif
			else
				return lf->getData();
		}

		/**
		 * "Travel" the registry, and build needed nodes.
		 *
		 * @param _path Path to follow.
		 */
		KTree<T> 		*travel(const string &_path)
		{
			size_t      next    = 0;
    		size_t      prev    = 0;
    		KTree<T> 	*root   = this;
    		KTree<T>	*parent = NULL;
    		KTree<T> 	*child  = NULL;

    		if (_path.length() < 1) {return NULL;}

			while ((next = _path.find_first_of(separator, prev)) != std::string::npos)
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
						child = new KTree<T>(_path.substr(prev, next - prev));
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
		            child = new KTree<T>(_path.substr(prev, next - prev));
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
		KTree<T> 								*getNode(const string &_name)
		{
			class list< KTree<T>* >::iterator 	itr;

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
		leaf<T> 								*getLeaf(const string &_name)
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
		list< KTree<T>* >	nodes;
		char 				separator;
		leaf<T> 			leaf_default;
	};
}

namespace KNM
{
	
}

#endif