#pragma once

#include <string>

namespace desktop { namespace core { namespace service {
	class CopyFolderService
	{
	public:
		CopyFolderService();
		~CopyFolderService();

		bool copy(const std::string& input, const std::string& output) const;
	};
}}}