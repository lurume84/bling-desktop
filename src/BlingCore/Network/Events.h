#pragma once

#include "..\Utils\Patterns\PublisherSubscriber\Event.h"

#include "..\Network/Model/Credentials.h"

namespace bling { namespace core { namespace events {
	
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
}}}