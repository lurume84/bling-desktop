#pragma once

#include <memory>

namespace desktop { namespace core {
	
	namespace model
	{
		class IAgent;
	}

	class DesktopContext;
	
	class DesktopCore
	{
	public:
		DesktopCore();
		~DesktopCore();
		void initialize();
		void addAgent(std::unique_ptr<model::IAgent> agent);
	private:
		std::unique_ptr<DesktopContext> m_context;
	};
}}