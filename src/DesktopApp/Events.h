#pragma once

#include "DesktopCore\Utils\Patterns\PublisherSubscriber\Event.h"

#include <string>

class CefBrowser;

namespace desktop { namespace ui { namespace events {
	
	namespace sup = core::utils::patterns;
	
	const sup::EventType DOWNLOAD_STATUS_EVENT = "DOWNLOAD_STATUS_EVENT";
	struct DownloadStatusEvent : public sup::Event
	{
		DownloadStatusEvent()
		{
			m_name = DOWNLOAD_STATUS_EVENT;
		}

		bool bIsValid;
		bool bIsInProgress;
		bool bIsComplete;
		bool bIsCanceled;
		int nProgress;
		unsigned long long nSpeed;
		unsigned long long nReceived;
		unsigned long long nTotal;
		std::string m_path;
	};

	const sup::EventType BROWSER_LOAD_END_EVENT = "BROWSER_LOAD_END_EVENT";
	struct BrowserLoadEndEvent : public sup::Event
	{
		BrowserLoadEndEvent()
		{
			m_name = BROWSER_LOAD_END_EVENT;
		}
	};

	const sup::EventType BROWSER_CREATED_EVENT = "BROWSER_CREATED_EVENT";
	struct BrowserCreatedEvent : public sup::Event
	{
		BrowserCreatedEvent(CefBrowser& browser)
			:m_browser(browser)
		{
			m_name = BROWSER_CREATED_EVENT;
		}

		CefBrowser& m_browser;
	};

	const sup::EventType TOASTIFY_EVENT = "TOASTIFY_EVENT";
	struct ToastifyEvent : public sup::Event
	{
		ToastifyEvent(long code)
			:m_code(code)
		{
			m_name = TOASTIFY_EVENT;
		}

		long m_code;
	};
}}}