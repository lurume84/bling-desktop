#pragma once

#include <memory>
#include <string>

namespace ToastPP
{
	class CToast;
}

namespace bling { namespace ui {  namespace toast {
	struct ToastFactory
	{
		std::unique_ptr<ToastPP::CToast> getBasic(const std::wstring&, const std::wstring&) const;
	};
}}}