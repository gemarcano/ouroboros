#include "slre.h"
#include <string.h>
#include <stdbool.h>
#include "rest.h"
#include <string>
#include <utility>

namespace ouroboros
{
	static const char * full_regex = "^/groups/([a-z0-9-_]+)/field/([a-z0-9-_]+)/?$";
	static const char * group_regex = "^/groups/([a-z0-9-_]+)/?$";

	bool is_REST_URI(const std::string& aURI)
	{
		int result =
			slre_match(full_regex, aURI.c_str(), aURI.length(), NULL, 0, 0);
		int group_result =
			slre_match(group_regex, aURI.c_str(), aURI.length(), NULL, 0, 0);

		return (result >= 0 || group_result >= 0);
	}

	REST_call_type get_REST_call_type(const std::string& aURI)
	{	
		int item_result = slre_match(
			full_regex, aURI.c_str(), aURI.length(), NULL, 0, 0);
		if (item_result >= 0)
			return REST_call_type::NAME;
		
		int group_result = slre_match(
			group_regex, aURI.c_str(), aURI.length(), NULL, 0, 0);
		if (group_result >= 0)
			return REST_call_type::GROUP;
		
		return REST_call_type::NONE;
	}

	HTTP_request_type get_HTTP_request_type(const std::string& aMethodType)
	{
		if (aMethodType == "PUT")
		{
			return HTTP_request_type::PUT;
		}
		else if (aMethodType == "POST")
		{
			return HTTP_request_type::POST;
		}
		else if (aMethodType == "GET")
		{
			return HTTP_request_type::GET;
		}
		else if (aMethodType == "DELETE")
		{
			return HTTP_request_type::DELETE;
		}
		return HTTP_request_type::UNKNOWN;
	}

	std::pair<std::string, std::string> extract_group_name(const std::string& aURI)
	{
		struct slre_cap match[2];
		slre_match(full_regex, aURI.c_str(), aURI.length(), match, 2, 0);

		std::pair<std::string, std::string> result;
		
		//Copy group title from match to remove remaining characters
		std::string groupTitle(match[0].ptr);
		groupTitle.erase(groupTitle.begin()+match[0].len, groupTitle.end());

		result.first.assign(groupTitle);
		result.second.assign(match[1].ptr, match[1].len);
		return result;
	}

	std::string extract_group(const std::string& aURI)
	{
		struct slre_cap match[1];
		slre_match(group_regex, aURI.c_str(), aURI.length(), match, 1, 0);

		std::string result;
		result.assign(match[0].ptr, match[0].len);
		return result;
	}
	

}
