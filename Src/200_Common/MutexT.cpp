#include "stdafx.h"
#include "Mutex.h"

namespace core
{
	CMutex::CMutex(LPCTSTR pszName)
	: m_hMutex(NULL)
	{
		m_hMutex = CreateMutex(false, pszName);
		if( NULL == m_hMutex )
			Log_Error(TEXT("CreateMutex(false, %s) has failed in CMutex"), pszName);
	}
}