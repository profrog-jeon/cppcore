#include "stdafx.h"
#include "KernelObject.h"
#include "GlobalSemaphoreMap.h"
#ifdef __linux__
#include "System_Linux.h"
#else
#include "System_Mac.h"
#endif
#include "Information.h"
#include "Log.h"
#include "Environment.h"
#include <vector>
#include <cstdlib>
#include <cstring>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <signal.h>
#include <utime.h>
#include <fcntl.h>

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	static struct SIGNPIPE_IGN_INITIALIZATION
	{
		SIGNPIPE_IGN_INITIALIZATION(void)
		{
			::signal(SIGPIPE, SIG_IGN);
		}
	}	g_InitSignalIGN;

	//////////////////////////////////////////////////////////////////////////
	bool CreatePipe(HANDLE* pOutReadHandle, HANDLE* pOutWriteHandle)
	{
		int nFD[2];
		int nRet = pipe(nFD);
		
		// On success, zero is returned.  On error, -1 is returned, errno is set appropriately, and pipefd is left unchanged.
		if( nRet != 0 )
			return false;
		
		*pOutReadHandle = (HANDLE)nFD[0];
		*pOutWriteHandle = (HANDLE)nFD[1];		
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead)
	{
		int nFile = (int)(size_t)hFile;
		ssize_t tRead = ::read(nFile, lpBuffer, nNumberOfBytesToRead);
		if( tRead < 0 )
		{
			Log_Debug("read(%d, size:%u) failure, %d(%s)", nFile, nNumberOfBytesToRead, errno, strerror(errno));
			return false;
		}

		if( lpNumberOfBytesRead )
			*lpNumberOfBytesRead = (DWORD)tRead;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten)
	{
		int nFile = (int)(size_t)hFile;
		ssize_t tWritten = ::write(nFile, lpBuffer, nNumberOfBytesToWrite);
		if( tWritten < 0 )
		{
			Log_Debug("write(%d, size:%u) failure, %d(%s)", nFile, nNumberOfBytesToWrite, errno, strerror(errno));
			return false;
		}

		if( lpNumberOfBytesWritten )
			*lpNumberOfBytesWritten = (DWORD)tWritten;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	int64_t SetFilePointer(HANDLE hFile, int64_t nDistanceToMove, E_FILE_MOVE_METHOD nMoveMethod)
	{
		int nFile = (int)(size_t)hFile;
		int nWhence = SEEK_SET;
		switch(nMoveMethod)
		{
		case FILE_CURRENT_:		nWhence = SEEK_CUR;		break;
		case FILE_END_:			nWhence = SEEK_END;		break;
		case FILE_BEGIN_:
		default:				nWhence = SEEK_SET;		break;
		}

		off_t tNewOffset = ::lseek(nFile, (off_t)nDistanceToMove, nWhence);
		if( tNewOffset < 0 )
		{
			Log_Debug("lseek(%d, %llu, %d) failure, %d(%s)", nFile, nDistanceToMove, nWhence, errno, strerror(errno));
			return -1;
		}

		return (int64_t)tNewOffset;
	}

	//////////////////////////////////////////////////////////////////////////
	QWORD GetFileSize(HANDLE hFile)
	{
		int nFile = (int)(size_t)hFile;
		struct stat stStat = { 0, };
		if( ::fstat(nFile, &stStat) < 0 )
		{
			Log_Debug("fstat(%d) failure, %d(%s).", nFile, errno, strerror(errno));
			return 0;
		}
		return stStat.st_size;
	}

	//////////////////////////////////////////////////////////////////////////
	bool GetFileTime(HANDLE hFile, UINT64* pCreationTime, UINT64* pLastAccessTime, UINT64* pLastWriteTime)
	{
		int nFile = (int)(size_t)hFile;
		struct stat stStat = { 0, };
		if( ::fstat(nFile, &stStat) < 0 )
			return false;

		if( pCreationTime )			*pCreationTime		= stStat.st_ctime;
		if( pLastAccessTime )		*pLastAccessTime	= stStat.st_atime;
		if( pLastWriteTime )		*pLastWriteTime		= stStat.st_mtime;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool SetFileTime(HANDLE hFile, UINT64 tCreationTime, UINT64 tLastAccessTime, UINT64 tLastWriteTime)
	{
		int nFile = (int)(size_t)hFile;
		std::string strFilePath;		
#ifdef __APPLE__
		{
			char szFilePath[PATH_MAX];
			if( fcntl(nFile, F_GETPATH, szFilePath) < 0 )
			{
				Log_Debug("fd -> filename has failed.");
				return false;
			}
			strFilePath = szFilePath;
		}
#endif

#ifdef __linux__
		{
			std::string strLinkPath = Format("/proc/self/fd/%d", nFile);
			char szFileName[1024];
			if( ::readlink(strLinkPath.c_str(), szFileName, 1024) < 0 )
			{
				Log_Debug("readlink(%s) has failed.", strLinkPath.c_str());
				return false;
			}

			strFilePath = szFileName;
		}
#endif

		struct utimbuf TimeBuf = { 0, };
		TimeBuf.actime = (time_t)tLastAccessTime;
		TimeBuf.modtime = (time_t)tLastWriteTime;
		if( ::utime(strFilePath.c_str(), &TimeBuf) < 0 )
			return false;

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	void FlushFileBuffers(HANDLE hFile)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CloseFile(HANDLE hFile)
	{
		::close((int)(size_t)hFile);
	}

	//////////////////////////////////////////////////////////////////////////
	class _CPriorityHelper
	{
		int m_nMin;
		int m_nMax;

	public:
		const int nPolicy;
		_CPriorityHelper(void)
			: nPolicy(SCHED_RR)
			, m_nMin(1)
			, m_nMax(10)
		{
			pthread_attr_t thAttr;
			int nRet = 0;

			pthread_attr_init(&thAttr);
			nRet = pthread_attr_setschedpolicy(&thAttr, nPolicy);
			Log_Debug("pthread_attr_setschedpolicy(%d) ret:%d", nPolicy, nRet);
			pthread_attr_destroy(&thAttr);

			m_nMin = sched_get_priority_min(nPolicy);
			m_nMax = sched_get_priority_max(nPolicy);
			Log_Debug("priority min:%d ~ max:%d", m_nMin, m_nMax);
		}

		E_THREAD_PRIORITY Convert(int nValue)
		{
			nValue = MAX(m_nMin, nValue);
			nValue = MIN(m_nMax, nValue);

			double dRatio = (nValue - m_nMin) / (double)(m_nMax - m_nMin);
			if( dRatio <= 0.2 )	return THREAD_PRIORITY_BELOW_LOW;
			if( dRatio <= 0.4 )	return THREAD_PRIORITY_LOW;
			if( dRatio <= 0.6 )	return THREAD_PRIORITY_MIDDLE;
			if( dRatio <= 0.8 )	return THREAD_PRIORITY_HIGH;
			return THREAD_PRIORITY_ABOVE_HIGH;
		}

		int Convert(E_THREAD_PRIORITY nThreadPriority)
		{
			double dRatio = 0.0;
			switch(nThreadPriority)
			{
			case THREAD_PRIORITY_ABOVE_HIGH:	dRatio = 0.9;	break;
			case THREAD_PRIORITY_HIGH:			dRatio = 0.7;	break;
			case THREAD_PRIORITY_MIDDLE:		dRatio = 0.5;	break;
			case THREAD_PRIORITY_LOW:			dRatio = 0.3;	break;
			case THREAD_PRIORITY_BELOW_LOW:		dRatio = 0.1;	break;
			default:
				dRatio = 0.1;
			}

			int nPriority = (int)((double)(m_nMax - m_nMin) * dRatio + m_nMin);
			return nPriority;
		}
	};

	static _CPriorityHelper g_PriorityHelper;

	//////////////////////////////////////////////////////////////////////////
	struct _ST_THREAD_CALLER_DATA
	{
		int (*pfEntry)(void* pContext);
		void* pContext;
	};

	//////////////////////////////////////////////////////////////////////////
	static void* ThreadCaller(void* pContext)
	{
		_ST_THREAD_CALLER_DATA* pData = (_ST_THREAD_CALLER_DATA*)pContext;
		int nRet = pData->pfEntry(pData->pContext);
		delete pData;
		return (void*)((size_t)nRet);
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE CreateThread(int (*pfEntry)(void* pContext), void* pContext, E_THREAD_PRIORITY nPriority/* = THREAD_PRIORITY_MIDDLE*/)
	{
		_ST_THREAD_CALLER_DATA* pData = NULL;
		pthread_attr_t thAttr;
		bool bthAttrInited = false;
		int nRet = 0;

		try
		{
			pData = new(std::nothrow) _ST_THREAD_CALLER_DATA;
			if( NULL == pData )
				throw exception_format("Memory alloc failure for _ST_THREAD_CALLER_DATA");

			pData->pfEntry = pfEntry;
			pData->pContext = pContext;

			pthread_t tThread;
			pthread_attr_init(&thAttr);
			bthAttrInited = true;

			if( nRet = pthread_attr_setschedpolicy(&thAttr, g_PriorityHelper.nPolicy) )
				Log_Debug("pthread_attr_setschedpolicy(SCHED_RR), ret:%d, %s", nRet, strerror(nRet));

			sched_param param = { 0, };
			param.sched_priority = g_PriorityHelper.Convert(nPriority);

			if( nRet = pthread_attr_setschedparam(&thAttr, &param) )
				Log_Debug("pthread_attr_setschedparam(%d) ret:%d, %s", param.sched_priority, nRet, strerror(nRet));

			if( nRet = pthread_attr_setinheritsched(&thAttr, PTHREAD_EXPLICIT_SCHED) )
				Log_Debug("pthread_attr_setinheritsched(PTHREAD_EXPLICIT_SCHED) ret:%d", nRet);

			if( nRet = pthread_create(&tThread, &thAttr, ThreadCaller, pData) )
			{
				pthread_detach(tThread);
				Log_Debug("pthread_create with priority attr operation failure, ret:%d", nRet);
				if( nRet = pthread_create(&tThread, NULL, ThreadCaller, pData) )
					throw exception_format("pthread_create operation failure, ret:%d", nRet);
			}

			pthread_attr_destroy(&thAttr);

			return (HANDLE)tThread;
		}
		catch(std::exception& e)
		{
			Log_Debug("%s, %s", e.what(), strerror(nRet));
			SAFE_DELETE(pData);
			if( bthAttrInited )
				pthread_attr_destroy(&thAttr);
			return NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool TerminateThread(HANDLE hThread, DWORD dwExitCode)
	{
		return FALSE != ::pthread_cancel((pthread_t)hThread);
	}

	//////////////////////////////////////////////////////////////////////////
	EWAIT WaitForThread(HANDLE hThread, DWORD dwTimeOut, int* pnOutRet)
	{
		int nRet = WAIT_OBJECT_0_;
		try
		{
			timespec tTimeOut;
			nRet = CalcDelayedClockTime(dwTimeOut, tTimeOut);
			if( nRet )
				throw exception_format("CalcDelayedClockTime(%u) failure, %d", dwTimeOut, nRet);

			void* pRet = NULL;
			nRet = ::pthread_timedjoin_np((pthread_t)hThread, &pRet, &tTimeOut);
			if( ETIMEDOUT == nRet )
				return WAIT_TIMEOUT_;

			if( nRet )
				throw exception_format("wait operation has failed, %s", strerror(errno));

			if( pnOutRet )
				*pnOutRet = (int)(size_t)pRet;
		}
		catch(std::exception& e)
		{
			Log_Debug("%s", e.what());
			return WAIT_FAILED_;
		}
		return WAIT_OBJECT_0_;
	}

	//////////////////////////////////////////////////////////////////////////
	void CloseThreadHandle(HANDLE hThread)
	{
		pthread_detach((pthread_t)hThread);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE JoinThread(HANDLE hThread, int* pnOutRet)
	{
		void* pRet = NULL;
		int nRet = pthread_join((pthread_t)hThread, &pRet);
		if( pnOutRet )
			*pnOutRet = (int)(int64_t)pRet;

		return nRet;
	}

	//////////////////////////////////////////////////////////////////////////
	E_THREAD_PRIORITY GetThreadPriority(HANDLE hThread)
	{
		sched_param stParam = { 0 };
		int nPolicy = 0;
		int nRet = pthread_getschedparam((pthread_t)hThread, &nPolicy, &stParam);
		if( nRet )
		{
			Log_Debug("pthread_getschedparam operation failure, %d", nRet);
			return THREAD_PRIORITY_ERROR;
		}

		return g_PriorityHelper.Convert(stParam.sched_priority);
	}

	//////////////////////////////////////////////////////////////////////////
	bool SetThreadPriority(HANDLE hThread, E_THREAD_PRIORITY nPriority)
	{
		sched_param stParam = { 0 };
		stParam.sched_priority = g_PriorityHelper.Convert(nPriority);
		int nRet = pthread_setschedparam((pthread_t)hThread, g_PriorityHelper.nPolicy, &stParam);
		if( nRet )
		{
			Log_Debug("pthread_setschedparam operation failure, %d", nRet);
			return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE TerminateProcess(HANDLE hProcess)
	{
		int nPID = (int)(size_t)hProcess;
		if( ::kill(nPID, SIGKILL) < 0 )
			return errno;
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	struct _ST_WAIT_PROCESS_INFO
	{
		pthread_mutex_t tMutex;
		pthread_cond_t tCond;
		int nPID;
		int nExitCode;
		int nRet;
		int nErrno;

		_ST_WAIT_PROCESS_INFO(void)
		{
			::pthread_mutex_init(&tMutex, NULL);
			::pthread_cond_init(&tCond, NULL);
		}

		~_ST_WAIT_PROCESS_INFO(void)
		{
			::pthread_cond_destroy(&tCond);
			::pthread_mutex_destroy(&tMutex);
		}
	};

	//////////////////////////////////////////////////////////////////////////
	static void* WaitForProcessWorker(void* pContext)
	{
		_ST_WAIT_PROCESS_INFO* pInfo = (_ST_WAIT_PROCESS_INFO*)pContext;

		int nStatus = 0;
		pInfo->nRet = ::waitpid(pInfo->nPID, &nStatus, 0);
		::pthread_mutex_lock(&pInfo->tMutex);
		if( -1 == pInfo->nRet )
			pInfo->nErrno = errno;
		else
		{
			if( 0 == WIFEXITED(nStatus) )
			{
				Log_Warn("process is not well finished, status:%d", nStatus);
				pInfo->nExitCode = nStatus;	// 비정상종료인 경우에는 에러코드를 그대로 반환
			}
			else
				pInfo->nExitCode = (signed char)WEXITSTATUS(nStatus);
			pInfo->nErrno = 0;
		}
		::pthread_cond_signal(&pInfo->tCond);
		::pthread_mutex_unlock(&pInfo->tMutex);
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	EWAIT WaitForProcess(HANDLE hProcess, DWORD dwTimeOut, int* pOutExitCode)
	{
		ECODE nRet = EC_SUCCESS;
		pthread_t tThread = (pthread_t)-1;
		try
		{
			timespec tTimeout;
			nRet = CalcDelayedClockTime(dwTimeOut, tTimeout);
			if( nRet )
				throw exception_format("CalcDelayedClockTime(%u) failure, %d", dwTimeOut, nRet);

			_ST_WAIT_PROCESS_INFO stInfo;
			stInfo.nPID = (int)(size_t)hProcess;
			stInfo.nExitCode = 0;

			if( nRet = ::pthread_create(&tThread, NULL, WaitForProcessWorker, &stInfo) )
				throw exception_format("pthread_create operation failure, ret:%d", nRet);

			::pthread_mutex_lock(&stInfo.tMutex);
			nRet = ::pthread_cond_timedwait(&stInfo.tCond, &stInfo.tMutex, &tTimeout);
			::pthread_mutex_unlock(&stInfo.tMutex);
			if( nRet == ETIMEDOUT )
			{
				::pthread_cancel(tThread);
				::pthread_join(tThread, NULL);
				return WAIT_TIMEOUT_;
			}

			nRet = stInfo.nErrno;
			if( -1 == stInfo.nRet )
				throw exception_format("wait operation has failed, %s", strerror(errno));

			Log_Info("stInfo.nExitCode:%d", stInfo.nExitCode);
			if( pOutExitCode )
				*pOutExitCode = stInfo.nExitCode;

			::pthread_detach(tThread);
		}
		catch(std::exception& e)
		{
			Log_Debug("%s", e.what());
			if( (pthread_t)-1 != tThread )
				::pthread_detach(tThread);

			return WAIT_FAILED_;
		}

		return WAIT_OBJECT_0_;
	}

	//////////////////////////////////////////////////////////////////////////
	void CloseProcessHandle(HANDLE hProcess)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void ClosePipeHandle(HANDLE hPipe)
	{
		::close((int)(size_t)hPipe);
	}

	//////////////////////////////////////////////////////////////////////////
	void ExitProcess(int nExitCode)
	{
		::exit(nExitCode);
	}

	//////////////////////////////////////////////////////////////////////////
	void CloseMutex(HANDLE hMutex)
	{
		Internal()->DestroySemHandle(hMutex);
	}

	//////////////////////////////////////////////////////////////////////////
	EWAIT WaitForMutex(HANDLE hMutex, DWORD dwTimeOut)
	{
		CSemHandle SemHandle;
		if( !Internal()->GetSemHandle(hMutex, SemHandle) )
			return WAIT_FAILED_;
#ifdef __APPLE__
        if( _ST_SEM_HANDLE::UNNAMED_SEM == SemHandle->nType )
        {
            int64_t nNanoSec = (int64_t)dwTimeOut * 1000000;
            if( dispatch_wait(SemHandle->Unnamed.Sem, dispatch_time(DISPATCH_TIME_NOW, nNanoSec)) )
                return WAIT_TIMEOUT_;
			__sync_sub_and_fetch(&SemHandle->Unnamed.nFlag, 1);
            return WAIT_OBJECT_0_;
        }
#endif
		if( GlobalSemMap()->CheckAbandoned(SemHandle->hGlobalSem) )
			return WAIT_ABANDONED_;

		// check lock owner
		if( SemHandle->nOwnerTID == GetCurrentThreadId() && SemHandle->nLockCount > 0 )
		{
			SemHandle->nLockCount++;
			return WAIT_OBJECT_0_;
		}

		int nRet = 0;
		if( dwTimeOut == 0xFFFFFFFF )
		{
			nRet = ::sem_wait(SemHandle->pSem);
			if( nRet < 0 )
			{
				Log_Debug("sem_wait failure, %d", errno);
				return WAIT_FAILED_;
			}
		}
		else
		{
			timespec stTimeout = { 0, };
			nRet = CalcDelayedClockTime(dwTimeOut, stTimeout);
			if( nRet )
			{
				Log_Debug("CalcDelayedClockTime(%u) failure, %d", dwTimeOut, nRet);
				return WAIT_FAILED_;
			}

			nRet = ::sem_timedwait(SemHandle->pSem, &stTimeout);
			if( nRet < 0 )
			{
				if( ETIMEDOUT == errno )
					return WAIT_TIMEOUT_;
				Log_Debug("sem_timedwait failure, %d", errno);
				return WAIT_FAILED_;
			}
		}

		GlobalSemMap()->MarkOwner(SemHandle->hGlobalSem);
		SemHandle->nOwnerTID = GetCurrentThreadId();
		SemHandle->nLockCount = 1;
		return WAIT_OBJECT_0_;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReleaseMutex(HANDLE hMutex)
	{
		CSemHandle SemHandle;
		if( !Internal()->GetSemHandle(hMutex, SemHandle) )
			return false;
#ifdef __APPLE__
        if( _ST_SEM_HANDLE::UNNAMED_SEM == SemHandle->nType )
        {
            dispatch_semaphore_signal(SemHandle->Unnamed.Sem);
            __sync_add_and_fetch(&SemHandle->Unnamed.nFlag, 1);
            return true;
        }
#endif
		if( GlobalSemMap()->CheckAbandoned(SemHandle->hGlobalSem) )
		{
			GlobalSemMap()->UnmarkOwner(SemHandle->hGlobalSem);
			SemHandle->nOwnerTID = 0;
			SemHandle->nLockCount = 0;
			if( ::sem_post(SemHandle->pSem) )
				return false;
			return true;
		}

		if( SemHandle->nOwnerTID != GetCurrentThreadId() )
			return false;

		// check lock owner
		if( SemHandle->nLockCount > 1 )
		{
			SemHandle->nLockCount--;
			return true;
		}

		SemHandle->nLockCount--;
		if( SemHandle->nLockCount <= 0 )
		{
			SemHandle->nOwnerTID = 0;
			SemHandle->nLockCount = 0;
			if( ::sem_post(SemHandle->pSem) )
				return false;
		}

		GlobalSemMap()->UnmarkOwner(SemHandle->hGlobalSem);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE InitializeCriticalSection(void)
	{
		pthread_mutex_t* hMutex = new pthread_mutex_t;

		// building mutex attribute for recursive lock
		pthread_mutexattr_t tMutexAttr;
		pthread_mutexattr_init(&tMutexAttr);
		int nAttrErr = pthread_mutexattr_settype(&tMutexAttr, PTHREAD_MUTEX_RECURSIVE_NP);
		if( nAttrErr )
		{
			Log_Debug("pthread_mutexattr_settype(&tMutexAttr, PTHREAD_MUTEX_RECURSIVE_NP) failure, %d, %s", nAttrErr, strerror(nAttrErr));
			pthread_mutex_init(hMutex, NULL);	// always returns 0
		}
		else
			pthread_mutex_init(hMutex, &tMutexAttr);

		pthread_mutexattr_destroy(&tMutexAttr);
		return hMutex;
	}

	//////////////////////////////////////////////////////////////////////////
	void DeleteCriticalSection(HANDLE hCS)
	{
		pthread_mutex_t* hMutex = (pthread_mutex_t*)hCS;
		pthread_mutex_destroy(hMutex);
		delete hMutex;
	}

	//////////////////////////////////////////////////////////////////////////
	void EnterCriticalSection(HANDLE hCS)
	{
		pthread_mutex_t* hMutex = (pthread_mutex_t*)hCS;
		pthread_mutex_lock(hMutex);
	}

	//////////////////////////////////////////////////////////////////////////
	void LeaveCriticalSection(HANDLE hCS)
	{
		pthread_mutex_unlock((pthread_mutex_t*)hCS);
	}

	//////////////////////////////////////////////////////////////////////////
	void DestroyPipe(HANDLE hPipe)
	{
		int nPipe = (int)(size_t)hPipe;
		//::unlink(pPipeHandle->strPipeName.c_str());
		::close(nPipe);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE SendThroughPipe(HANDLE hPipe, const void* pData, size_t tDataSize)
	{
		int nPipe = (int)(size_t)hPipe;
		try
		{
			int nWritten = ::write(nPipe, pData, tDataSize);
			if( nWritten < 0 )
				throw exception_format("write pipe failure, %d", errno);

			errno = EC_WRITE_FAILURE;
			if( nWritten < (int)tDataSize )
				throw exception_format("Write is not completed!(try:%u, written:%d)", tDataSize, nWritten);
		}
		catch(std::exception& e)
		{
			Log_Debug("%s", e.what());
			return errno;
		}
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE RecvFromPipe(HANDLE hPipe, void* pData, size_t tDataSize)
	{
		int nPipe = (int)(size_t)hPipe;
		fd_set fds;
		timeval stTimeout = { 0, };

		try
		{			
			FD_ZERO(&fds);
			FD_SET(nPipe, &fds);

			stTimeout.tv_sec = 1;
			stTimeout.tv_usec = 0;

			int nRet = 0;
			while (true)
			{
				nRet = ::select(nPipe + 1, &fds, NULL, NULL, &stTimeout);
				if( nRet < 0 )
					throw exception_format("select operation failure, %d", errno);

				nRet = ::read(nPipe, pData, tDataSize);
				if( nRet <= 0 )
				{
					if( EPIPE == errno || EBADF == errno )
					{
					    Log_Info("pipe has been closed.");
						return EC_INVALID_HANDLE;
					}

					// 1(sec) timed out
					if( 0 == nRet )
						continue;

					throw exception_format("pipe reading failure, %d", errno);
				}

				errno = EC_READ_FAILURE;
				if( nRet < (int)tDataSize )
					throw exception_format("Read is not completed!(try:%u, written:%d)", tDataSize, nRet);

				break;
			}
		}
		catch(std::exception &ex)
		{
			Log_Debug("%s", ex.what());
			return errno;
		}
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE ReleaseSemaphore(HANDLE hSemaphore, int nReleaseCount)
	{
		CSemHandle SemHandle;
		if( !Internal()->GetSemHandle(hSemaphore, SemHandle) )
			return EC_INVALID_HANDLE;
#ifdef __linux__
		while(nReleaseCount--)
		{
			if( ::sem_post(SemHandle->pSem) )
				return errno;
		}
		return EC_SUCCESS;
#else
		if( _ST_SEM_HANDLE::UNNAMED_SEM == SemHandle->nType )
		{
			while(nReleaseCount--)
				dispatch_semaphore_signal(SemHandle->Unnamed.Sem);
			__sync_add_and_fetch(&SemHandle->Unnamed.nFlag, nReleaseCount);
			return EC_SUCCESS;
		}

        if( _ST_SEM_HANDLE::NAMED_SEM == SemHandle->nType )
        {
            while(nReleaseCount--)
            {
                if( ::sem_post(SemHandle->pSem) )
                    return errno;
            }
			return EC_SUCCESS;
        }

		return EC_INVALID_HANDLE;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	EWAIT WaitForSemaphore(HANDLE hSemaphore, DWORD dwTimeOut)
	{
		CSemHandle SemHandle;
		if( !Internal()->GetSemHandle(hSemaphore, SemHandle) )
			return WAIT_FAILED_;
#ifdef __APPLE__
        if( _ST_SEM_HANDLE::UNNAMED_SEM == SemHandle->nType )
        {
            int64_t nNanoSec = (int64_t)dwTimeOut * 1000000;
            if( dispatch_wait(SemHandle->Unnamed.Sem, dispatch_time(DISPATCH_TIME_NOW, nNanoSec)) )
                return WAIT_TIMEOUT_;
            __sync_sub_and_fetch(&SemHandle->Unnamed.nFlag, 1);
            return WAIT_OBJECT_0_;
        }
#endif
		int nRet = 0;
		if( dwTimeOut == 0xFFFFFFFF )
		{
			nRet = ::sem_wait(SemHandle->pSem);
			if( nRet < 0 )
			{
				Log_Debug("sem_wait failure, %d", errno);
				return WAIT_FAILED_;
			}
		}
		else
		{
			timespec stTimeout = { 0, };
			nRet = CalcDelayedClockTime(dwTimeOut, stTimeout);
			if( nRet )
			{
				Log_Debug("CalcDelayedClockTime(%u) failure, %d", dwTimeOut, nRet);
				return WAIT_FAILED_;
			}

			nRet = ::sem_timedwait(SemHandle->pSem, &stTimeout);
			if( nRet < 0 )
			{
				if( ETIMEDOUT == errno )
					return WAIT_TIMEOUT_;
				Log_Debug("sem_timedwait failure, %d", errno);
				return WAIT_FAILED_;
			}
		}

		return WAIT_OBJECT_0_;
	}

	//////////////////////////////////////////////////////////////////////////
	void CloseSemaphore(HANDLE hSemaphore)
	{
		Internal()->DestroySemHandle(hSemaphore);
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE CreateEvent(bool bManualReset, bool bInitialState)
	{
		CEventHandle EventHandle;
		HANDLE hRet = Internal()->CreateEventHandle(EventHandle);
		if( NULL == hRet )
			return NULL;
		EventHandle->bAutoReset = !bManualReset;
		EventHandle->bSignaled = bInitialState;
		return hRet;
	}

	//////////////////////////////////////////////////////////////////////////
	bool SetEvent(HANDLE hEvent)
	{
		CEventHandle EventHandle;
		if( !Internal()->GetEventHandle(hEvent, EventHandle) )
			return false;

		pthread_mutex_lock(&EventHandle->tMutex);
		EventHandle->bSignaled = true;
		pthread_cond_signal(&EventHandle->tCond);
		pthread_mutex_unlock(&EventHandle->tMutex);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PulseEvent(HANDLE hEvent)
	{
		CEventHandle EventHandle;
		if( !Internal()->GetEventHandle(hEvent, EventHandle) )
			return false;

		pthread_mutex_lock(&EventHandle->tMutex);
		if( EventHandle->bAutoReset )
			pthread_cond_signal(&EventHandle->tCond);
		else
			pthread_cond_broadcast(&EventHandle->tCond);

		pthread_mutex_unlock(&EventHandle->tMutex);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ResetEvent(HANDLE hEvent)
	{
		CEventHandle EventHandle;
		if( !Internal()->GetEventHandle(hEvent, EventHandle) )
			return false;

		EventHandle->bSignaled = false;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	EWAIT WaitForEvent(HANDLE hEvent, DWORD dwTimeOut)
	{
		CEventHandle EventHandle;
		if( !Internal()->GetEventHandle(hEvent, EventHandle) )
			return WAIT_FAILED_;

		timespec tTimeout;
		ECODE nRet = CalcDelayedClockTime(dwTimeOut, tTimeout);
		if( nRet )
		{
			Log_Debug("CalcDelayedClockTime(%u) failure, %d", dwTimeOut, nRet);
			return WAIT_FAILED_;
		}

		if( EventHandle->bSignaled )
		{
			if( EventHandle->bAutoReset )
				EventHandle->bSignaled = false;
			return WAIT_OBJECT_0_;
		}

		pthread_mutex_lock(&EventHandle->tMutex);
		nRet = pthread_cond_timedwait(&EventHandle->tCond, &EventHandle->tMutex, &tTimeout);
		if( EventHandle->bAutoReset )
			EventHandle->bSignaled = false;
		pthread_mutex_unlock(&EventHandle->tMutex);
		if( nRet == ETIMEDOUT )
			return WAIT_TIMEOUT_;

		if( nRet )
			return WAIT_FAILED_;

		return WAIT_OBJECT_0_;
	}

	//////////////////////////////////////////////////////////////////////////
	void CloseEvent(HANDLE hEvent)
	{
		Internal()->DestroyEventHandle(hEvent);
	}

	//////////////////////////////////////////////////////////////////////////
	static inline HANDLE CreateMemoryMappedFile(int nFile, DWORD dwFileMapAccess, UINT64 qwMaximumSize, DWORD dwKey)
	{
		int nProtectFlag = 0;
		if( FILE_MAP_READ_    & dwFileMapAccess )		nProtectFlag |= PROT_READ;
		if( FILE_MAP_WRITE_   & dwFileMapAccess )		nProtectFlag |= PROT_WRITE;
		if( FILE_MAP_EXECUTE_ & dwFileMapAccess )		nProtectFlag |= PROT_EXEC;

		int nShareFlag = (dwKey == 0)? MAP_PRIVATE : MAP_SHARED;
		LPVOID pRet = ::mmap(NULL, qwMaximumSize, nProtectFlag, nShareFlag, nFile, 0);
		if( MAP_FAILED == pRet )
		{
			Log_Debug("mmap failure, %d(%s)", errno, strerror(errno));
			return NULL;
		}

		_ST_FILEMAP_HANDLE* pHandle = new(std::nothrow) _ST_FILEMAP_HANDLE;
		if( NULL == pHandle )
		{
			SetLastError(EC_NOT_ENOUGH_MEMORY);
			Log_Debug("Internal handle allocation faliure.");
			return NULL;
		}

		pHandle->nType = _ST_FILEMAP_HANDLE::FILEMAP;
		pHandle->dwKey = dwKey;
		pHandle->qwLength = qwMaximumSize;
		pHandle->FileMap.nFile = dup(nFile);
		pHandle->FileMap.pBaseAddress = pRet;
		Internal()->RegisterFilemapHandle(pHandle);
		return (HANDLE)(size_t)pHandle;
	}

	//////////////////////////////////////////////////////////////////////////
	static inline HANDLE CreateSharedMemory(int nAccessPriv, DWORD dwFileMapAccess, UINT64 qwMaximumSize, DWORD dwKey)
	{
		key_t tKey = (dwKey == 0)? IPC_PRIVATE : dwKey;

		int nProtectFlag = 0;
		if( (FILE_MAP_WRITE_ & dwFileMapAccess) == 0 )
		{
			if( FILE_MAP_READ_ & dwFileMapAccess )		nProtectFlag |= SHM_RDONLY;
			if( FILE_MAP_EXECUTE_ & dwFileMapAccess )	nProtectFlag |= SHM_EXEC;
		}

		int nSharedMem = ::shmget(tKey, qwMaximumSize, nAccessPriv);
		if( nSharedMem < 0 )
		{
			Log_Debug("shmget(%u, %u) failure, %d(%s)", tKey, (DWORD)qwMaximumSize, errno, strerror(errno));
			return NULL;
		}

		_ST_FILEMAP_HANDLE* pHandle = new(std::nothrow) _ST_FILEMAP_HANDLE;
		if( NULL == pHandle )
		{
			SetLastError(EC_NOT_ENOUGH_MEMORY);
			Log_Debug("Internal handle allocation faliure.");
			return NULL;
		}

		pHandle->nType = _ST_FILEMAP_HANDLE::SHAREDMEM;
		pHandle->dwKey = dwKey;
		pHandle->qwLength = qwMaximumSize;
		pHandle->SharedMem.nSharedMem = nSharedMem;
		pHandle->SharedMem.nAccessPriv = nAccessPriv;
		pHandle->SharedMem.nProtectFlag = nProtectFlag;
		Internal()->RegisterFilemapHandle(pHandle);
		return (HANDLE)(size_t)pHandle;
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE CreateFileMapping(HANDLE hFile, E_PAGE_PROTECTION nPageProtect, DWORD dwFileMapAccess, QWORD qwMaximumSize, DWORD dwKey)
	{
		if( hFile )
			return CreateMemoryMappedFile((int)(size_t)hFile, dwFileMapAccess, qwMaximumSize, dwKey);

		int nAccessPriv = 0;
		switch(nPageProtect)
		{
		case PAGE_READONLY_			:	nAccessPriv = S_IREAD;				break;
		case PAGE_READWRITE_		:	nAccessPriv = S_IREAD|S_IWRITE;		break;
		case PAGE_WRITECOPY_		:	nAccessPriv = S_IWRITE;				break;
		case PAGE_EXECUTE_			:	nAccessPriv = S_IEXEC;				break;
		case PAGE_EXECUTE_READ_		:	nAccessPriv = S_IEXEC|S_IREAD;		break;
		case PAGE_EXECUTE_READWRITE_:	nAccessPriv = S_IEXEC|S_IREAD|S_IWRITE;		break;
		case PAGE_EXECUTE_WRITECOPY_:	nAccessPriv = S_IEXEC|S_IWRITE;		break;
		case PAGE_NOACCESS_			:
		default:
			nAccessPriv = 0;
			break;
		}
		return CreateSharedMemory(IPC_CREAT|nAccessPriv, dwFileMapAccess, qwMaximumSize, dwKey);
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE OpenFileMapping(DWORD dwFileMapAccess, DWORD dwKey)
	{
		_ST_FILEMAP_HANDLE* pHandle = Internal()->GetFilemapHandleByKey(dwKey);
		if( NULL == pHandle || pHandle->nType == _ST_FILEMAP_HANDLE::SHAREDMEM )
		{
			int nAccessPriv = 0;
			if( FILE_MAP_READ_    & dwFileMapAccess )		nAccessPriv |= S_IREAD;
			if( FILE_MAP_WRITE_   & dwFileMapAccess )		nAccessPriv |= S_IWRITE;
			if( FILE_MAP_EXECUTE_ & dwFileMapAccess )		nAccessPriv |= S_IEXEC;

			return CreateSharedMemory(nAccessPriv, dwFileMapAccess, 0, dwKey);
		}
		
		// _ST_FILEMAP_HANDLE::FILEMAP
		return CreateMemoryMappedFile(pHandle->FileMap.nFile, dwFileMapAccess, pHandle->qwLength, dwKey);
	}

	//////////////////////////////////////////////////////////////////////////
	LPVOID MapViewOfFile(HANDLE hFileMapping, QWORD qwOffset, size_t tLength)
	{
		_ST_FILEMAP_HANDLE* pHandle = Internal()->GetFilemapHandle(hFileMapping);
		if( NULL == pHandle )
		{
			SetLastError(EC_INVALID_HANDLE);
			return NULL;
		}

		if( pHandle->nType == _ST_FILEMAP_HANDLE::SHAREDMEM )
		{
			LPVOID pRet = ::shmat(pHandle->SharedMem.nSharedMem, 0, pHandle->SharedMem.nProtectFlag);
			if( (void*)-1 == pRet )
			{
				Log_Debug("shmat error, %d(%s)", errno, strerror(errno));
				return NULL;
			}

			return pRet;
		}

		// _ST_FILEMAP_HANDLE::FILEMAP
		if( qwOffset + tLength > pHandle->qwLength )
			return NULL;
		return (LPBYTE)pHandle->FileMap.pBaseAddress + qwOffset;
	}

	//////////////////////////////////////////////////////////////////////////
	bool FlushViewOfFile(const void* lpBaseAddress, size_t dwNumberOfBytesToFlush)
	{
		if( ::msync((void*)lpBaseAddress, dwNumberOfBytesToFlush, MS_ASYNC) < 0 )
		{
			Log_Debug("msync failure, %d(%s)", errno, strerror(errno));
			return false;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool UnmapViewOfFile(HANDLE hFileMapping, LPCVOID lpBaseAddress)
	{
		_ST_FILEMAP_HANDLE* pHandle = Internal()->GetFilemapHandle(hFileMapping);
		if( NULL == pHandle )
		{
			SetLastError(EC_INVALID_HANDLE);
			return false;
		}

		if( pHandle->nType == _ST_FILEMAP_HANDLE::SHAREDMEM )
		{
			if( -1 == ::shmdt(lpBaseAddress) )
			{
				Log_Debug("shmdt(0x%08X failure, %d(%s)", lpBaseAddress, errno, strerror(errno));
				return false;
			}
			return true;
		}

		// _ST_FILEMAP_HANDLE::FILEMAP
		QWORD tStartOffset = (QWORD)pHandle->FileMap.pBaseAddress;
		QWORD tEndOffset = tStartOffset + pHandle->qwLength;
		QWORD tAddress = (QWORD)lpBaseAddress;
		if( tAddress < tStartOffset )
			return false;
		if( tAddress >= tEndOffset )
			return false;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	void CloseFileMappingHandle(HANDLE hFileMapping)
	{
		_ST_FILEMAP_HANDLE* pHandle = Internal()->GetFilemapHandle(hFileMapping);
		if( NULL == pHandle )
		{
			SetLastError(EC_INVALID_HANDLE);
			return;
		}

		if( pHandle->nType == _ST_FILEMAP_HANDLE::SHAREDMEM )
		{
			if( -1 == ::shmctl(pHandle->SharedMem.nSharedMem, IPC_RMID, NULL) )
				Log_Debug("shmctl(%d, IPC_RMID, NULL) failure, %d(%s)"
				, pHandle->SharedMem.nSharedMem, errno, strerror(errno));
		}

		if( pHandle->nType == _ST_FILEMAP_HANDLE::FILEMAP )
		{
			if( -1 == ::munmap(pHandle->FileMap.pBaseAddress, pHandle->qwLength) )
				Log_Debug("munmap(0x%08X, %llu) failure, %d(%s)"
				, pHandle->FileMap.pBaseAddress, pHandle->qwLength, errno, strerror(errno));
			if( -1 == ::close(pHandle->FileMap.nFile) )
				Log_Debug("close(%u) failure, %d(%s)"
				, pHandle->FileMap.nFile, errno, strerror(errno));
		}
		Internal()->DestroyFilemapHandle(hFileMapping);
	}
}
