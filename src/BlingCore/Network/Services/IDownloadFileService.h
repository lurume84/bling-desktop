#pragma once

#include <string>
#include <memory>

namespace bling { namespace core { namespace service {

	class IDownloadFileService
	{
	public:
		virtual ~IDownloadFileService() = default;
		virtual std::string download(const std::string& host, const std::string& url) const = 0;
	};
}}}