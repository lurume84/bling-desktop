#pragma once

#include "CopyFolderService.h"

#include <string>
#include <memory>

namespace desktop { namespace core { namespace service {
	class ReplaceFolderService
	{
	public:
		ReplaceFolderService(std::unique_ptr<CopyFolderService> copyService = std::make_unique<CopyFolderService>());
		~ReplaceFolderService();

		bool replace(const std::string& input, const std::string& output) const;

	private:
		std::unique_ptr<CopyFolderService> m_copyService;
	};
}}}