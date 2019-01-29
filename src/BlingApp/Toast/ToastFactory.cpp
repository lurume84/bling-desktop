#include "stdafx.h"

#include "ToastFactory.h"

#include "Toast.h"

namespace bling { namespace ui {  namespace toast {

	std::unique_ptr<ToastPP::CToast> ToastFactory::getBasic(const std::wstring& title, const std::wstring& message) const
	{
		auto toast = std::make_unique<ToastPP::CToast>();
		
		std::wstring raw = L"<toast>\r\n\
								<visual>\r\n\
								<binding template=\"ToastGeneric\">\r\n\
								<text>" + title + L"</text>\r\n\
								<text>" + message + L"</text>\r\n\
								</binding>\r\n\
								</visual>\r\n\
							</toast>";

		toast->Create(raw.c_str());
		
		return std::move(toast);
	}
}}}