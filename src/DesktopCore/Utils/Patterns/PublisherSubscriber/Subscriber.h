#pragma once

#include "Event.h"

#include <map>
#include <memory>
#include <functional>
#include <boost/signals2/signal.hpp>

namespace desktop { namespace core { namespace utils { namespace patterns {
	class Subscriber
	{
	public:
		typedef std::function<void (const Event&)> CallbackType;
		Subscriber();
		~Subscriber();
		void subscribe(CallbackType cb, const EventType& evtName);
		void unsubscribe(const EventType& evtName);
	private:
		typedef  std::unique_ptr<boost::signals2::scoped_connection> ConnectionType;
		typedef  std::map<EventType, ConnectionType> SubscriptionsMapType;

        SubscriptionsMapType m_subscriptions;
	};
}}}}