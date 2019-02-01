#pragma once

#include <string>

namespace bling { namespace core { namespace service {
	class ReplaceFolderService
	{
	public:
		ReplaceFolderService();
		~ReplaceFolderService();

		bool replace(const std::string& input, const std::string& ouput) const;
	};
}}}