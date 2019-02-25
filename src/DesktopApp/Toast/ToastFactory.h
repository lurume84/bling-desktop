#pragma once

#include <memory>
#include <string>

namespace ToastPP
{
	class CToast;
}

namespace desktop { namespace ui {  namespace toast {
	struct ToastFactory
	{
		std::shared_ptr<ToastPP::CToast> getBasic(const std::wstring&, const std::wstring&) const;
		std::shared_ptr<ToastPP::CToast> getYesNo(const std::wstring&, const std::wstring&, const std::wstring&) const;
	};
}}}