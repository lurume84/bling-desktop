#include "SaveTokenAgent.h"

#include "Utils\Patterns\PublisherSubscriber\Broker.h"
#include "../../Network/Events.h"
#include "..\..\Network\Model\Credentials.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
#include <sstream>

namespace desktop { namespace core { namespace agent {

	SaveTokenAgent::SaveTokenAgent(std::unique_ptr<service::FileIOService> fileIOService, std::unique_ptr<service::ApplicationDataService> applicationService)
	: m_fileIOService(std::move(fileIOService))
	, m_applicationService(std::move(applicationService))
	{
		m_subscriber.subscribe([this](const desktop::core::utils::patterns::Event& rawEvt)
		{
			const auto& evt = static_cast<const core::events::CredentialsEvent&>(rawEvt);

			auto credentials = std::make_unique<model::Credentials>(evt.m_credentials);

			auto viewerFolder = m_applicationService->getViewerFolder();
			
			try
			{
				boost::property_tree::ptree tree;

				{
					boost::property_tree::ptree token;
					token.add_child("authtoken", boost::property_tree::ptree{ credentials->m_token });

					tree.add_child("authtoken", token);
				}

				{
					std::string region;

					std::stringstream ss(credentials->m_host);
					getline(ss, region, '-'); getline(ss, region, '.');

					boost::property_tree::ptree token;
					token.add_child(region, boost::property_tree::ptree{ "" });

					tree.add_child("region", token);
				}

				boost::property_tree::json_parser::write_json(viewerFolder + "\\token.json", tree);
			}
			catch (...)
			{
				
			}
			
		}, events::CREDENTIALS_EVENT);
	}

	SaveTokenAgent::~SaveTokenAgent()
	{
		
	}
}}}