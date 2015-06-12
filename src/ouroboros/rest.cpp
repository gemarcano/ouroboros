#include <ouroboros/rest.h>

#include <regex>

#include <string>
#include <utility>

namespace ouroboros
{
	static const std::string character_set(".");
	static const std::regex full_regex("^/groups/(" + character_set + "+)/fields/(" + character_set + "+?)/?$");
	static const std::regex group_regex("^/groups/(" + character_set + "+?)/?$");
	static const std::regex functions_regex("^/groups/(" + character_set + "+)/functions/(" + character_set + "+?)/?$");
	
	static const std::regex root_field_regex("^/fields/(" + character_set + "+?)/?$");
	static const std::regex root_group_regex("^/groups/?$");
	
	static const std::regex root_functions_regex("^/functions/(" + character_set + "+?)/?$");
	
	static const std::regex full_regex_callback("^/groups/(" + character_set + "+)/fields/(" + character_set + "+?)/callback$");
	static const std::regex root_field_regex_callback("^/fields/(" + character_set + "+?)/callback$");

	/**	Extracts the group from the given REST URI.
	 *
	 *	@param [in] aURI URI containing a REST group.
	 *
	 *	@returns String representation of the groups found in the URI,
	 *		'-' delimited.
	 */
	static std::string extract_group(const std::string& aURI);

	/**	Extracts the group and item name from the given REST URI.
	 *
	 *	@param [in] aURI URI containing a REST group and name.
	 *
	 *	@returns Pair containing a string representation of the groups found in
	 *		the URI (first), '-' delimited, and the string representation of the
	 *		name of the item (second).
	 */
	static std::pair<std::string, std::string> extract_group_name(
		const std::string& aURI);

	bool is_REST_URI(const std::string& aURI)
	{
		return std::regex_match(aURI, full_regex) ||
			std::regex_match(aURI, group_regex) ||
			std::regex_match(aURI, functions_regex) ||
			std::regex_match(aURI, root_field_regex) ||
			std::regex_match(aURI, root_group_regex) ||
			std::regex_match(aURI, root_functions_regex) ||
			std::regex_match(aURI, root_field_regex_callback) ||
			std::regex_match(aURI, full_regex_callback);
	}

	static rest_request_type get_rest_request_type(const std::string& aURI)
	{
		//Order matters, if character_set is too accepting, it could swallow up
		//the characters "/callback", so check the callback regex first before
		//checking the regular fields one
		if (std::regex_match(aURI, full_regex_callback) ||
			std::regex_match(aURI, root_field_regex_callback))
			return CALLBACK;
		
		if (std::regex_match(aURI, full_regex) ||
			std::regex_match(aURI, root_field_regex))
			return FIELDS;

		if (std::regex_match(aURI, group_regex) ||
			std::regex_match(aURI, root_group_regex))
			return GROUPS;

		if (std::regex_match(aURI, functions_regex) ||
			std::regex_match(aURI, root_functions_regex))
			return FUNCTIONS;

		return NONE;
	}

	static http_request_type get_http_request_type(const std::string& aMethodType)
	{
		if (aMethodType == "PUT")
		{
			return PUT;
		}
		else if (aMethodType == "POST")
		{
			return POST;
		}
		else if (aMethodType == "GET")
		{
			return GET;
		}
		else if (aMethodType == "DELETE")
		{
			return DELETE;
		}
		return UNKNOWN;
	}

	std::pair<std::string, std::string> extract_group_name(const std::string& aURI)
	{
		std::pair<std::string, std::string> result;

		//Check if user is accessing field in root first
		std::smatch match;
		if (std::regex_match(aURI, match, root_field_regex) ||
			std::regex_match(aURI, match, root_field_regex_callback))
		{
			result.first = std::string();
			result.second = match[1];
		}
		else
		{
			if (!std::regex_match(aURI, match, full_regex))
			{
				std::regex_match(aURI, match, full_regex_callback);
			}

			std::string groupTitle(match[1]);

			result.first = groupTitle;
			result.second = match[2];
		}

		return result;
	}

	std::pair<std::string, std::string> extract_functions(const std::string& aURI)
	{
		std::pair<std::string, std::string> result;

		//Check if user is accessing functions in root first
		std::smatch match;
		if(std::regex_match(aURI, match, root_functions_regex))
		{
			result.first = std::string();
			result.second = match[1];
		}
		else
		{
			std::regex_match(aURI, match, functions_regex);

			//Copy group title from match to remove remaining characters
			std::string groupTitle(match[1]);
			
			result.first.assign(groupTitle);
			result.second = (match[2]);
		}

		return result;
	}
	
	std::string extract_group(const std::string& aURI)
	{
		std::smatch match;
		std::string result;
		if(std::regex_match(aURI, match, group_regex))
		{
			result = match[1];
		}

		return result;
	}

	rest_request::rest_request(
		mg_connection* apConnection, const std::string& aUri)
	:mHttpType(get_http_request_type(apConnection->request_method)),
		mRestType(get_rest_request_type(aUri)), mpConnection(apConnection)
	{
		switch (mRestType)
		{
			case FIELDS:
			case CALLBACK:
			{
				std::pair<std::string, std::string> data =
					extract_group_name(aUri);
				mGroups = data.first;
				mFields = data.second;
			}
				break;

			case GROUPS:
				mGroups = extract_group(aUri);
				break;

			case FUNCTIONS:
			{
				std::pair<std::string, std::string> data = extract_functions(aUri);
				mGroups = data.first;
				mFunctions = data.second;
			}
				break;

			case NONE:
				break;
		}
	}

	rest_request::~rest_request()
	{}

	http_request_type rest_request::getHttpRequestType() const
	{
		return mHttpType;
	}

	rest_request_type rest_request::getRestRequestType() const
	{
		return mRestType;
	}

	std::string rest_request::getFields() const
	{
		return mFields;
	}

	std::string rest_request::getGroups() const
	{
		return mGroups;
	}

	std::string rest_request::getFunctions() const
	{
		return mFunctions;
	}

	mg_connection *rest_request::getConnection() const
	{
		return mpConnection;
	}
}
