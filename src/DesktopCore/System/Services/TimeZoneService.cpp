#include "TimeZoneService.h"

#include <sstream>

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/c_local_time_adjustor.hpp"

namespace desktop { namespace core { namespace service {

	namespace
	{
		boost::posix_time::time_duration get_utc_offset()
		{
			using namespace boost::posix_time;

			// boost::date_time::c_local_adjustor uses the C-API to adjust a
			// moment given in utc to the same moment in the local time zone.
			typedef boost::date_time::c_local_adjustor<ptime> local_adj;

			const ptime utc_now = second_clock::universal_time();
			const ptime now = local_adj::utc_to_local(utc_now);

			return utc_now - now;
		}
	}

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

			boost::posix_time::time_duration offset = get_utc_offset();

			std::string iso = boost::posix_time::to_iso_extended_string(local);

			int64_t hours = offset.hours();
			char symbol = '+';

			if(offset.is_negative())
			{
				hours = offset.invert_sign().hours();
				symbol = '-';
			}

			std::stringstream ss;
			ss << iso.substr(0, iso.find_first_of(","));
			ss << symbol << std::setw(2) << std::setfill('0') << hours;
			ss << ":";
			ss << std::setw(2) << std::setfill('0') << offset.minutes();

			return ss.str();
		}
		catch (...)
		{
			return timestamp;
		}
	}

	std::string TimeZoneService::universalToLocal(time_t timestamp) const
	{
		std::string iso = boost::posix_time::to_iso_extended_string(boost::posix_time::from_time_t(timestamp));

		std::string utc = iso.substr(0, iso.find_first_of(",")) + "+00:00";

		return universalToLocal(utc);
	}
}}}