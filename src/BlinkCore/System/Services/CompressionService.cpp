#include "CompressionService.h"

#include <boost/filesystem.hpp>

#include "sevenzip/7zpp.h"

namespace blink { namespace core { namespace service {

	CompressionService::CompressionService(const std::string& format)
	: m_format(format)
	{
		
	}

	CompressionService::~CompressionService()
	{
		
	}

	
	bool CompressionService::extract(const std::string& input, const std::string& output) const
	{
		try
		{
			SevenZip::SevenZipLibrary lib;

			if (m_format == "zip")
			{
				lib.Load("7z.dll"); //put 7z.dll here for compressing zip files (7za loaded by default supporting only 7z format)
			}
			else
			{
				lib.Load();
			}

			SevenZip::SevenZipExtractor extractor(lib, input);

			if (m_format == "zip")
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