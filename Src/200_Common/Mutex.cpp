#include "stdafx.h"
#include "Mutex.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CMutex::~CMutex(void)
	{
		if( NULL == m_hMutex )
			return;

		CloseMutex(m_hMutex);
		m_hMutex = NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	void CMutex::Enter(void)
	{
		if( NULL == m_hMutex )
		{
			Log_Error("Enter Mutex, m_hMutex is NULL");
			return;
		}
		EWAIT nRet = WaitForMutex(m_hMutex, INFINITE);
		if( WAIT_OBJECT_0_ != nRet )
			Log_Error("WaitForMutex calling failure, %d", nRet);
	}

	//////////////////////////////////////////////////////////////////////////
	void CMutex::Leave(void)
	{
		if( NULL == m_hMutex )
		{
			Log_Error("Leave Mutex, m_hMutex is NULL");
			return;
		}

		if( !ReleaseMutex(m_hMutex) )
			Log_Error("ReleaseMutex calling failure");
	}

	//////////////////////////////////////////////////////////////////////////
	CMutex::Owner::Owner(CMutex& obj)
		: m_Instance(obj)
	{
		if( NULL == m_Instance.m_hMutex )
		{
			Log_Error("EnterMutex, hMutex is NULL");
			return;
		}
		m_Instance.Enter();
	}

	//////////////////////////////////////////////////////////////////////////
	CMutex::Owner::~Owner(void)
	{
		if( NULL == m_Instance.m_hMutex )
		{
			Log_Error("LeaveMutex, hMutex is NULL");
			return;
		}
		m_Instance.Leave();
	}
}
