#include "ExecutableFile.h"

namespace desktop { namespace core { namespace model { namespace system {
	ExecutableFile::ExecutableFile(Path path, Arguments arguments)
	: m_path(std::move(path))
	, m_arguments(std::move(arguments))
	{

	}

	bool ExecutableFile::operator==(const ExecutableFile& other) const
	{
		return m_path.get() == other.m_path.get() && m_arguments.get() == other.m_arguments.get();
	}

}}}}