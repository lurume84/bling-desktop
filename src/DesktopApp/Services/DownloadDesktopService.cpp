#include "DownloadDesktopService.h"

//#include "Agents/NotificationAgent.h"
#include "Events.h"
//#include "DesktopApp.h"

#include "DesktopCore\Upgrade\Events.h"
#include "DesktopCore\System\Services\LogService.h"

#include "DesktopCore\Utils\Patterns\PublisherSubscriber\Broker.h"

#pragma warning(push)
#pragma warning(disable : 4100)
#pragma warning(disable : 4481)
#include <include/cef_app.h>
#pragma warning(pop)

#include <boost\filesystem\operations.hpp>

#include <locale>
#include <codecvt>
#include <string>
#include <shlobj.h>
#include <shlwapi.h>
#include <objbase.h>
#include <shellapi.h>

namespace desktop { namespace ui { namespace service {

	DownloadDesktopService::DownloadDesktopService(CefBrowser& browser, std::unique_ptr<core::service::EncodeStringService> encodeService,
											std::unique_ptr<core::service::ApplicationDataService> applicationService)
	: m_browser(browser)
	, m_encodeService(std::move(encodeService))
	, m_applicationService(std::move(applicationService))
	{
		m_subscriber.subscribe([this](const desktop::core::utils::patterns::Event& rawEvt)
		{
			auto evt = static_cast<const events::DownloadStatusEvent&>(rawEvt);

			if (evt.bIsComplete)
			{
				m_path = evt.m_path;

				m_cv.notify_one();
			}

		}, events::DOWNLOAD_STATUS_EVENT);

		m_subscriber.subscribe([this, &browser](const desktop::core::utils::patterns::Event& rawEvt)
		{
			auto evt = static_cast<const desktop::core::events::UpgradeDesktopCompletedEvent&>(rawEvt);

			std::stringstream ss;
			ss << "Bling Desktop " << evt.m_version << " is available, do you want to install it?";

			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::wstring message = converter.from_bytes(ss.str());

			const int result = MessageBox(NULL, message.c_str(), L"Bling upgrade", MB_YESNO);

			switch (result)
			{
			case IDYES:
				ShellExecuteA(nullptr, "open", evt.m_path.c_str(), nullptr, nullptr, SW_SHOW);
				break;
			case IDNO:
				break;
			}
		}, desktop::core::events::UPGRADE_DESKTOP_COMPLETED_EVENT);
	}

	DownloadDesktopService::~DownloadDesktopService()
	{
		m_cv.notify_all();
	};

	std::string DownloadDesktopService::download(const std::string& url, std::map<std::string, std::string> requestHeaders, const std::string &folder) const
	{
		std::string script = "window.location = '" + url + "';";

		m_browser.GetMainFrame()->ExecuteJavaScript(script, m_browser.GetMainFrame()->GetURL(), 0);

		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait(lock);

		if(m_path != "")
		{
			boost::system::error_code ec;
			boost::filesystem::rename(m_path, folder, ec);

			if (ec != boost::system::errc::success)
			{
				core::service::LogService::error("Desktop Upgrade: Could not rename Desktop installer from {} to {}", m_path, folder);
			}
			else
			{
				m_path = folder;
			}
		}

		return m_path;
	}

	std::string DownloadDesktopService::download(const std::string& host, const std::string& url, std::map<std::string, std::string> requestHeaders, const std::string &/*folder*/) const
	{
		auto pos = url.find(host) + host.size();

		std::string script = "window.location = 'https://" + host + url.substr(pos) + "';";

		m_browser.GetMainFrame()->ExecuteJavaScript(script, m_browser.GetMainFrame()->GetURL(), 0);

		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait(lock);

		return m_path;
	}

	void DownloadDesktopService::cancel()
	{
		m_cv.notify_all();
	}
}}}