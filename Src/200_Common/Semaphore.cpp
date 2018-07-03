#include "stdafx.h"
#include "Semaphore.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CSemaphore::CSemaphore(void)
	: m_hSemaphore(NULL)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CSemaphore::~CSemaphore(void)
	{
		Close();
	}

	//////////////////////////////////////////////////////////////////////////
	void CSemaphore::Close(void)
	{
		if( NULL == m_hSemaphore )
			return;
		CloseSemaphore(m_hSemaphore);
		m_hSemaphore = NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSemaphore::Release(int nReleaseCount)
	{
		if( NULL == m_hSemaphore )
			return EC_INVALID_HANDLE;
		return ReleaseSemaphore(m_hSemaphore, nReleaseCount);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSemaphore::Wait(DWORD dwTimeOut)
	{
		if( NULL == m_hSemaphore )
			return EC_INVALID_HANDLE;
		return WaitForSemaphore(m_hSemaphore, dwTimeOut);
	}
}