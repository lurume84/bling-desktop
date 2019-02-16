#include "stdafx.h"

#include "DownloadFileService.h"

#include "Agents/NotificationAgent.h"
#include "Events.h"
#include "BlingApp.h"

#include "BlingCore\Upgrade\Events.h"
#include "Toast\ToastEventHandler.h"
#include "Toast\ToastCommandLineInfo.h"
#include "Toast\ToastFactory.h"

#include "BlingCore\Utils\Patterns\PublisherSubscriber\Broker.h"

#include <boost\filesystem\operations.hpp>

namespace bling { namespace ui { namespace service {

	DownloadFileService::DownloadFileService(CefRefPtr<CefBrowser> browser, std::unique_ptr<core::service::EncodeStringService> encodeService)
	: m_browser(browser)
	, m_encodeService(std::move(encodeService))
	{
		m_subscriber.subscribe([this](const bling::core::utils::patterns::Event& rawEvt)
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
			auto path = m_encodeService->utf8toUtf16(boost::filesystem::canonical("Html/loading/img/logo_download.png").string());
			
			auto callback = evt.m_callback;

			auto notification = std::make_unique<core::model::Notification>([callback]() 
			{
				if (theApp.m_toastAction == L"accept")
				{
					return callback();
				}
				else
				{
					return true;
				}
			}, []() 
			{
				return true; 
			}, []() 
			{
				return true; 
			});

			m_handler = std::make_shared<toast::ToastEventHandler>(std::move(notification));

			toast::ToastFactory factory;
			m_toast = factory.getYesNo(L"Upgrade", L"Version " + version + L" available", L"Download");

			agent::NotificationAgent::ShowNotificationEvent notificationEvt(m_toast, m_handler);
			core::utils::patterns::Broker::get().publish(notificationEvt);

		}, core::events::DOWNLOAD_UPGRADE_EVENT);
	}

	DownloadFileService::~DownloadFileService() = default;

	std::string DownloadFileService::download(const std::string& host, const std::string& url, const std::string &/*folder*/) const
	{
		auto pos = url.find(host) + host.size();

		std::string script = "window.location = 'https://" + host + url.substr(pos) + "';";

		m_browser->GetMainFrame()->ExecuteJavaScript(script, m_browser->GetMainFrame()->GetURL(), 0);

		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait(lock);

		return m_path;
	}
}}}