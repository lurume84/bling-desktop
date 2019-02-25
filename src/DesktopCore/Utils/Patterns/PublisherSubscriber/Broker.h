#pragma once

#include "Event.h"

#include <boost/signals2/signal.hpp>
#include <map>
#include <mutex>

namespace desktop { namespace core { namespace utils { namespace patterns {
	class Subscriber;
	struct Event;

	class Broker
	{
	public:
		typedef  boost::signals2::scoped_connection ConnectionType;
		typedef std::function<void (const Event&)> CallbackType;
		void unsubscribe(CallbackType cb, const EventType& evtName);
		std::unique_ptr<ConnectionType> subscribe(CallbackType cb, const EventType& evtName);
		void publish(const Event& evt);
        static Broker& get();
	private:
		typedef boost::signals2::signal<void (const Event&)> SignalType;
		Broker();
		Broker(const Broker &);
		~Broker();
		Broker & operator=(const Broker&) = delete;
		Broker * operator&() = delete;
        
		SignalType& getSignal(const EventType& evtName);
	private:
		typedef std::map<EventType, std::unique_ptr<SignalType>> TStrSubscriberMap;
		
        TStrSubscriberMap m_subscriptions;
		std::mutex m_mutexSubscriptions;
	};
}}}}