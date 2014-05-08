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

#include <string>
#include <KDylib.hh>
#include <KError.hh>

using namespace std;

namespace KNM
{
	/**
	 * KModule handle a C++ module.
	 * It can generate module instances and configure them.
	 */
	template <class T>
	class 	KModule
	{
	public:
		/**
		 * Default constructor.
		 */
		KModule()
		{
			builder   = NULL;
			destroyer = NULL;
		}

		/**
		 * Construct with a libray name.
		 *
		 * @param @param _path Module path/name
		 */
		KModule(const string &_path)
		{
			dylib.setPath(_path);
			builder   = NULL;
			destroyer = NULL;
		}

		/**
		 * Construct with a library and functions names.
		 *
		 * @param _path Module path/name
		 * @param _build Module constructor function name
		 * @param _destroy Module destructor function name
		 */
		KModule(const string &_path, const string &_build, const string &_destroy)
		:builder_name(_build), destroyer_name(_destroy)
		{
			dylib.setPath(_path);
			builder   = NULL;
			destroyer = NULL;
		}

		/**
		 * Set module path / name.
		 *
		 * @param _path Module path/name
		 */
		void 	setModule(const string &_path)
		{
			dylib.setPath(_path);
		}

		/**
		 * Set module builder (aka. constructor) function name.
		 *
		 * @param _build Function name
		 */
		void 	setBuilder(const string &_build)
		{
			builder_name = _build;
		}

		/**
		 * Set module destoyer (aka. destructor) function name.
		 *
		 * @param _destroy Function name
		 */
		void 	setDestroyer(const string &_destroy)
		{
			destroyer_name = _destroy;
		}

		/**
		 * Initialize libray and functions pointer.
		 */
		void 	init()
		{
			if (!builder)
			{
				builder   = (T* (*)())dylib.symbol(builder_name);
				if (!builder)
					throw new KError("KModule::init", "unable to find building function");
			}

			if (!destroyer)
			{
				destroyer = (T* (*)())dylib.symbol(destroyer_name);
				if (!destroyer)
					throw new KError("KModule::init", "unable to find destroying function");
			}
		}

		/**
		 * Build and return a new module.
		 */
		T 		*create()
		{
			T 	*module;

			init();
			module = builder();
			return module;
		}

		/**
		 * Destroy a module.
		 *
		 * @param _module Module instance pointer
		 */
		void 	destroy(T *_module)
		{
			if (_module)
			{
				init();
				destroyer(_module);
			}
		}

	private:
		KDylib 		dylib;				/**< KDylib (dynamic library manager) instance. */
		string 		builder_name;		/**< Builder (aka. constructor) name. */
		string 		destroyer_name;		/**< Destroyer (aka. destructor) name. */
		T 			*(*builder)();		/**< Builder function pointer. */
		T 			*(*destroyer)();	/**< Destroyer function pointer. */
	};
}