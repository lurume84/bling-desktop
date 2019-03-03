#pragma once

#include <string>
#include <map>
#include <memory>

namespace desktop { namespace core { namespace service {

	class IDownloadFileService
	{
	public:
		virtual ~IDownloadFileService() = default;
		virtual std::string download(const std::string& host, const std::string& url, std::map<std::string, std::string> requestHeaders, const std::string &folder) const = 0;
	};
}}}