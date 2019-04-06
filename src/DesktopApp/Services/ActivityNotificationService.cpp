#include "stdafx.h"

#include "ActivityNotificationService.h"

#include "Events.h"
#include "DesktopApp.h"

#include "DesktopCore\Upgrade\Events.h"
#include "Toast\ToastEventHandler.h"
#include "Toast\ToastCommandLineInfo.h"
#include "Toast\ToastFactory.h"

#include "DesktopCore\Utils\Patterns\PublisherSubscriber\Broker.h"

#include <boost\filesystem\operations.hpp>

namespace desktop { namespace ui { namespace service {

	ActivityNotificationService::ActivityNotificationService(CefRefPtr<CefBrowser> browser)
	: m_browser(browser)
	{
		
	}

	ActivityNotificationService::~ActivityNotificationService() = default;

	void ActivityNotificationService::notify()
	{
		auto notification = std::make_unique<core::model::Notification>([]() {return true; }, []() {return true; }, []() {return true; });

		toast::ToastFactory factory;

		m_handler = std::make_shared<toast::ToastEventHandler>(std::move(notification));
		m_toast = factory.getBasic(L"Activity", L"Movement in your home");
	}
}}}