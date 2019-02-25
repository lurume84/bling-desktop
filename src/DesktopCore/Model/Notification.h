#pragma once

#include <functional>

namespace desktop { namespace core { namespace model { 
	struct Notification
	{
		Notification(std::function<bool()> activated, std::function<bool()> dismissed, std::function<bool()> failed)
		: m_activated(activated)
		, m_dismissed(dismissed)
		, m_failed(failed)
		{
		
		}

		std::function<bool()> m_activated;
		std::function<bool()> m_dismissed;
		std::function<bool()> m_failed;
	};
}}}