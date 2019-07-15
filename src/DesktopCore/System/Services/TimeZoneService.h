#pragma once

#include <string>

namespace desktop { namespace core { namespace service {
	class TimeZoneService
	{
	public:
		TimeZoneService();
		~TimeZoneService();

		std::string universalToLocal(const std::string& timestamp) const;
		std::string universalToLocal(time_t timestamp) const;
	};
}}}