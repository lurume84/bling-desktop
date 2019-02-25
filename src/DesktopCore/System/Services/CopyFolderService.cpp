#include "CopyFolderService.h"

#include <boost/filesystem.hpp>

namespace desktop { namespace core { namespace service {

	CopyFolderService::CopyFolderService() = default;
	CopyFolderService::~CopyFolderService() = default;

	namespace
	{
		void recursiveCopy(const boost::filesystem::path& input, const boost::filesystem::path& output)
		{
			if (boost::filesystem::exists(output))
			{
				throw std::runtime_error(output.generic_string() + " exists");
			}

			if (boost::filesystem::is_directory(input))
			{
				boost::filesystem::create_directories(output);

				for (boost::filesystem::directory_entry& item : boost::filesystem::directory_iterator(input))
				{
					recursiveCopy(item.path(), output / item.path().filename());
				}
			}
			else if (boost::filesystem::is_regular_file(input))
			{
				boost::filesystem::copy(input, output);
			}
			else
			{
				throw std::runtime_error(output.generic_string() + " not dir or file");
			}
		}
	}

	bool CopyFolderService::copy(const std::string& input, const std::string& output) const
	{
		try
		{
			recursiveCopy(input, output);
			return true;
		}
		catch (...)
		{
			return false;
		}
	}
}}}