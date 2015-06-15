#ifndef _OUROBOROS_FUNCTION_MANAGER_H_
#define _OUROBOROS_FUNCTION_MANAGER_H_

#include <ouroboros/data/misc.h>

#include <vector>
#include <string>
#include <map>
#include <functional>

namespace ouroboros
{
	class function_manager
	{
	public:
		function_manager();
		typedef std::function<void(const std::vector<std::string>&)> function_f;
		
		/**	Registers a response function for the specified function call.
		 *
		 *	@param [in] aFunctionName Name of the function to register.
		 *	@param [in] aResponse Callback functor that is called as
		 *		void(std::vector<std::string>) function.
		 *	@returns ID string of the registered function. Empty string if
		 *		failed to register.
		 */
		template<typename F>
		std::string register_function(
			const std::string& aFunctionName, F&& aResponse);
		
		/**	Unregisters a function with the given ID.
		 *
		 *	@returns The name of the given function when registered.
		 */
		std::string unregister_function(const std::string& aID);
		
		/**	Executes a response function for the specified function call.
		 *
		 *	@param [in] aFunctionName Name of the function to call.
		 *	@param [in] aParameters Parameters as strings in a vector.
		 */
		void execute_function(
			const std::string& aFunctionName,
			const std::vector<std::string>& aParameters);

		function_manager(const function_manager&) = delete;

	private:
		static const std::string rand_string;
		
		std::map<std::string, std::string> mIdToName;
		std::map<std::string, std::vector<std::pair<std::string, function_f>>> mNameToFuncs;
	};
}

#include "function_manager.ipp"

#endif/*_OUROBOROS_FUNCTION_MANAGER_H_*/
