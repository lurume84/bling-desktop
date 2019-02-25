#include "Subscriber.h"

#include "Broker.h"

#include <boost/signals2/connection.hpp>

namespace desktop { namespace core { namespace utils { namespace patterns {

	Subscriber::Subscriber() = default;

	Subscriber::~Subscriber()
	{
		SubscriptionsMapType::iterator it = m_subscriptions.begin();

		while(it != m_subscriptions.end())
		{
			it->second.reset();
			++it;
		}

		m_subscriptions.clear();
	}

	void Subscriber::subscribe(CallbackType cb, const EventType& evtName)
	{
		if(!m_subscriptions[evtName])
		{
			m_subscriptions[evtName] = Broker::get().subscribe(cb, evtName);
		}
	}

	void Subscriber::unsubscribe(const EventType& evtName)
	{
		if(m_subscriptions[evtName])
		{
			m_subscriptions[evtName].get()->disconnect();
			m_subscriptions[evtName].reset();
		}
	}
}}}}