#include "LogAgent.h"

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

#include <boost/filesystem.hpp>

namespace desktop { namespace core { namespace agent {

	LogAgent::LogAgent(std::unique_ptr<service::ApplicationDataService> applicationService, std::unique_ptr<core::service::IniFileService> iniFileService)
	: m_applicationService(std::move(applicationService))
	, m_iniFileService(std::move(iniFileService))
	{
		auto documents = m_applicationService->getMyDocuments();

		auto logFiles = m_iniFileService->get<bool>(documents + "Bling.ini", "Log", "Enabled", true);

		if (logFiles)
		{
			boost::filesystem::create_directories(documents + "Logs");

			spdlog::init_thread_pool(8192, 1);
			auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt >();
			auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(documents + "Logs\\Bling.log", 1024 * 1024 * 10, 1, true);
			std::vector<spdlog::sink_ptr> sinks{ stdout_sink, rotating_sink };

			m_logger = std::make_shared<spdlog::async_logger>("BlingLog", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);

			spdlog::register_logger(m_logger);
			spdlog::flush_every(std::chrono::seconds(5));
		}
	}
}}}