#pragma once

#include <string>

namespace bling { namespace core { namespace model { 
	struct Credentials
	{
		Credentials(const std::string& host, const std::string& port, const std::string& token)
		: m_host(host)
		, m_port(port)
		, m_token(token)
		{
		
		}

		std::string m_host, m_port, m_token;
	};
}}}