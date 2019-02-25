#include "stdafx.h"

#include "ToastFactory.h"

#include "Toast.h"

namespace desktop { namespace ui {  namespace toast {

	namespace
	{
		CStringW FixUpImagesInXML(_In_ const CStringW& sXML)
		{
			CStringW sUpdatedXML(sXML);
			std::wstring sModuleName;
			if (FAILED(ToastPP::CManager::GetExecutablePath(sModuleName)))
				return sUpdatedXML;
			ATL::CPathW sPath(sModuleName.c_str());
			if (!sPath.RemoveFileSpec())
				return sUpdatedXML;
			sUpdatedXML.Replace(L"%EXEPATH%", sPath);
			return sUpdatedXML;
		}
	}

	std::shared_ptr<ToastPP::CToast> ToastFactory::getBasic(const std::wstring& title, const std::wstring& message) const
	{
		auto toast = std::make_shared<ToastPP::CToast>();
		
		std::wstring raw = L"<toast>\r\n\
								<visual>\r\n\
									<binding template=\"ToastGeneric\">\r\n\
										<text>" + title + L"</text>\r\n\
										<text>" + message + L"</text>\r\n\
									</binding>\r\n\
								</visual>\r\n\
							</toast>";

		toast->Create(FixUpImagesInXML(raw.c_str()));
		
		return std::move(toast);
	}

	std::shared_ptr<ToastPP::CToast> ToastFactory::getYesNo(const std::wstring& title, const std::wstring& message, const std::wstring& action) const
	{
		auto toast = std::make_shared<ToastPP::CToast>();

		std::wstring raw = L"<toast launch=\"action=viewAlarm&amp;alarmId=3\">\r\n\
			 <visual>\r\n\
			  <binding template=\"ToastGeneric\">\r\n\
			   <text>" + title + L"</text>\r\n\
			   <text>" + message + L"</text>\r\n\
			  </binding>\r\n\
			 </visual>\r\n\
			 <actions>\r\n\
			 <action arguments=\"accept\" content=\"" + action + L"\"/>\r\n\
			 <action arguments=\"dismiss\" content=\"Dismiss\"/>\r\n\
			 </actions>\r\n\
			</toast>";

		toast->Create(FixUpImagesInXML(raw.c_str()));

		return std::move(toast);
	}
}}}