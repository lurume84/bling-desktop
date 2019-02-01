#include "IDownloadFileService.h"

namespace bling { namespace core { namespace service {

	class HTTPClientService;
	class ParseURIService;

	class DownloadFileService : public IDownloadFileService
	{
	public:
		DownloadFileService(std::unique_ptr<service::HTTPClientService> clientService = std::make_unique<service::HTTPClientService>(),
							std::unique_ptr<service::ParseURIService> uriService = std::make_unique<service::ParseURIService>());
		~DownloadFileService();
		std::string download(const std::string& host, const std::string& url) const override;
	private:
		std::unique_ptr<service::HTTPClientService> m_clientService;
		std::unique_ptr<service::ParseURIService> m_uriService;
	};
}}}