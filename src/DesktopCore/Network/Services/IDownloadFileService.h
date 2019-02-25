#pragma once

#include <string>
#include <memory>

namespace desktop { namespace core { namespace service {

	class IDownloadFileService
	{
	public:
		virtual ~IDownloadFileService() = default;
		virtual std::string download(const std::string& host, const std::string& url, const std::string &folder) const = 0;
	};
}}}