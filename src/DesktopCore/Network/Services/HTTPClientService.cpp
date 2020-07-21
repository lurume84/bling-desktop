#include "HTTPClientService.h"

#include "System/Services/LogService.h"

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <cstdlib>

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <httplib.h>

namespace desktop { namespace core { namespace service {

	HTTPClientService::HTTPClientService() = default;
	HTTPClientService::~HTTPClientService() = default;

	bool HTTPClientService::get(const std::string& server, const std::string& port, const std::string& path,
		const std::map<std::string, std::string>& requestHeaders,
		std::map<std::string, std::string>& responseHeaders,
		std::string& content, unsigned int& status_code)
	{
		return send(server, port, "GET", path, requestHeaders, responseHeaders, content, status_code);
	}

	bool HTTPClientService::post(const std::string& server, const std::string& port, const std::string& path,
		const std::map<std::string, std::string>& requestHeaders,
		std::map<std::string, std::string>& responseHeaders,
		std::string& content, unsigned int& status_code)
	{
		return send(server, port, "POST", path, requestHeaders, responseHeaders, content, status_code);
	}

	bool HTTPClientService::send(const std::string& server, const std::string& port, const std::string& action, 
								const std::string& path, const std::map<std::string, std::string>& requestHeaders,
								std::map<std::string, std::string>& responseHeaders,
								std::string& content, unsigned int& status_code)
	{
		httplib::Headers headers;

		for (auto &header : requestHeaders)
		{
			headers.insert(header);
		}

		std::shared_ptr<httplib::Response> res;

		httplib::SSLClient cli(server, std::stoi(port), "", "");

		if (action == "GET")
		{
			res = cli.Get(path.c_str(), headers);
		}
		else if (action == "POST")
		{
			res = cli.Post(path.c_str(), headers, "", "");
		}
		else
		{
			service::LogService::info("HTTP {} not supported", action);
			return false;
		}

		if (res)
		{
			status_code = static_cast<unsigned int>(res->status);

			for (auto &header : res->headers)
			{
				responseHeaders.insert(header);
			}

			content = res->body;

			if (res->status != 200)
			{
				service::LogService::info("HTTP {} {} {}:{}{}", action, status_code, server, port, path);

				return false;
			}
		}

		return true;
	}
}}}