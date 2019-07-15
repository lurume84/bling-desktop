#pragma once

#include "Model\NamedType.h"

#include <string>
#include <boost/filesystem/path.hpp>

namespace desktop { namespace core { namespace model { namespace system {
	struct ExecutableFile
	{
		using Path = NamedType<boost::filesystem::path, struct PathParameter>;
		using Arguments = NamedType<std::string, struct ArgumentsParameter>;

		ExecutableFile(Path path, Arguments arguments);

		bool operator==(const ExecutableFile& other) const;

		Path m_path;
		Arguments m_arguments;
	};
}}}}