#pragma once

#include <string>
#include <boost/throw_exception.hpp>
#include <boost/exception/info.hpp>

namespace desktop { namespace core {
	
	namespace model { namespace system {
		struct ProcessInformation;
		struct ExecutableFile;
	}}
	
	namespace service { namespace system {

	class ICreateProcessService
	{
	public:
		virtual ~ICreateProcessService() = default;
		virtual std::unique_ptr<model::system::ProcessInformation> create(const model::system::ExecutableFile& file, const std::string& currentDirectory) const = 0;
	};

	class CreateProcessService : public ICreateProcessService
	{
	public:
		struct CreateProcessServiceException : public virtual std::exception, public virtual boost::exception
		{
			char const * what() const throw()
			{
				return "Unable to start the proces";
			}

			typedef boost::error_info<struct pathErrorType, const std::string> pathInfo;
			typedef boost::error_info<struct ErrorCodeType, int> errorCodeInfo;
		};

		virtual std::unique_ptr<model::system::ProcessInformation> create(const model::system::ExecutableFile& file, const std::string& currentDirectory) const override;
	};
}}}}