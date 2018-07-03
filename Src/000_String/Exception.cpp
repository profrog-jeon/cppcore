#include "stdafx.h"
#include "Exception.h"
#include "StdString.h"
#include "StdStringLegacy.h"
#include "Unicode.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	exception_format::exception_format(const TCHAR* pszFormat, ...)
		: std::exception()
	{
		const size_t tBuffSize = 2048;
		TCHAR szBuff[tBuffSize] = { 0, };
		va_list list;
		va_start(list, pszFormat);
		SafeSVPrintf(szBuff, tBuffSize, pszFormat, list);
		va_end(list);

		m_strWhatA = MBSFromTCS(szBuff);
		m_strWhatW = WCSFromTCS(szBuff);
	}
}

