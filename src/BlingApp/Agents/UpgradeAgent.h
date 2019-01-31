#pragma once

#include "BlingCore\Utils\Patterns\PublisherSubscriber\Subscriber.h"

namespace bling { namespace ui {  
	
	namespace agent {

	namespace cup = core::utils::patterns;

	class UpgradeAgent
	{
	public:
		UpgradeAgent();
		~UpgradeAgent();

	private:
		cup::Subscriber m_subscriber;
	};
}}}