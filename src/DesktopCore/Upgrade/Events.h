#pragma once

#include "..\Utils\Patterns\PublisherSubscriber\Event.h"

namespace desktop { namespace core { namespace events {
	
	namespace sup = utils::patterns;
	
	const sup::EventType DOWNLOAD_UPGRADE_EVENT = "DOWNLOAD_UPGRADE_EVENT";
	struct DownloadUpgradeEvent : public sup::Event
	{
		DownloadUpgradeEvent(const std::string& version, std::function<bool()> callback)
		: m_version(version)
		, m_callback(callback)
		{
			m_name = DOWNLOAD_UPGRADE_EVENT;
		}

		std::string m_version;
		std::function<bool()> m_callback;
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

	const sup::EventType UPGRADE_VIEWER_COMPLETED_EVENT = "UPGRADE_VIEWER_COMPLETED_EVENT";
	struct UpgradeViewerCompletedEvent : public sup::Event
	{
		UpgradeViewerCompletedEvent(const std::string& version)
			: m_version(version)
		{
			m_name = UPGRADE_VIEWER_COMPLETED_EVENT;
		}

		std::string m_version;
	};

	const sup::EventType UPGRADE_DESKTOP_COMPLETED_EVENT = "UPGRADE_DESKTOP_COMPLETED_EVENT";
	struct UpgradeDesktopCompletedEvent : public sup::Event
	{
		UpgradeDesktopCompletedEvent(const std::string& version)
			: m_version(version)
		{
			m_name = UPGRADE_DESKTOP_COMPLETED_EVENT;
		}

		std::string m_version;
	};
}}}