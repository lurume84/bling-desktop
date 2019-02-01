#pragma once

#include "BlingCore\Utils\Patterns\PublisherSubscriber\Event.h"

#include <string>

namespace bling { namespace ui { namespace events {
	
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
}}}