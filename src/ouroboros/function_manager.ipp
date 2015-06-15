namespace ouroboros
{
	template <typename F>
	std::string function_manager::register_function(
		const std::string& aFunctionName, F&& aResponse)
	{
		std::string result = aFunctionName;
		result += ":";
		result = detail::generate_random_string(result, rand_string, 1);
		
		while (mIdToName.count(result))
		{
			result = detail::generate_random_string(result, rand_string, 1);
		}

		mIdToName[result] = aFunctionName;
		mNameToFuncs[aFunctionName].emplace_back(result, aResponse);

		return result;
	}
}