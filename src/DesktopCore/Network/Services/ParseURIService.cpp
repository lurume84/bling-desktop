#include "ParseURIService.h"

#include <boost/regex.hpp>

namespace desktop { namespace core { namespace service {

	ParseURIService::ParseURIService()
	{
	
	}

	ParseURIService::~ParseURIService() = default;

	bool ParseURIService::parse(const std::string& uri, std::string& protocol, std::string& domain, std::string& port, std::string& path, std::string& query, std::string& fragment) const
	{
		boost::regex ex("(http|https)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)");
		boost::cmatch what;

		if (regex_match(uri.c_str(), what, ex))
		{
			protocol = std::string(what[1].first, what[1].second);
			domain = std::string(what[2].first, what[2].second);
			port = std::string(what[3].first, what[3].second);
			path = std::string(what[4].first, what[4].second);
			query = std::string(what[5].first, what[5].second);
			fragment = std::string(what[6].first, what[6].second);

			if (port == "")
			{
				if (protocol == "https")
				{
					port = "443";
				}
				else
				{
					port = "80";
				}
			}

			return true;
		}
		else
		{
			return false;
		}
	}
}}}