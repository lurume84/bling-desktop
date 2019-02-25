#include "ReplaceFolderService.h"

#include <boost/filesystem.hpp>

namespace desktop { namespace core { namespace service {

	ReplaceFolderService::ReplaceFolderService(std::unique_ptr<CopyFolderService> copyService)
	: m_copyService(std::move(copyService))
	{

	}

	ReplaceFolderService::~ReplaceFolderService() = default;

	bool ReplaceFolderService::replace(const std::string& input, const std::string& output) const
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
				auto res = m_copyService->copy(input, output);
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