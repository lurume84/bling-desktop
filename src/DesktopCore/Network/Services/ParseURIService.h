#pragma once

#include <string>
#include <memory>

namespace desktop { namespace core { namespace service {

	class ParseURIService
	{
	public:
		ParseURIService();
		~ParseURIService();
		bool parse(const std::string& uri, std::string& protocol, std::string& domain, std::string& port, std::string& path, std::string& query, std::string& fragment) const;
	};
}}}