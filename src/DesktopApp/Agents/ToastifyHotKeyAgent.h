#pragma once

#include "DesktopCore\Model\IAgent.h"
#include "DesktopCore\Utils\Patterns\PublisherSubscriber\Subscriber.h"

class CefBrowser;

namespace desktop { namespace ui {
	
	namespace cup = core::utils::patterns;
	
	namespace agent {
	
	class ToastifyHotKeyAgent : public core::model::IAgent
	{
	public:
		ToastifyHotKeyAgent(CefBrowser& browser);
		~ToastifyHotKeyAgent();

	private:
		CefBrowser&			m_browser;
		cup::Subscriber		m_subscriber;
	};
}}}