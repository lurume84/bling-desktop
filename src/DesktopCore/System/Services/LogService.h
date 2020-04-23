#pragma once

#include <string>
#include "spdlog/spdlog.h"

namespace desktop { namespace core { namespace service {
	class LogService
	{
	public:
		LogService();
		~LogService();

		template<typename... Args>
		static inline void info(const std::string& fmt, const Args &... args)
		{
			spdlog::get("loggername")->info(fmt, args...);
		}

		template<typename... Args>
		static inline void error(const std::string& fmt, const Args &... args)
		{
			spdlog::get("loggername")->error(fmt, args...);
		}

		template<typename... Args>
		static inline void warn(const std::string& fmt, const Args &... args)
		{
			spdlog::get("loggername")->warn(fmt, args...);
		}

		template<typename... Args>
		static inline void critical(const std::string& fmt, const Args &... args)
		{
			spdlog::get("loggername")->critical(fmt, args...);
		}
	};
}}}