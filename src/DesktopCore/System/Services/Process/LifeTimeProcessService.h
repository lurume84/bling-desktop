#pragma once

namespace desktop { namespace core {
	
	namespace model { namespace system {
		struct ProcessInformation;
	}}
	
	namespace service { namespace system {

	class LifeTimeProcessService
	{
	public:
		virtual ~LifeTimeProcessService() = default;
		virtual bool isAlive(model::system::ProcessInformation&) const;
		//virtual bool isAlive(model::system::DesktopInformation&) const;
	};
}}}}