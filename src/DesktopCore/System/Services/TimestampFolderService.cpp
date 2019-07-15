#include "TimestampFolderService.h"

#include <sstream>

namespace desktop { namespace core { namespace service {

	TimestampFolderService::TimestampFolderService() = default;
	TimestampFolderService::~TimestampFolderService() = default;

	std::string TimestampFolderService::get(const std::string& timestamp) const
	{
		std::stringstream ss(timestamp);

		std::string year, month, day;
		getline(ss, year, '-'); getline(ss, month, '-'); getline(ss, day, 'T');

		int monthNumber = std::stoi(month);

		if (monthNumber <= 12)
		{
			month = months[monthNumber - 1];
		}

		return year + "\\" + month + "\\" + day + "\\";
	}

	std::string TimestampFolderService::get(time_t timestamp) const
	{
		std::stringstream ss;
		ss << timestamp;

		std::string raw = ss.str();
		return get(raw);
	}
}}}