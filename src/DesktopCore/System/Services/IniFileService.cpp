#include "IniFileService.h"

namespace desktop { namespace core { namespace service {

	IniFileService::IniFileService(std::unique_ptr<service::FileIOService> fileIOService)
	: m_fileIOService(std::move(fileIOService))
	{
		
	}

	IniFileService::~IniFileService() = default;
}}}