#include "Broker.h"

#include "Event.h"

namespace desktop { namespace core { namespace utils { namespace patterns {

	Broker& Broker::get()
    {
        static Broker S;
        return S;
    }
    
	Broker::Broker() = default;

	Broker::~Broker()
	{
		std::unique_lock<std::mutex> lock(m_mutexSubscriptions);

		TStrSubscriberMap::iterator it = m_subscriptions.begin();
		while(it != m_subscriptions.end())
		{
			it->second.reset();
			++it;
		}
        
		m_subscriptions.clear();
	}

	Broker::SignalType& Broker::getSignal(const EventType& evtName)
	{
		std::unique_lock<std::mutex> lock(m_mutexSubscriptions);
		
		TStrSubscriberMap::iterator it = m_subscriptions.find(evtName);

		if(it == m_subscriptions.end())
		{
			m_subscriptions[evtName] = std::make_unique<SignalType>();
            return *m_subscriptions[evtName];
		}
		else
		{
			return *it->second;
		}
	}

	std::unique_ptr<Broker::ConnectionType> Broker::subscribe(CallbackType cb, const EventType& evtName)
	{
		SignalType& signal = getSignal(evtName);
		return std::make_unique<ConnectionType>(signal.connect(cb));
	}

	void Broker::publish(const Event& evt)
	{
		getSignal(evt.m_name)(evt);
	}
}}}}