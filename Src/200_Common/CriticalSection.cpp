#include "stdafx.h"
#include "CriticalSection.h"
#include <cstring>

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CCriticalSection::CCriticalSection(void)
		: m_hCriticalSection(NULL)
	{
		m_hCriticalSection = InitializeCriticalSection();
		if( NULL == m_hCriticalSection )
			Log_Error("InitializeCriticalSection calling failure");
	}

	//////////////////////////////////////////////////////////////////////////
	CCriticalSection::~CCriticalSection(void)
	{
		if( NULL == m_hCriticalSection )
			return;

		DeleteCriticalSection(m_hCriticalSection);
		m_hCriticalSection = NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	void CCriticalSection::Enter(void)
	{
		if( NULL == m_hCriticalSection )
		{
			Log_Error("Enter CriticalSection, m_hCriticalSection is NULL");
			return;
		}
		EnterCriticalSection(m_hCriticalSection);
	}

	//////////////////////////////////////////////////////////////////////////
	void CCriticalSection::Leave(void)
	{
		if( NULL == m_hCriticalSection )
		{
			Log_Error("Leave CriticalSection, m_hCriticalSection is NULL");
			return;
		}
		LeaveCriticalSection(m_hCriticalSection);
	}

	//////////////////////////////////////////////////////////////////////////
	CCriticalSection::Owner::Owner(CCriticalSection& obj)
		: m_Instance(obj)
	{
		if( NULL == m_Instance.m_hCriticalSection )
		{
			Log_Error("EnterCriticalSection, m_hCriticalSection is NULL");
			return;
		}
		m_Instance.Enter();
	}

	//////////////////////////////////////////////////////////////////////////
	CCriticalSection::Owner::~Owner(void)
	{
		if( NULL == m_Instance.m_hCriticalSection )
		{
			Log_Error("LeaveCriticalSection, m_hCriticalSection is NULL");
			return;
		}
		m_Instance.Leave();
	}
}
