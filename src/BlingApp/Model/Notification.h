#pragma once

#include "Toast\Toast.h"
#include "Toast\ToastEventHandler.h"
#include <memory>

namespace bling { namespace ui { 
	struct Notification
	{
		Notification();

		ToastPP::CToast m_toast;
	};
}}