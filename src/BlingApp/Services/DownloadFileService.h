#pragma once

#include "BlingCore\Utils\Patterns\PublisherSubscriber\Subscriber.h"

#include "BlingCore\Network\Services\IDownloadFileService.h"

#pragma warning(push)
#pragma warning(disable : 4100)
#pragma warning(disable : 4481)
#include <cef/cef_app.h>
#pragma warning(pop)

#include <mutex>
#include <condition_variable>

namespace bling { 
	
	namespace core { namespace service {
		class HTTPClientService;
	}}
	
	namespace ui { namespace service {

	namespace cup = core::utils::patterns;

	class DownloadFileService : public core::service::IDownloadFileService
	{
	public:
		DownloadFileService(CefRefPtr<CefBrowser> browser);
		~DownloadFileService();
		std::string download(const std::string& host, const std::string& url, const std::string &folder) const override;
	private:
		CefRefPtr<CefBrowser>	m_browser;
		cup::Subscriber			m_subscriber;

		std::string				m_path;

		mutable std::condition_variable m_cv;
		mutable std::mutex				m_mutex;
	};
}}}