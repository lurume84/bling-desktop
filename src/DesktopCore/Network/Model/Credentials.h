#pragma once

#include <string>

namespace desktop { namespace core { namespace model { 
	struct Credentials
	{
		Credentials(const std::string& host, const std::string& port, const std::string& token, const std::string& account)
		: m_host(host)
		, m_port(port)
		, m_token(token)
		, m_account(account)
		{
		
		}

		std::string m_host, m_port, m_token, m_account;
	};
}}}