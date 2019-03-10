#pragma once

#include <memory>
#include <vector>

namespace desktop { namespace core {

	namespace model
	{
		class IAgent;
	}

	struct DesktopContext
	{
		std::vector<std::unique_ptr<model::IAgent>> m_agents;
	};
}}