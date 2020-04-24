#pragma once

#include "../../Model/IAgent.h"
#include "../../System/Services/ApplicationDataService.h"

#include "DesktopCore\System\Services\IniFileService.h"

namespace spdlog
{
	class logger;
}

namespace desktop { namespace core { namespace agent {
	class LogAgent : public model::IAgent
	{
	public:
		LogAgent(std::unique_ptr<service::ApplicationDataService> applicationService = std::make_unique<service::ApplicationDataService>(),
				std::unique_ptr<core::service::IniFileService> iniFileService = std::make_unique<core::service::IniFileService>());
		~LogAgent() = default;
	private:
		std::unique_ptr<service::ApplicationDataService>	m_applicationService;
		std::shared_ptr<spdlog::logger>						m_logger;
		std::unique_ptr<core::service::IniFileService>		m_iniFileService;
	};
}}}