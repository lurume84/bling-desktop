#include "stdafx.h"

#include "DownloadViewerService.h"

#include "Events.h"
#include "DesktopApp.h"

#include "DesktopCore\Upgrade\Events.h"

#include "DesktopCore\Utils\Patterns\PublisherSubscriber\Broker.h"

#include <boost\filesystem\operations.hpp>

namespace desktop { namespace ui { namespace service {

	DownloadViewerService::DownloadViewerService(CefRefPtr<CefBrowser> browser, std::unique_ptr<core::service::EncodeStringService> encodeService, 
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

		m_subscriber.subscribe([this](const core::utils::patterns::Event& rawEvt)
		{
			auto evt = static_cast<const core::events::DownloadUpgradeEvent&>(rawEvt);

			auto version = m_encodeService->utf8toUtf16(evt.m_version);
			
			if (boost::filesystem::exists(m_applicationService->getViewerFolder() + "/index.html"))
			{
				
			}
			else
			{
				evt.m_callback();
			}
		}, core::events::DOWNLOAD_UPGRADE_EVENT);

		m_subscriber.subscribe([this](const desktop::core::utils::patterns::Event& rawEvt)
		{
			m_browser->GetMainFrame()->LoadURL(boost::filesystem::canonical(m_applicationService->getViewerFolder() + "/index.html").string());
		}, desktop::core::events::UPGRADE_VIEWER_COMPLETED_EVENT);
	}

	DownloadViewerService::~DownloadViewerService() = default;

	std::string DownloadViewerService::download(const std::string& host, const std::string& url, std::map<std::string, std::string> requestHeaders, const std::string &/*folder*/) const
	{
		auto pos = url.find(host) + host.size();

		std::string script = "window.location = 'https://" + host + url.substr(pos) + "';";

		m_browser->GetMainFrame()->ExecuteJavaScript(script, m_browser->GetMainFrame()->GetURL(), 0);

		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait(lock);

		return m_path;
	}
}}}