#pragma once

#include <string>

namespace desktop { namespace core { namespace service {
	class ReplaceFolderService
	{
	public:
		ReplaceFolderService();
		~ReplaceFolderService();

		bool replace(const std::string& input, const std::string& ouput) const;
	};
}}}