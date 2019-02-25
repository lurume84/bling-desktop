#pragma once

#include <string>

namespace bling { namespace core { namespace service {
	class ApplicationDataService
	{
	public:
		ApplicationDataService();
		~ApplicationDataService();

		std::string getMyDocuments() const;
		std::string getApplicationName() const;
	};
}}}