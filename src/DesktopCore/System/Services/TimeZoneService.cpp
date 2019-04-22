#include "TimeZoneService.h"

#include <sstream>

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/c_local_time_adjustor.hpp"

namespace desktop { namespace core { namespace service {

	TimeZoneService::TimeZoneService() = default;
	TimeZoneService::~TimeZoneService() = default;

	std::string TimeZoneService::universalToLocal(const std::string& timestamp) const
	{
		//2019-04-20T17:38:24+00:00
		try 
		{
			const std::locale loc = std::locale(std::locale::classic(), new boost::posix_time::time_input_facet("%Y-%m-%dT%H:%M:%S+00:00"));
			std::istringstream is(timestamp);
			is.imbue(loc);

			boost::posix_time::ptime universal;
			is >> universal;

			boost::posix_time::ptime local = boost::date_time::c_local_adjustor<boost::posix_time::ptime>::utc_to_local(universal);

			return boost::posix_time::to_simple_string(local);
		}
		catch (...)
		{
			return timestamp;
		}
	}
}}}