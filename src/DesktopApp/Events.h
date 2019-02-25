#pragma once

#include "DesktopCore\Utils\Patterns\PublisherSubscriber\Event.h"

#include <string>

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
}}}