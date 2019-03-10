#include "DesktopCore.h"

#include "DesktopContext.h"

#include "Model/IAgent.h"

namespace desktop { namespace core {
	DesktopCore::DesktopCore() = default;
	DesktopCore::~DesktopCore() = default;
	
	void DesktopCore::initialize()
	{
		m_context = std::make_unique<DesktopContext>();
	}

	void DesktopCore::addAgent(std::unique_ptr<model::IAgent> agent)
	{
		m_context->m_agents.push_back(std::move(agent));
	}
}}