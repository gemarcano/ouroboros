namespace ouroboros
{
	template <typename F>
	std::string ouroboros_server::register_callback(
		const std::string& aGroup,
		const std::string& aField,
		F&& aCallback)
	{
		std::string result = mCallbackManager.register_callback(normalize_group(aGroup), aField, aCallback);
		return result;
	}

	template <typename F>
	bool ouroboros_server::register_function(const std::string& aFunctionName, F&& aResponse)
	{
		return mFunctionManager.register_function(aFunctionName, aResponse);
	}
}