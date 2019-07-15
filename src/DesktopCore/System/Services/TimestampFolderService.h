#pragma once

#include <string>

namespace desktop { namespace core { namespace service {
	class TimestampFolderService
	{
	public:
		TimestampFolderService();
		~TimestampFolderService();

		std::string get(const std::string& timestamp) const;
		std::string get(time_t timestamp) const;

		std::string months[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
	};
}}}