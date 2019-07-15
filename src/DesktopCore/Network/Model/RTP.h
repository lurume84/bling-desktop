#pragma once

#include <string>

namespace desktop { namespace core { namespace model { 
	struct RTP
	{
		RTP(const std::string& url)
		: m_url(url)
		{
		
		}

		std::string m_url;
	};
}}}