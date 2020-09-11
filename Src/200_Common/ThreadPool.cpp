#include "stdafx.h"
#include "ThreadPool.h"

namespace core
{
	using namespace internal;

	//////////////////////////////////////////////////////////////////////////
	CThreadPool::CThreadPool()
		: m_vecInstance()
		, m_queReady()
		, m_bEscapeFlag(false)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CThreadPool::~CThreadPool()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	int ThreadPoolWorker(void* pContext);
	ECODE CThreadPool::Init(size_t tPoolCount, E_THREAD_PRIORITY nPriority)
	{
		try
		{
			m_bEscapeFlag = false;
			m_vecInstance.resize(tPoolCount);

			size_t i;
			for (i = 0; i < m_vecInstance.size(); i++)
			{
				m_vecInstance[i].strName = Format(TEXT("Thread_#%u"), i);
				m_vecInstance[i].pOwner = this;
				m_vecInstance[i].hEvent = CreateEvent(false, false);
				m_vecInstance[i].pbEscapeFlag = &m_bEscapeFlag;

				m_vecInstance[i].hThread = core::CreateThread(ThreadPoolWorker, &m_vecInstance[i]);
				if (NULL == m_vecInstance[i].hThread)
					throw exception_format(TEXT("%s creating failed"), m_vecInstance[i].strName.c_str());

				m_queReady.Push(&m_vecInstance[i]);
			}

			Log_Info(TEXT("Thread pool count:%u created."), tPoolCount);
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			Destroy();
			return EC_INTERNAL_ERROR;
		}
		
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	void CThreadPool::Destroy(void)
	{
		m_bEscapeFlag = true;

		size_t i;
		for (i = 0; i < m_vecInstance.size(); i++)
		{
			ST_THREADPOOL_DATA& refInstance = m_vecInstance[i];
			if (refInstance.hThread)
			{
				SetEvent(refInstance.hEvent);
				
				int nErrCode = 0;
				core::JoinThread(refInstance.hThread, &nErrCode);
				refInstance.hThread = NULL;
			}

			if (refInstance.hEvent)
			{
				CloseEvent(refInstance.hEvent);
				refInstance.hEvent = NULL;
			}
		}
		m_vecInstance.clear();
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CThreadPool::CreateThread(int(*pfEntry)(void *pContext), void * pContext)
	{
		ST_THREADPOOL_DATA* pThreadData = NULL;
		ECODE nRet = m_queReady.Pop(pThreadData);
		if (EC_SUCCESS != nRet)
		{
			static int nRejectCount = 0;
			Log_Error("No more thread... try count:%d", InterlockedIncrement_(&nRejectCount));
			return nRet;
		}

		pThreadData->pfThreadFunc = pfEntry;
		pThreadData->pContext = pContext;
		SetEvent(pThreadData->hEvent);
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	void CThreadPool::ReturnThread(HTHREADPOOL hThread)
	{
		m_queReady.Push(hThread);
	}

	//////////////////////////////////////////////////////////////////////////
	int ThreadPoolWorker(void* pContext)
	{
		ST_THREADPOOL_DATA* pThreadData = (ST_THREADPOOL_DATA*)pContext;
		Log_Info(TEXT("%s started."), pThreadData->strName.c_str());

		while (!*pThreadData->pbEscapeFlag)
		{
			EWAIT nRet = WaitForEvent(pThreadData->hEvent, INFINITE);
			if (WAIT_OBJECT_0_ != nRet)
				break;

			if (NULL == pThreadData->pfThreadFunc)
				break;

			int nExitCode = pThreadData->pfThreadFunc(pThreadData->pContext);
			Log_Info(TEXT("%s finished with exit-code:%d"), pThreadData->strName.c_str(), nExitCode);

			pThreadData->pfThreadFunc = NULL;
			pThreadData->pContext = NULL;
			pThreadData->pOwner->ReturnThread(pThreadData);
		}

		Log_Info(TEXT("%s escaped."), pThreadData->strName.c_str());
		return 0;
	}
}
