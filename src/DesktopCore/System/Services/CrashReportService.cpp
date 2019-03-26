#include "CrashReportService.h"

#include <boost/filesystem.hpp>

namespace desktop { namespace core { namespace service {

	CrashReportService::CrashReportService(std::unique_ptr<ApplicationDataService> appService)
	: m_appService(std::move(appService))
	{
	
	}

	CrashReportService::~CrashReportService() = default;

	bool CrashReportService::initialize(const std::vector<std::string>& files)
	{
		CR_INSTALL_INFO info;
		memset(&info, 0, sizeof(CR_INSTALL_INFO));
		info.cb = sizeof(CR_INSTALL_INFO);
		info.pszAppName = m_appService->getApplicationName().c_str(); // Define application name.
		info.pszAppVersion = m_appService->getApplicationVersion().c_str();     // Define application version.		
		info.dwFlags = 0;
		info.dwFlags |= CR_INST_ALL_POSSIBLE_HANDLERS; // Install all available exception handlers.    
													   //info.dwFlags |= CR_INST_NO_GUI;               // Don't display GUI.
		info.dwFlags |= CR_INST_DONT_SEND_REPORT;     // Don't send report immediately, just queue for later delivery.
		info.dwFlags |= CR_INST_STORE_ZIP_ARCHIVES;   // Maintain zip archives
		info.pszDebugHelpDLL = NULL;                    // Search for dbghelp.dll using default search sequence.
		info.uMiniDumpType = MiniDumpNormal;            // Define minidump size.
		
		auto folder = m_appService->getMyDocuments() + "CrashReports\\";

		if (!boost::filesystem::exists(folder))
		{
			boost::filesystem::create_directories(folder);
		}

		info.pszErrorReportSaveDir = folder.c_str();

		info.nRestartTimeout = 50;

		m_helper = std::make_unique<CrAutoInstallHelper>(&info);
		if (m_helper->m_nInstallStatus != 0)
		{
			TCHAR buff[256];
			crGetLastErrorMsg(buff, 256);
			MessageBox(NULL, buff, "crInstall error", MB_OK);
			return false;
		}

		bool result = crAddScreenshot(CR_AS_MAIN_WINDOW);

		if(result == 0)
		{
			result = crAddProperty("ReportService", "CrashReportServicev1.0");
		}

		return result;
	}

}}}