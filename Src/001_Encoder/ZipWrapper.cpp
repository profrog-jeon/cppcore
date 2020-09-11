#include "stdafx.h"
#include "ZipWrapper.h"
#include "../../Inc/zip.h"
#include <stack>

namespace core
{
	CZipWrapper::CZipWrapper(void)
		: m_pZip(NULL)
	{
	}

	CZipWrapper::~CZipWrapper(void)
	{
	}

	bool CZipWrapper::FlushAndClose(void)
	{
		if (NULL == m_pZip)
			return true;

		if (0 < zip_close(m_pZip))
		{
			Log_Error("%s", GetLastErrorStr());
			m_pZip = NULL;
			return false;
		}

		m_pZip = NULL;
		return true;
	}

	void CZipWrapper::DiscardAndClose(void)
	{
		if (NULL == m_pZip)
			return;

		zip_discard(m_pZip);
		m_pZip = NULL;
	}

	LPCSTR CZipWrapper::GetLastErrorStr(void)
	{
		if (NULL == m_pZip)
			return "";

		LPCSTR pszErrMsg = zip_strerror(m_pZip);
		if (NULL == pszErrMsg)
			return "";

		return pszErrMsg;
	}
}
