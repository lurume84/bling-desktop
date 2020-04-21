#include "LogAgent.h"

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

namespace desktop { namespace core { namespace agent {

	LogAgent::LogAgent(std::unique_ptr<service::ApplicationDataService> applicationService)
	: m_applicationService(std::move(applicationService))
	{
		auto documents = m_applicationService->getMyDocuments();

		spdlog::init_thread_pool(8192, 1);
		auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt >();
		auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(documents + "Bling.log", 1024 * 1024 * 10, 1, true);
		std::vector<spdlog::sink_ptr> sinks{ stdout_sink, rotating_sink };

		m_logger = std::make_shared<spdlog::async_logger>("loggername", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);

		spdlog::register_logger(m_logger);
	}
}}}