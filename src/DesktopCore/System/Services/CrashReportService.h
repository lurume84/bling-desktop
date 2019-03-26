#pragma once

#include "ApplicationDataService.h"

#include <memory>
#include <vector>
#include <string>
#include <crashrpt\CrashRpt.h>

namespace desktop { namespace core { namespace service {
	class CrashReportService
	{
	public:
		CrashReportService(std::unique_ptr<ApplicationDataService> appService = std::make_unique<ApplicationDataService>());
		~CrashReportService();

		bool initialize(const std::vector<std::string>& files);
	private:
		std::unique_ptr<CrAutoInstallHelper> m_helper;
		std::unique_ptr<ApplicationDataService> m_appService;
	};
}}}