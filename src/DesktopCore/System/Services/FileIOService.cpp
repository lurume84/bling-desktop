#include "FileIOService.h"

namespace desktop { namespace core { namespace service {

	FileIOService::FileIOService() = default;
	FileIOService::~FileIOService() = default;

	bool FileIOService::save(const boost::filesystem::path& output, const std::string& content) const
	{
		try
		{
			boost::filesystem::create_directories(output.parent_path());

			std::ofstream f(output.string(), std::ios::binary);
			f << content;
			f.close();

			return true;
		}
		catch (...)
		{
			return false;
		}
	}
	
}}}