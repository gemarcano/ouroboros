namespace ouroboros
{
	template <typename F>
	bool function_manager::register_function(
		const std::string& aFunctionName, F aResponse)
	{
		mFunctionCallbacks[aFunctionName].emplace_back(aResponse);
		
		return true;
	}
}