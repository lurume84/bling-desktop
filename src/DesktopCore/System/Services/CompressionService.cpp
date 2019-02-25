#include "CompressionService.h"

#include <boost/filesystem.hpp>

#include "sevenzip/7zpp.h"

namespace desktop { namespace core { namespace service {

	CompressionService::CompressionService() = default;
	CompressionService::~CompressionService() = default;
	
	bool CompressionService::extract(const std::string& format, const std::string& input, const std::string& output) const
	{
		try
		{
			SevenZip::SevenZipLibrary lib;

			if (format == "zip")
			{
				lib.Load("7z.dll"); //put 7z.dll here for compressing zip files (7za loaded by default supporting only 7z format)
			}
			else
			{
				lib.Load();
			}

			SevenZip::SevenZipExtractor extractor(lib, input);

			if (format == "zip")
			{
				extractor.SetCompressionFormat(SevenZip::CompressionFormat::Zip);
			}

			extractor.ExtractArchive(boost::filesystem::canonical(output).string());

			return true;
		}
		catch (SevenZip::SevenZipException& ex)
		{
			
		}

		return false;
	}
	
}}}