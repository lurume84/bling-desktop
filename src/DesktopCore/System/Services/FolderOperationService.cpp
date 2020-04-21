#include "FolderOperationService.h"

#include <boost/filesystem.hpp>

namespace desktop { namespace core { namespace service {

	FolderOperationService::FolderOperationService() = default;
	FolderOperationService::~FolderOperationService() = default;

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

	bool FolderOperationService::copy(const std::string& input, const std::string& output) const
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

	bool FolderOperationService::replace(const std::string& input, const std::string& output) const
	{
		try
		{
			if (boost::filesystem::exists(output))
			{
				boost::filesystem::remove_all(output);
			}

			boost::system::error_code ec;
			boost::filesystem::rename(input, output, ec);

			if (ec)
			{
				auto res = copy(input, output);
				boost::filesystem::remove_all(input);

				return res;
			}
			else
			{
				return true;
			}
		}
		catch (...)
		{
			return false;
		}
	}
}}}