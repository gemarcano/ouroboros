#ifndef _OUROBOROS_CALLBACK_MANAGER_H_
#define _OUROBOROS_CALLBACK_MANAGER_H_

#include <ouroboros/data/base.hpp>
#include <ouroboros/device_tree.hpp>

#include <map>
#include <functional>
#include <string>

namespace ouroboros
{
	/**	This class is meant to become the primary callback manager for the
	 *		system, but right now it only supports "registering" names and
	 *		creating IDs for said names.
	 *
	 */
	class callback_manager
	{
	public:

		typedef std::function<void()> callback_f;

		/**	Constructor.
		 *
		 *	@param [in] aStore Reference to data store with elements to manage.
		 */
		callback_manager(data_store<var_field>& aStore);

		/**	Registers a callback with the given name.
		 *
		 *	@returns The ID assigned to the callback based on the given name.
		 */
		template <typename F>
		std::string register_callback(const std::string& aGroup, 
			const std::string& aField, F aCallback);
		
		/**	Runs all callbacks associated with the specified field.
		 *
		 *	@param [in] aGroup Name of the field's group with callbacks to execute.
		 *	@param [in] aField Name of the field with callbacks to execute.
		 *
		 *	@post If there are callbacks associated with the field aFieldName,
		 *		these will be called, else nothing will happen.
		 */
		void trigger_callbacks(const std::string& aGroup, const std::string& aField);
		
		/**	Unregisters a callback with the given ID.
		 *
		 *	@returns The name of the given string when registered.
		 */
		std::string unregister_callback(const std::string& aID);

	private:
		static const std::string rand_string;
		
		data_store<var_field>& mStore;
		
		std::map<std::string, std::string> mIdToFields;
		std::map<std::string, std::vector<std::pair<std::string, callback_f>>> mFieldToCallbacks;
		
	};
}

#include "callback_manager.ipp"

#endif//_OUROBOROS_CALLBACK_MANAGER_H_
