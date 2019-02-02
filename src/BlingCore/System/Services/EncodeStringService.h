#pragma once

#include <string>

namespace bling { namespace core { namespace service {
	class EncodeStringService
	{
	public:
		EncodeStringService();
		~EncodeStringService();

		std::wstring utf8toUtf16(const std::string & str) const;
	};
}}}