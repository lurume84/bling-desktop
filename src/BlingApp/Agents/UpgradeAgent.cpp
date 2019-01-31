#include "stdafx.h"

#include "UpgradeAgent.h"

#include "BlingCore\Upgrade\Events.h"

#include "Toast\ToastEventHandler.h"
#include "Toast\ToastCommandLineInfo.h"
#include "Toast\ToastNotificationActivationCallback.h"
#include "Toast\ToastFactory.h"

#include <boost\filesystem\operations.hpp>

namespace bling { namespace ui {  namespace agent {

	namespace
	{
		std::wstring utf8toUtf16(const std::string & str)
		{
			if (str.empty())
				return std::wstring();

			size_t charsNeeded = ::MultiByteToWideChar(CP_UTF8, 0,
				str.data(), (int)str.size(), NULL, 0);
			if (charsNeeded == 0)
				throw std::runtime_error("Failed converting UTF-8 string to UTF-16");

			std::vector<wchar_t> buffer(charsNeeded);
			int charsConverted = ::MultiByteToWideChar(CP_UTF8, 0,
				str.data(), (int)str.size(), &buffer[0], buffer.size());
			if (charsConverted == 0)
				throw std::runtime_error("Failed converting UTF-8 string to UTF-16");

			return std::wstring(&buffer[0], charsConverted);
		}
	}

	UpgradeAgent::UpgradeAgent()
	{
		m_subscriber.subscribe([this](const bling::core::utils::patterns::Event& rawEvt)
		{
			auto evt = static_cast<const bling::core::events::DownloadUpgradeEvent&>(rawEvt);

			auto version = utf8toUtf16(evt.m_version);
			auto path = utf8toUtf16(boost::filesystem::canonical("Html/loading/img/logo_download.png").string());

			auto notification = std::make_unique<core::model::Notification>([]() {return true; }, []() {return true; }, []() {return true; });

			toast::ToastEventHandler* handler = new toast::ToastEventHandler(std::move(notification));

			toast::ToastFactory factory;
			auto toast = factory.getBasic(L"Version " + version + L" available", path);

			

		}, bling::core::events::DOWNLOAD_UPGRADE_EVENT);

		/*m_subscriber.subscribe([this](const bling::core::utils::patterns::Event& rawEvt)
		{
			auto evt = static_cast<const bling::core::events::UpgradeCompletedEvent&>(rawEvt);

			auto version = utf8toUtf16(evt.m_version);

			CefRefPtr<CefBrowser> mainBrowser;

			for (auto &browser : browser_list_)
			{
				mainBrowser = browser;
			}

			ToastEventHandler* handler = new ToastEventHandler([&mainBrowser]()
			{
				mainBrowser->GetMainFrame()->LoadURL(boost::filesystem::canonical("Html/viewer/index.html").string());
				return true;
			}, []() {return true; }, []() {return true; });

			bling::app::Toast toast(handler);
			toast.DisplayToast(L"Version " + version, L"Succesfully installed new version, click to refresh", boost::filesystem::canonical("Html/loading/img/logo_upgrade.png").c_str());

		}, bling::core::events::UPGRADE_COMPLETED_EVENT);*/
	}

	UpgradeAgent::~UpgradeAgent()
	{
		
	}
}}}