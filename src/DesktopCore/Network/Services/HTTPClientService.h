#pragma once

#include <string>
#include <map>

namespace desktop { namespace core { namespace service {

	class HTTPClientService
	{
	public:
		HTTPClientService();
		~HTTPClientService();
		bool get(const std::string& server, const std::string& port, const std::string&, 
					const std::map<std::string, std::string>& requestHeaders, 
					std::map<std::string, std::string>& responseHeaders, 
					std::string& content, unsigned int& status_code);
		bool post(const std::string& server, const std::string& port, const std::string&,
			const std::map<std::string, std::string>& requestHeaders,
			std::map<std::string, std::string>& responseHeaders,
			std::string& content, unsigned int& status_code);
	private:
		bool send(const std::string& server, const std::string& port, const std::string& action,
			const std::string& path, const std::map<std::string, std::string>& requestHeaders,
			std::map<std::string, std::string>& responseHeaders,
			std::string& content, unsigned int& status_code);
	};
}}}