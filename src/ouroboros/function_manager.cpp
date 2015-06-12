#include <ouroboros/function_manager.h>

#include <vector>
#include <string>

namespace ouroboros
{
	
	function_manager::function_manager()
	{}
	
	void function_manager::execute_function(
		const std::string& aFunctionName,
		const std::vector<std::string>& aParameters)
	{
		if (mFunctionCallbacks.count(aFunctionName))
		{
			for (function_f& func : mFunctionCallbacks[aFunctionName])
			{
				func(aParameters);
			}
		}
	}
}
