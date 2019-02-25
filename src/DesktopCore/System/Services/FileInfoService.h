#pragma once

#include <string>

#include <boost/filesystem.hpp>

namespace desktop { namespace core { namespace service {
	class FileInfoService
	{
	public:
		FileInfoService();
		~FileInfoService();

		std::string getProductVersion(const std::string& file) const;
	};
}}}