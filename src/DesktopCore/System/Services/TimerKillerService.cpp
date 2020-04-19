#include "TimerKillerService.h"

namespace desktop { namespace core { namespace service {

	void TimerKillerService::kill()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_terminate = true;
		m_cv.notify_all();
	}
}}}