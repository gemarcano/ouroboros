#include <ouroboros/plugin_manager.h>
#include <ouroboros/plugin.h>

#include <dlfcn.h>
#include <dirent.h>

#include <iostream>
#include <set>
#include <climits>
#include <cstdlib>

namespace ouroboros
{

	class plugin
	{
	public:
		plugin(ouroboros_server& aServer, void *aLib);
		bool is_valid() const;
		bool load();
		void* unload();
		
		plugin(const plugin&) = delete;
		plugin(plugin&&) = default;
	private:
		void *mLib;
		ouroboros_server& mServer;
		plugin_entry_function mEntry;
		plugin_exit_function mExit;
	};
	
	plugin::plugin(ouroboros_server& aServer, void* aLib)
	:mLib(aLib), mServer(aServer), mEntry(nullptr), mExit(nullptr)
	{
		//HACK:
		//This is a hackish way to ward off undefined behavior caused by dlsym.
		//In short, POSIX requires conversions between object pointers and
		//function pointers to be possible, while the C and C++ ISO standards
		//do not. As a result, a simple cast is insuficcient. On the plus side,
		//newer POSIX standard require that this conversion be supported in order
		//for a system to be POSIX compliant, so any systems that can use dlsym
		//support this conversion.
		char *err;
		*(void **) (&mEntry) = dlsym(aLib, "plugin_entry");
		if ((err = dlerror()))
		{
			mEntry = nullptr;
		}
		
		//HACK:
		//This is a hackish way to ward off undefined behavior caused by dlsym.
		//In short, POSIX requires conversions between object pointers and
		//function pointers to be possible, while the C and C++ ISO standards
		//do not. As a result, a simple cast is insuficcient. On the plus side,
		//newer POSIX standard require that this conversion be supported in order
		//for a system to be POSIX compliant, so any systems that can use dlsym
		//support this conversion.
		*(void **) (&mExit) = dlsym(aLib, "plugin_exit");
		if ((err = dlerror()))
		{
			mExit = nullptr;
		}
	}
	
	bool plugin::is_valid() const
	{
		return mEntry && mExit;
	}
	
	bool plugin::load()
	{
		if (is_valid())
		{
			return mEntry(mServer);
		}
		return false;
	}
	
	void* plugin::unload()
	{
		if (is_valid())
		{
			mExit(mServer);
			return mLib;
		}
		return nullptr;
	}


	plugin_manager::plugin_manager(ouroboros_server& aServer)
	:mServer(aServer)
	{}

	plugin_manager::~plugin_manager()
	{
		for (auto& pair : mPlugins)
		{
			unload(pair.first);
		}
	}

	bool plugin_manager::unload(const std::string& aPath)
	{
		if (mPlugins.count(aPath))
		{
			void *lib = mPlugins.at(aPath).unload();
			dlclose(lib);
			return true;
		}
		return false;
	}

	bool plugin_manager::load(const std::string& aPath)
	{//TODO Check that both functions are available
		typedef void* library_t;
		std::string fullPath(aPath);

		library_t lib = dlopen(fullPath.c_str(), RTLD_NOW);

		const char *err = dlerror();
		if (err)
		{
			return false;
		}

		plugin pl(mServer, lib);
		
		if (!pl.is_valid())
		{
			return false;
		}

		mPlugins.emplace(fullPath, std::move(pl));
		return pl.load();
	}

	static std::set<std::string> list_files(const std::string& aDir)
	{
		std::set<std::string> results;
		char abs_path[PATH_MAX];

		realpath(aDir.c_str(), abs_path);
		std::string directory(abs_path);
		directory += '/';

		DIR *d = opendir(abs_path);

		if (d)
		{
			dirent *dir;
			while ((dir = readdir(d)) != NULL)
			{
				if (realpath((directory + dir->d_name).c_str(), abs_path))
				{
					results.insert(abs_path);
				}
			}
			closedir(d);
		}
		return results;
	}

	//TODO document the fact that loading will only load one plugin per absolute
	//Path, so symlinks to the same place will not be counted twice

	std::size_t plugin_manager::load_directory(const std::string& aDirectory)
	{
		std::size_t number_loaded = 0;
		std::set<std::string> files(list_files(aDirectory));

		if (!files.empty())
		{
			for (const std::string& file : files)
			{
				number_loaded += load(file);
			}
		}
		return number_loaded;
	}
}

