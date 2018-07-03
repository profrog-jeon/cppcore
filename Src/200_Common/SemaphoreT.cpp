#include "stdafx.h"
#include "Semaphore.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	ECODE CSemaphore::Create(int nInitialCount, LPCTSTR pszName)
	{
		if( m_hSemaphore )
			return EC_ALREADY_CREATED;

		HANDLE hSemaphore = CreateSemaphore(nInitialCount, pszName);
		if( NULL == hSemaphore )
			return GetLastError();

		m_hSemaphore = hSemaphore;
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSemaphore::Open(LPCTSTR pszName)
	{
		if( m_hSemaphore )
			return EC_ALREADY_CREATED;

		HANDLE hSemaphore = OpenSemaphore(pszName);
		if( NULL == hSemaphore )
			return GetLastError();

		m_hSemaphore = hSemaphore;
		return EC_SUCCESS;
	}
}
