#include "stdafx.h"

#include "UpgradeAgent.h"

#include "BlingCore\Upgrade\Events.h"

#include "Toast\ToastEventHandler.h"
#include "Toast\ToastCommandLineInfo.h"
#include "Toast\ToastFactory.h"

#include <boost\filesystem\operations.hpp>

namespace bling { namespace ui {  namespace agent {

	UpgradeAgent::UpgradeAgent()
	{
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