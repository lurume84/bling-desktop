#pragma once

#include <string>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

namespace desktop { namespace core { namespace service {

	class HTTPClientService
	{
	public:
		HTTPClientService();
		~HTTPClientService();
		bool get(const std::string& server, const std::string& port, const std::string&, 
					const std::map<std::string, std::string>& requestHeaders, 
					std::map<std::string, std::string>& responseHeaders, 
					std::string& content, unsigned int& status_code);
		bool post(const std::string& server, const std::string& port, const std::string&,
			const std::map<std::string, std::string>& requestHeaders,
			std::map<std::string, std::string>& responseHeaders,
			std::string& content, unsigned int& status_code);
	private:
		bool send(const std::string& server, const std::string& port, const std::string& action,
			const std::string& path, const std::map<std::string, std::string>& requestHeaders,
			std::map<std::string, std::string>& responseHeaders,
			std::string& content, unsigned int& status_code);
		bool receive(std::map<std::string, std::string>& headers, std::string& content, unsigned int& status_code);
	private:
		boost::asio::io_service m_io_service;
		std::string m_root;
		std::auto_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> m_socket;
		boost::asio::ssl::context context_;
	};
}}}