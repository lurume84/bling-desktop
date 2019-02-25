#include "ReplaceFolderService.h"

#include <boost/filesystem.hpp>

namespace desktop { namespace core { namespace service {

	ReplaceFolderService::ReplaceFolderService() = default;
	ReplaceFolderService::~ReplaceFolderService() = default;

	bool ReplaceFolderService::replace(const std::string& input, const std::string& ouput) const
	{
		try
		{
			if (boost::filesystem::exists(ouput))
			{
				boost::filesystem::remove_all(ouput);
			}

			boost::filesystem::rename(input, ouput);

			return true;
		}
		catch (...)
		{
			return false;
		}
	}
	
}}}