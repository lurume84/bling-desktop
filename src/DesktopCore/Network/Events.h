#pragma once

#include "..\Utils\Patterns\PublisherSubscriber\Event.h"

#include "..\Network\Model\Credentials.h"
#include "..\Network\Model\RTP.h"

namespace desktop { namespace core { namespace events {
	
	namespace sup = utils::patterns;
	
	const sup::EventType CREDENTIALS_EVENT = "CREDENTIALS_EVENT";
	struct CredentialsEvent : public sup::Event
	{
		CredentialsEvent(const model::Credentials& credentials)
		: m_credentials(credentials)
		{
			m_name = CREDENTIALS_EVENT;
		}

		model::Credentials m_credentials;
	};

	namespace sup = utils::patterns;

	const sup::EventType LIVEVIEW_EVENT = "LIVEVIEW_EVENT";
	struct LiveViewEvent : public sup::Event
	{
		LiveViewEvent(const model::RTP& rtp)
			: m_rtp(rtp)
		{
			m_name = LIVEVIEW_EVENT;
		}

		model::RTP m_rtp;
	};
}}}