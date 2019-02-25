#include "EncodeStringService.h"

#include <windows.h>

#include <vector>

namespace desktop { namespace core { namespace service {

	EncodeStringService::EncodeStringService() = default;
	EncodeStringService::~EncodeStringService() = default;

	std::wstring EncodeStringService::utf8toUtf16(const std::string & str) const
	{
		if (str.empty())
			return std::wstring();

		size_t charsNeeded = ::MultiByteToWideChar(CP_UTF8, 0,
			str.data(), (int)str.size(), NULL, 0);
		if (charsNeeded == 0)
			throw std::runtime_error("Failed converting UTF-8 string to UTF-16");

		std::vector<wchar_t> buffer(charsNeeded);
		int charsConverted = ::MultiByteToWideChar(CP_UTF8, 0,
			str.data(), (int)str.size(), &buffer[0], buffer.size());
		if (charsConverted == 0)
			throw std::runtime_error("Failed converting UTF-8 string to UTF-16");

		return std::wstring(&buffer[0], charsConverted);
	}
	
}}}