#pragma once

#include <string>

#include <boost/filesystem.hpp>

namespace desktop { namespace core { namespace service {
	class FileIOService
	{
	public:
		FileIOService();
		~FileIOService();

		bool load(const boost::filesystem::path& input, std::stringstream& content) const;
		bool save(const boost::filesystem::path& output, const std::string& content) const;
	};
}}}