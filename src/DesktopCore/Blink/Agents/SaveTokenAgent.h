#pragma once

#include "../../System/Services/ApplicationDataService.h"
#include "../../System/Services/FileIOService.h"
#include "../../Utils\Patterns\PublisherSubscriber\Subscriber.h"
#include "../../Model/IAgent.h"

#include <string>
#include <map>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

namespace desktop { namespace core { 
	
	namespace model
	{
		struct Credentials;
	}
	
	namespace agent {
	
	namespace cup = core::utils::patterns;
	
	class SaveTokenAgent : public model::IAgent
	{
	public:
		SaveTokenAgent(std::unique_ptr<service::FileIOService> fileIOService = std::make_unique<service::FileIOService>(),
						std::unique_ptr<service::ApplicationDataService> applicationService = std::make_unique<service::ApplicationDataService>());
		~SaveTokenAgent();
	private:
		std::unique_ptr<service::FileIOService> m_fileIOService;
		std::unique_ptr<service::ApplicationDataService> m_applicationService;
		cup::Subscriber m_subscriber;
	};
}}}