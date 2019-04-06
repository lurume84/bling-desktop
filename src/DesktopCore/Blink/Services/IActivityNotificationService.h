#pragma once

#include <string>
#include <map>
#include <memory>

namespace desktop { namespace core { namespace service {

	class IActivityNotificationService
	{
	public:
		virtual ~IActivityNotificationService() = default;
		virtual void notify() = 0;
	};
}}}