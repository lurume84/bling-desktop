#pragma once

#include "FileIOService.h"

#include <string>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace desktop { namespace core { namespace service {
	class IniFileService
	{
	public:
		IniFileService(std::unique_ptr<service::FileIOService> fileIOService = std::make_unique<service::FileIOService>());
		~IniFileService();

		template<typename T>
		bool set(const std::string& path, const std::string& section, const std::string& entry, T value)
		{
			std::stringstream iss;

			boost::property_tree::ptree tree;

			if (m_fileIOService->load(path, iss))
			{
				boost::property_tree::ini_parser::read_ini(iss, tree);
			}

			tree.put<T>(section + "." + entry, value);

			std::ostringstream oss;
			boost::property_tree::ini_parser::write_ini(oss, tree);

			return m_fileIOService->save(path, oss.str());
		}

		template<typename T>
		T get(const std::string& path, std::string section, std::string entry, T defaultValue)
		{
			std::stringstream iss;

			if (m_fileIOService->load(path, iss))
			{
				boost::property_tree::ptree tree;
				boost::property_tree::ini_parser::read_ini(iss, tree);

				return tree.get<T>(section + "." + entry, defaultValue);
			}

			return defaultValue;
		}

	private:
		std::unique_ptr<service::FileIOService> m_fileIOService;
	};
}}}