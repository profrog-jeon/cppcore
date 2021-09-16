#include "stdafx.h"
#include "System_Mac.h"
#include "GlobalSemaphoreMap.h"
#include "Environment.h"
#include "Log.h"
#include <errno.h>
#include <string.h>
#include <sys/sysctl.h>//#include <sys/sysinfo.h>

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
            if( NULL != Unnamed.Sem )
            {
                for(; Unnamed.nFlag<0; Unnamed.nFlag++)
                    dispatch_semaphore_signal(Unnamed.Sem);
                
                dispatch_release(Unnamed.Sem);
                Unnamed.Sem = NULL;
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
		int nAttrErr = ::pthread_mutexattr_settype(&tMutexAttr, PTHREAD_MUTEX_RECURSIVE_NP);
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

#if defined(__MACH__) && !defined(CLOCK_REALTIME)
#include <sys/time.h>
#define CLOCK_REALTIME 0
    //////////////////////////////////////////////////////////////////////////
    // clock_gettime is not implemented on older versions of OS X (< 10.12).
    // If implemented, CLOCK_REALTIME will have already been defined.
    int clock_gettime(int /*clk_id*/, struct timespec* t)
    {
        struct timeval now;
        int rv = gettimeofday(&now, NULL);
        if (rv) return rv;
        t->tv_sec  = now.tv_sec;
        t->tv_nsec = now.tv_usec * 1000;
        return 0;
    }
#endif
}

//////////////////////////////////////////////////////////////////////////
struct _ST_SEM_TIMEDWAIT_DATA
{
	pthread_mutex_t tMutex;
	pthread_cond_t tCond;
	sem_t* pSem;
	int nRet;

	_ST_SEM_TIMEDWAIT_DATA(void)
	{
		::pthread_mutex_init(&tMutex, NULL);
		::pthread_cond_init(&tCond, NULL);
	}

	~_ST_SEM_TIMEDWAIT_DATA(void)
	{
		int nLastError = errno;
		::pthread_cond_destroy(&tCond);
		::pthread_mutex_destroy(&tMutex);
		errno = nLastError;
	}
};

//////////////////////////////////////////////////////////////////////////
static void* sem_timedwait_worker(void* pContext)
{
	_ST_SEM_TIMEDWAIT_DATA* pData = (_ST_SEM_TIMEDWAIT_DATA*)pContext;
	pData->nRet = ::sem_wait(pData->pSem);

	::pthread_mutex_lock(&pData->tMutex);
	::pthread_cond_signal(&pData->tCond);
	::pthread_mutex_unlock(&pData->tMutex);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout)
{
	int nRet;
	do
	{	// Quick test to see if a lock can be immediately obtained.
		if( 0 == (nRet = sem_trywait(sem)) )
			return 0;
	}   while(nRet < 0 && errno == EINTR);

	_ST_SEM_TIMEDWAIT_DATA stData;
	stData.pSem = sem;

	pthread_t tWorkerThread;
	::pthread_create(&tWorkerThread, NULL, sem_timedwait_worker, &stData);

	::pthread_mutex_lock(&stData.tMutex);
	nRet = pthread_cond_timedwait(&stData.tCond, &stData.tMutex, abs_timeout);
	::pthread_mutex_unlock(&stData.tMutex);

	if( ETIMEDOUT == nRet )
	{
		::pthread_cancel(tWorkerThread);
		::pthread_join(tWorkerThread, NULL);
		::pthread_detach(tWorkerThread);
		errno = ETIMEDOUT;
		return -1;
	}

	::pthread_detach(tWorkerThread);
	return stData.nRet;
}

//////////////////////////////////////////////////////////////////////////
struct _ST_PTHREAD_TIMEDJOIN_DATA
{
	pthread_mutex_t tMutex;
	pthread_cond_t tCond;
	pthread_t tThread;
	void* pThreadExit;
	int nRet;

	_ST_PTHREAD_TIMEDJOIN_DATA(void)
	{
		::pthread_mutex_init(&tMutex, NULL);
		::pthread_cond_init(&tCond, NULL);
	}

	~_ST_PTHREAD_TIMEDJOIN_DATA(void)
	{
		int nLastError = errno;
		::pthread_cond_destroy(&tCond);
		::pthread_mutex_destroy(&tMutex);
		errno = nLastError;
	}
};

//////////////////////////////////////////////////////////////////////////
int get_nprocs()
{
	int nRet = 0;
	size_t tCountLen = sizeof(nRet);
	::sysctlbyname("hw.logicalcpu", &nRet, &tCountLen, NULL, 0);
	return nRet;
}

//////////////////////////////////////////////////////////////////////////
static void* pthread_timedjoin_np_worker(void* pContext)
{
	_ST_PTHREAD_TIMEDJOIN_DATA* pData = (_ST_PTHREAD_TIMEDJOIN_DATA*)pContext;
	pData->nRet = ::pthread_join(pData->tThread, &pData->pThreadExit);

	::pthread_mutex_lock(&pData->tMutex);
	::pthread_cond_signal(&pData->tCond);
	::pthread_mutex_unlock(&pData->tMutex);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
int pthread_timedjoin_np(pthread_t thread, void **retval, const struct timespec *abstime)
{
	_ST_PTHREAD_TIMEDJOIN_DATA stData;
	stData.tThread = thread;

	pthread_t tWorkerThread;
	::pthread_create(&tWorkerThread, NULL, pthread_timedjoin_np_worker, &stData);

	::pthread_mutex_lock(&stData.tMutex);
	int nRet = pthread_cond_timedwait(&stData.tCond, &stData.tMutex, abstime);
	::pthread_mutex_unlock(&stData.tMutex);

	if( ETIMEDOUT == nRet )
	{
		::pthread_cancel(tWorkerThread);
		::pthread_join(tWorkerThread, NULL);
		::pthread_detach(tWorkerThread);
		return nRet;
	}

	if( retval )
		*retval = stData.pThreadExit;
	
	::pthread_detach(tWorkerThread);
	return stData.nRet;
}

