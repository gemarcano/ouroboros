#include <data/base.hpp>
#include <data/data_store.hpp>
#include <server/device_tree.tpp>
#include <memory>

namespace ouroboros
{
	template <class field>
	device_tree<field>& device_tree<field>::get_device_tree()
	{
		if (!mpTree)
		{
			mpTree = new device_tree<field>();
		}
		return *mpTree;
	}

	template <class field>
	device_tree<field>::device_tree()
	{
		mpDataStore =
			new data_store<field>(
				new group<field>(detail::build_tree<field>()));
	}

	template <class field>
	device_tree<field>::~device_tree()
	{
		//we need to free all of the resources we've allocated
		delete mpDataStore;
	}

	template <class field>
	data_store<field>& device_tree<field>::get_data_store()
	{
		return *mpDataStore;
	}

	template<class field>
	device_tree<field> *device_tree<field>::mpTree;
}
