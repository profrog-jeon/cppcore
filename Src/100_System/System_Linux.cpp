#include "stdafx.h"
#include "System_Linux.h"
#include "GlobalSemaphoreMap.h"
#include "Environment.h"
#include "Log.h"
#include <errno.h>
#include <string.h>

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	_ST_SEM_HANDLE::_ST_SEM_HANDLE(void)
	: nType(NAMED_SEM)
	, pSem(SEM_FAILED)
	, hGlobalSem(NULL)
	, nOwnerTID(0)
	, nLockCount(0)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	_ST_SEM_HANDLE::~_ST_SEM_HANDLE(void)
	{
		if( NAMED_SEM == nType )
		{
			if( SEM_FAILED != pSem )
			{
				::sem_close(pSem);
				std::string strName = GlobalSemMap()->Destroy(hGlobalSem);
				if( !strName.empty() )
					::sem_unlink(strName.c_str());
			}
		}
		if( UNNAMED_SEM == nType )
		{
			if( pSem && (SEM_FAILED != pSem) )
			{
				::sem_destroy(pSem);
				delete pSem;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	_ST_EVENT_HANDLE::_ST_EVENT_HANDLE(void)
	: tMutex()
	, tCond()
	, bAutoReset(false)
	, bSignaled(0)
	{
		pthread_mutexattr_t tMutexAttr;
		::pthread_mutexattr_init(&tMutexAttr);
		int nAttrErr = ::pthread_mutexattr_settype(&tMutexAttr, PTHREAD_MUTEX_RECURSIVE);
		if( nAttrErr )
		{
			Log_Debug("pthread_mutexattr_settype(&tMutexAttr, PTHREAD_MUTEX_RECURSIVE_NP) failure, %d, %s", nAttrErr, strerror(nAttrErr));
			::pthread_mutex_init(&tMutex, NULL);	// always returns 0
		}
		else
			::pthread_mutex_init(&tMutex, &tMutexAttr);

		::pthread_mutexattr_destroy(&tMutexAttr);
		::pthread_cond_init(&tCond, NULL);
	}

	//////////////////////////////////////////////////////////////////////////
	_ST_EVENT_HANDLE::~_ST_EVENT_HANDLE(void)
	{
		::pthread_cond_destroy(&tCond);
		::pthread_mutex_destroy(&tMutex);
	}

	//////////////////////////////////////////////////////////////////////////
	CInternal::CInternal(void)
	{
		m_csSemHandleMap = InitializeCriticalSection();
		m_csEventHandleMap = InitializeCriticalSection();
		m_csFilemapHandleSet = InitializeCriticalSection();
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE CInternal::CreateSemHandle(CSemHandle& outSemHandle)
	{
		_ST_SEM_HANDLE* pRet = new(std::nothrow) _ST_SEM_HANDLE;
		if( NULL == pRet )
			return NULL;

		outSemHandle = CSemHandle(pRet);
		EnterCriticalSection(m_csSemHandleMap);
		m_SemHandleMap.insert(std::make_pair((HANDLE)pRet, outSemHandle));
		LeaveCriticalSection(m_csSemHandleMap);
		return (HANDLE)pRet;
	}

	//////////////////////////////////////////////////////////////////////////
	bool CInternal::GetSemHandle(HANDLE hSemaphore, CSemHandle& outSemHandle)
	{
		bool bFound = false;
		EnterCriticalSection(m_csSemHandleMap);
		CSemHandleMapIt iter = m_SemHandleMap.find(hSemaphore);
		if( m_SemHandleMap.end() != iter )
		{
			bFound = true;
			outSemHandle = iter->second;
		}
		LeaveCriticalSection(m_csSemHandleMap);
		return bFound;
	}

	//////////////////////////////////////////////////////////////////////////
	void CInternal::DestroySemHandle(HANDLE hSemaphore)
	{
		EnterCriticalSection(m_csSemHandleMap);
		CSemHandleMapIt iter = m_SemHandleMap.find(hSemaphore);
		if( m_SemHandleMap.end() != iter )
			m_SemHandleMap.erase(iter);
		LeaveCriticalSection(m_csSemHandleMap);
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE CInternal::CreateEventHandle(CEventHandle& outEventHandle)
	{
		_ST_EVENT_HANDLE* pRet = new(std::nothrow) _ST_EVENT_HANDLE;
		if( NULL == pRet )
			return NULL;

		outEventHandle = CEventHandle(pRet);
		EnterCriticalSection(m_csEventHandleMap);
		m_EventHandleMap.insert(std::make_pair((HANDLE)pRet, outEventHandle));
		LeaveCriticalSection(m_csEventHandleMap);
		return (HANDLE)pRet;
	}

	//////////////////////////////////////////////////////////////////////////
	bool CInternal::GetEventHandle(HANDLE hEvent, CEventHandle& outEventHandle)
	{
		bool bFound = false;
		EnterCriticalSection(m_csEventHandleMap);
		CEventHandleMapIt iter = m_EventHandleMap.find(hEvent);
		if( m_EventHandleMap.end() != iter )
		{
			bFound = true;
			outEventHandle = iter->second;
		}
		LeaveCriticalSection(m_csEventHandleMap);
		return bFound;
	}

	//////////////////////////////////////////////////////////////////////////
	void CInternal::DestroyEventHandle(HANDLE hEvent)
	{
		EnterCriticalSection(m_csEventHandleMap);
		CEventHandleMapIt iter = m_EventHandleMap.find(hEvent);
		if( m_EventHandleMap.end() != iter )
			m_EventHandleMap.erase(iter);
		LeaveCriticalSection(m_csEventHandleMap);
	}

	//////////////////////////////////////////////////////////////////////////
	void CInternal::RegisterFilemapHandle(_ST_FILEMAP_HANDLE* pHandle)
	{
		EnterCriticalSection(m_csSemHandleMap);
		m_FilemapHandleSet.insert(pHandle);
		LeaveCriticalSection(m_csSemHandleMap);
	}

	//////////////////////////////////////////////////////////////////////////
	_ST_FILEMAP_HANDLE* CInternal::GetFilemapHandle(HANDLE hFilemap)
	{
		EnterCriticalSection(m_csSemHandleMap);
		CFilemapHandleSetIt iter = m_FilemapHandleSet.find((_ST_FILEMAP_HANDLE*)hFilemap);
		_ST_FILEMAP_HANDLE* pHandle = (m_FilemapHandleSet.end() == iter)? NULL : *iter;
		LeaveCriticalSection(m_csSemHandleMap);
		return pHandle;
	}

	//////////////////////////////////////////////////////////////////////////
	_ST_FILEMAP_HANDLE* CInternal::GetFilemapHandleByKey(DWORD dwKey)
	{
		_ST_FILEMAP_HANDLE* pHandle = NULL;
		EnterCriticalSection(m_csSemHandleMap);
		CFilemapHandleSetIt iter;
		for(iter=m_FilemapHandleSet.begin(); iter!=m_FilemapHandleSet.end(); iter++)
		{
			if( (*iter)->dwKey != dwKey )
				continue;

			pHandle = *iter;
			break;
		}
		LeaveCriticalSection(m_csSemHandleMap);
		return pHandle;
	}

	//////////////////////////////////////////////////////////////////////////
	void CInternal::DestroyFilemapHandle(HANDLE hFilemap)
	{
		EnterCriticalSection(m_csSemHandleMap);
		CFilemapHandleSetIt iter = m_FilemapHandleSet.find((_ST_FILEMAP_HANDLE*)hFilemap);
		if( m_FilemapHandleSet.end() != iter )
		{
			_ST_FILEMAP_HANDLE* pHandle = *iter;
			m_FilemapHandleSet.erase(iter);
			delete pHandle;
		}
		LeaveCriticalSection(m_csSemHandleMap);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CalcDelayedClockTime(DWORD ms, timespec& outTime)
	{
		if( ::clock_gettime(CLOCK_REALTIME, &outTime) < 0 )
		{
			Log_Debug("clock_gettime(CLOCK_REALTIME, &ts) failure, %s", strerror(errno));
			return errno;
		}

		outTime.tv_nsec += (long)(ms % 1000) * (long)1000000;
		outTime.tv_sec += (time_t)(ms / 1000) + ((time_t)outTime.tv_nsec / (time_t)1000000000);
		outTime.tv_nsec = (outTime.tv_nsec % (long)1000000000);
		return EC_SUCCESS;
	}
}