#pragma once

#include "../../Model/IAgent.h"
#include "../../System/Services/ApplicationDataService.h"

namespace spdlog
{
	class logger;
}

namespace desktop { namespace core { namespace agent {
	class LogAgent : public model::IAgent
	{
	public:
		LogAgent(std::unique_ptr<service::ApplicationDataService> applicationService = std::make_unique<service::ApplicationDataService>());
		~LogAgent() = default;
	private:
		std::unique_ptr<service::ApplicationDataService>	m_applicationService;
		std::shared_ptr<spdlog::logger>						m_logger;
	};
}}}