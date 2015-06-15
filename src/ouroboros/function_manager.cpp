#include <ouroboros/function_manager.h>

#include <vector>
#include <string>
#include <algorithm>

namespace ouroboros
{
	const std::string function_manager::rand_string("0123456789");

	function_manager::function_manager()
	{}

	void function_manager::execute_function(
		const std::string& aFunctionName,
		const std::vector<std::string>& aParameters)
	{
		if (mNameToFuncs.count(aFunctionName))
		{
			for (auto& pair : mNameToFuncs[aFunctionName])
			{
				pair.second(aParameters);
			}
		}
	}
	
	std::string function_manager::unregister_function(const std::string& aID)
	{
		std::string result;
		
		if (mIdToName.count(aID))
		{
			std::string name = result = mIdToName[aID];
			std::vector<std::pair<std::string, function_f>>& vec = mNameToFuncs[name];
			auto cmp = [aID](const std::pair<std::string, function_f>& a){ return aID == a.first; };
			vec.erase(std::find_if(vec.begin(), vec.end(), cmp));
			
			mIdToName.erase(aID);
		}

		return result;
	}
}
