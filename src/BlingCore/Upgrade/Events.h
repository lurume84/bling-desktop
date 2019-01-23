#include "..\Utils\Patterns\PublisherSubscriber\Event.h"

namespace bling { namespace core { namespace events {
	
	namespace sup = utils::patterns;
	
	const sup::EventType DOWNLOAD_UPGRADE_EVENT = "DOWNLOAD_UPGRADE_EVENT";
	struct DownloadUpgradeEvent : public sup::Event
	{
		DownloadUpgradeEvent(const std::string& version)
		: m_version(version)
		{
			m_name = DOWNLOAD_UPGRADE_EVENT;
		}

		std::string m_version;
	};

	const sup::EventType EXTRACT_UPGRADE_EVENT = "EXTRACT_UPGRADE_EVENT";
	struct ExtractUpgradeEvent : public sup::Event
	{
		ExtractUpgradeEvent(const std::string& version)
			: m_version(version)
		{
			m_name = EXTRACT_UPGRADE_EVENT;
		}

		std::string m_version;
	};

	const sup::EventType UPGRADE_COMPLETED_EVENT = "UPGRADE_COMPLETED_EVENT";
	struct UpgradeCompletedEvent : public sup::Event
	{
		UpgradeCompletedEvent(const std::string& version)
			: m_version(version)
		{
			m_name = UPGRADE_COMPLETED_EVENT;
		}

		std::string m_version;
	};
}}}