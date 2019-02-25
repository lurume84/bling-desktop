#pragma once

#include <string>

namespace desktop { namespace core { namespace service {
	class CompressionService
	{
	public:
		CompressionService();
		~CompressionService();

		bool extract(const std::string& format, const std::string& input, const std::string& output) const;
	};
}}}