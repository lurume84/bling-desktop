#pragma once

#include <mutex>
#include <condition_variable>

namespace desktop { namespace core { namespace service {
	class TimerKillerService
	{
	public:
		TimerKillerService() = default;
		~TimerKillerService() = default;

		template<class R, class P>
		bool wait_for(std::chrono::duration<R, P> const& time) const
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			return !m_cv.wait_for(lock, time, [&] {return m_terminate; });
		}

		void kill();
	private:
		mutable std::condition_variable m_cv;
		mutable std::mutex m_mutex;
		bool m_terminate = false;
	};
}}}