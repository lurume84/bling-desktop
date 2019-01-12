#include <string>

namespace blink { namespace core { namespace service {
	class CompressionService
	{
	public:
		CompressionService(const std::string& format);
		~CompressionService();

		bool extract(const std::string& input, const std::string& output) const;
	private:
		std::string m_format;
	};
}}}