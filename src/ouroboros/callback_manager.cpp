#include <ouroboros/callback_manager.h>
#include <ouroboros/data/misc.h>

#include <cstdlib>
#include <functional>
#include <algorithm>

namespace ouroboros
{
	const std::string callback_manager::rand_string("0123456789");
	
	callback_manager::callback_manager(data_store<var_field>& aStore)
	:mStore(aStore)
	{}

	void callback_manager::trigger_callbacks(
		const std::string& aGroup,
		const std::string& aField)
	{
		std::string resource = aGroup + '/' + aField;
		if (mFieldToCallbacks.count(resource))
		{
			for (auto& pair : mFieldToCallbacks.at(resource))
			{
				pair.second();
			}
		}
	}

	std::string callback_manager::unregister_callback(const std::string& aID)
	{
		std::string result;
		
		if (mIdToFields.count(aID))
		{
			std::string field = result = mIdToFields[aID];
			std::vector<std::pair<std::string, callback_f>>& vec = mFieldToCallbacks[field];
			auto cmp = [aID](const std::pair<std::string, callback_f>& a){ return aID == a.first; };
			vec.erase(std::find_if(vec.begin(), vec.end(), cmp));
			
			mIdToFields.erase(aID);
		}
		


		return result;
	}
}
