#include "HTTPClientService.h"

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/bind.hpp>
#include <cstdlib>

namespace desktop { namespace core { namespace service {

	using boost::asio::ip::tcp;

	HTTPClientService::HTTPClientService()
	: m_io_service()
	, context_(boost::asio::ssl::context::sslv23)
	{
		context_.set_options(
			boost::asio::ssl::context::default_workarounds
			| boost::asio::ssl::context::no_sslv2
			| boost::asio::ssl::context::no_sslv3
			| boost::asio::ssl::context::no_tlsv1);

		m_socket.reset(new boost::asio::ssl::stream<boost::asio::ip::tcp::socket>(m_io_service, context_));
	}

	HTTPClientService::~HTTPClientService() = default;

	bool HTTPClientService::get(const std::string& server, const std::string& port, const std::string& path,
		const std::map<std::string, std::string>& requestHeaders,
		std::map<std::string, std::string>& responseHeaders,
		std::string& content, unsigned int& status_code)
	{
		return send(server, port, "GET", path, requestHeaders, responseHeaders, content, status_code);
	}

	bool HTTPClientService::post(const std::string& server, const std::string& port, const std::string& path,
		const std::map<std::string, std::string>& requestHeaders,
		std::map<std::string, std::string>& responseHeaders,
		std::string& content, unsigned int& status_code)
	{
		return send(server, port, "POST", path, requestHeaders, responseHeaders, content, status_code);
	}

	bool HTTPClientService::send(const std::string& server, const std::string& port, const std::string& action, 
								const std::string& path, const std::map<std::string, std::string>& requestHeaders,
								std::map<std::string, std::string>& responseHeaders,
								std::string& content, unsigned int& status_code)
	{
		try
		{
			tcp::resolver resolver(m_io_service);
			tcp::resolver::query query(server, port);
			tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

			boost::system::error_code error;
			boost::asio::connect(m_socket->lowest_layer(), endpoint_iterator, error);

			m_socket->handshake(boost::asio::ssl::stream_base::client, error);

			boost::asio::streambuf request;
			std::ostream request_stream(&request);
			request_stream << action << " " << path << " HTTP/1.0\r\n";
			request_stream << "Host: " << server << "\r\n";
			request_stream << "Accept: */*\r\n";
			request_stream << "User-Agent: Mozilla / 5.0 (Windows NT 10.0; Win64; x64) AppleWebKit / 537.36 (KHTML, like Gecko) Chrome / 71.0.3578.98 Safari / 537.36\r\n";
			request_stream << "Connection: close\r\n";
		
			for (auto& header : requestHeaders)
			{
				request_stream << header.first << ": " << header.second << "\r\n";
			}

			request_stream << "\r\n";

			boost::asio::write(*(m_socket.get()), request);

			bool result = receive(responseHeaders, content, status_code);

			m_socket->shutdown(error);
			m_socket->lowest_layer().close(error);
			m_io_service.stop();

			return result;
		}
		catch (...)
		{
			return false;
		}	
	}

	bool HTTPClientService::receive(std::map<std::string, std::string>& headers, std::string& content, unsigned int& status_code)
	{
		boost::asio::streambuf response;
		boost::asio::read_until(*(m_socket.get()), response, "\r\n");

		std::istream response_stream(&response);
		std::string http_version;
		response_stream >> http_version;

		response_stream >> status_code;
		std::string status_message;
		std::getline(response_stream, status_message);

		if (!response_stream || "HTTP/" != http_version.substr(0, 5))
		{
			return false;
		}
		else
		{
			// Read the response headers, which are terminated by a blank line.
			boost::asio::read_until(*(m_socket.get()), response, "\r\n\r\n");

			// Process the response headers.
			std::string header;
			while (std::getline(response_stream, header) && header != "\r")
			{
				std::pair<std::string, std::string> header_struct;
				header_struct.first = header.substr(0, header.find(":"));
				header_struct.second = header.substr(header.find(":") + 2, header.size());

				if (*header_struct.second.rbegin() == '\n')
				{
					header_struct.second.pop_back();
				}

				if (*header_struct.second.rbegin() == '\r')
				{
					header_struct.second.pop_back();
				}

				headers.insert(header_struct);
			}

			std::stringstream content_os;

			// Write whatever content we already have to output.
			if (response.size() > 0)
			{
				content_os << &response;
			}

			// Read until EOF, writing data to output as we go.
			boost::system::error_code error;
			while (boost::asio::read(*(m_socket.get()), response, boost::asio::transfer_at_least(1), error))
			{
				content_os << &response;
			}

			content = content_os.str();

			return (boost::asio::error::eof == error);
		}
	}
}}}