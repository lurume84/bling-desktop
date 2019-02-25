#pragma once

#include "stdafx.h"

namespace desktop { namespace ui {  namespace toast {
	class CToastPPCommandLineInfo : public CCommandLineInfo
	{
	public:
		//Constructors / Destructors
		CToastPPCommandLineInfo() : m_bRegister(FALSE),
			m_bUnRegister(FALSE)
		{

		}

		//Methods
		void ParseParam(const TCHAR* lpszParam, BOOL bFlag, BOOL bLast) override
		{
			if (bFlag)
			{
				CString sParamUpper(lpszParam);
				sParamUpper.MakeUpper();
				if (sParamUpper == _T("REGSERVER"))
				{
					m_bRegister = TRUE;
					m_bUnRegister = FALSE;
				}
				else if (sParamUpper == _T("UNREGSERVER"))
				{
					m_bUnRegister = TRUE;
					m_bRegister = FALSE;
				}
				else
					__super::ParseParam(lpszParam, bFlag, bLast);
			}
			else
				__super::ParseParam(lpszParam, bFlag, bLast);
		};

		//Member variables
		BOOL m_bRegister;
		BOOL m_bUnRegister;
	};
}}}