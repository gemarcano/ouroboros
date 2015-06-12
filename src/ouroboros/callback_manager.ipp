namespace ouroboros
{
	template <typename F>
	std::string callback_manager::register_callback(
		const std::string& aGroup, const std::string& aField, F&& aCallback)
	{
		std::string result;
		var_field *named = mStore.get(aGroup, aField);
		if (named)
		{
			std::string resource = aGroup + '/' + aField;
			result = resource;
			result += ":";
			result = detail::generate_random_string(result, rand_string, 1);

			while (mIdToFields.count(result))
			{
				result = detail::generate_random_string(result, rand_string, 1);
			}

			mIdToFields[result] = resource;
			mFieldToCallbacks[resource].emplace_back(result, std::bind(aCallback, named));
		}

		return result;
	}
}