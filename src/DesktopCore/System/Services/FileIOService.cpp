#include "FileIOService.h"

#include <sstream>

namespace desktop { namespace core { namespace service {

	FileIOService::FileIOService() = default;
	FileIOService::~FileIOService() = default;

	bool FileIOService::load(const boost::filesystem::path& input, std::stringstream& content) const
	{
		if (boost::filesystem::exists(input))
		{
			std::ifstream f(input.string(), std::ios_base::in | std::ios_base::binary);

			if (f)
			{
				content << f.rdbuf();
				return true;
			}
		}

		return false;
	}

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