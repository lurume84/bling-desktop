#pragma once

#include <string>

namespace desktop { namespace core { namespace service {
	class FolderOperationService
	{
	public:
		FolderOperationService();
		~FolderOperationService();

		bool copy(const std::string& input, const std::string& output) const;
		bool replace(const std::string& input, const std::string& output) const;
	};
}}}