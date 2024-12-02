#include "stdafx.h"
#include "KernelObject.h" 
#include "Log.h"
#undef TEXT
#include <Windows.h>
#include <process.h>
#include "System_Win.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	bool CreatePipe(HANDLE* pOutReadHandle, HANDLE* pOutWriteHandle)
	{
		SECURITY_ATTRIBUTES saAttr;
		saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
		saAttr.bInheritHandle = TRUE;
		saAttr.lpSecurityDescriptor = NULL;
		return ::CreatePipe(pOutReadHandle, pOutWriteHandle, &saAttr, 0) != FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead)
	{
		return FALSE != ::ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, NULL);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten)
	{
		return FALSE != ::WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, NULL);
	}

	//////////////////////////////////////////////////////////////////////////
	int64_t SetFilePointer(HANDLE hFile, int64_t nDistanceToMove, E_FILE_MOVE_METHOD nMoveMethod)
	{
		LARGE_INTEGER liDistanceToMove, liNewPosition;
		liDistanceToMove.QuadPart = nDistanceToMove;
		if( FALSE == ::SetFilePointerEx(hFile, liDistanceToMove, &liNewPosition, nMoveMethod) )
			return -1;

		return liNewPosition.QuadPart;
	}

	//////////////////////////////////////////////////////////////////////////
	QWORD GetFileSize(HANDLE hFile)
	{
		DWORD dwHigh = 0;
		DWORD dwLow = ::GetFileSize(hFile, &dwHigh);
		if( INVALID_FILE_SIZE == dwLow )
		{
			Log_Debug("GetFileSize(0x%08X) failure, ", hFile);
			return 0;
		}
		return MAKEQWORD(dwLow, dwHigh);
	}

	//////////////////////////////////////////////////////////////////////////
	bool GetFileTime(HANDLE hFile, UINT64* pCreationTime, UINT64* pLastAccessTime, UINT64* pLastWriteTime)
	{
		FILETIME ftCreateTime		= { 0, };
		FILETIME ftLastAccessTime	= { 0, };
		FILETIME ftLastWriteTime	= { 0, };

		if( FALSE == ::GetFileTime(hFile, &ftCreateTime, &ftLastAccessTime, &ftLastWriteTime) )
			return false;

		if( pCreationTime )			*pCreationTime = UnixTimeFrom(ftCreateTime		);
		if( pLastAccessTime )		*pLastAccessTime = UnixTimeFrom(ftLastAccessTime	);
		if( pLastWriteTime )		*pLastWriteTime = UnixTimeFrom(ftLastWriteTime	);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	void UnixTimeToFileTime(UINT64 tUnixTime, FILETIME* pft);
	bool SetFileTime(HANDLE hFile, UINT64 tCreationTime, UINT64 tLastAccessTime, UINT64 tLastWriteTime)
	{
		FILETIME ftCreateTime		= { 0, };
		FILETIME ftLastAccessTime	= { 0, };
		FILETIME ftLastWriteTime	= { 0, };

		UnixTimeToFileTime(tCreationTime,		&ftCreateTime		);
		UnixTimeToFileTime(tLastAccessTime, 	&ftLastAccessTime	);
		UnixTimeToFileTime(tLastWriteTime,		&ftLastWriteTime	);

		return FALSE != ::SetFileTime(hFile, 
			tCreationTime?		&ftCreateTime		: NULL, 
			&ftLastAccessTime?	&ftLastAccessTime	: NULL, 
			&ftLastWriteTime?	&ftLastWriteTime	: NULL);
	}

	//////////////////////////////////////////////////////////////////////////
	bool GetFileInformationByHandle(HANDLE hFile, ST_BY_HANDLE_FILE_INFORMATION* lpFileInformation)
	{
		BY_HANDLE_FILE_INFORMATION stInfo;
		if( FALSE == ::GetFileInformationByHandle(hFile, &stInfo) )
			return false;

		lpFileInformation->dwFileAttributes		 = stInfo.dwFileAttributes		;
		lpFileInformation->dwVolumeSerialNumber	 = stInfo.dwVolumeSerialNumber	;
		lpFileInformation->nFileSizeHigh		 = stInfo.nFileSizeHigh			;
		lpFileInformation->nFileSizeLow			 = stInfo.nFileSizeLow			;
		lpFileInformation->nNumberOfLinks		 = stInfo.nNumberOfLinks		;
		lpFileInformation->nFileIndexHigh		 = stInfo.nFileIndexHigh		;
		lpFileInformation->nFileIndexLow		 = stInfo.nFileIndexLow			;
		lpFileInformation->tCreationTime		 = UnixTimeFrom(stInfo.ftCreationTime	)	;
		lpFileInformation->tLastAccessTime		 = UnixTimeFrom(stInfo.ftLastAccessTime	)	;
		lpFileInformation->tLastWriteTime		 = UnixTimeFrom(stInfo.ftLastWriteTime	)	;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	void FlushFileBuffers(HANDLE hFile)
	{
		::FlushFileBuffers(hFile);
	}

	//////////////////////////////////////////////////////////////////////////
	void CloseFile(HANDLE hFile)
	{
		::CloseHandle(hFile);
	}

	//////////////////////////////////////////////////////////////////////////
	struct _ST_CREATE_THREAD_DATA
	{
		int (*pfEntry)(void* pContext);
		void*			pContext;
	};

	//////////////////////////////////////////////////////////////////////////
	static unsigned int WINAPI ThreadCaller(void* pContext)
	{
		_ST_CREATE_THREAD_DATA* pData = (_ST_CREATE_THREAD_DATA*)pContext;
		int nRet = pData->pfEntry(pData->pContext);
		delete pData;
		return nRet;
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE CreateThread(int (*pfEntry)(void* pContext), void* pContext, E_THREAD_PRIORITY nPriority)
	{
		_ST_CREATE_THREAD_DATA* pData = new _ST_CREATE_THREAD_DATA;
		pData->pfEntry	= pfEntry;
		pData->pContext = pContext;

		HANDLE hThread = (HANDLE)::_beginthreadex(NULL, 0, ThreadCaller, pData, 0, NULL);
		if( INVALID_HANDLE_VALUE == hThread )
		{
			Log_Debug("_beginthreadex operation failure");
			delete pData;
			return NULL;
		}

		if( FALSE == core::SetThreadPriority(hThread, nPriority) )
			Log_Debug("SetThreadPriority calling failure, as (%d-)%d(-%d)", THREAD_PRIORITY_BELOW_LOW, nPriority, THREAD_PRIORITY_ABOVE_HIGH);
		return hThread;	
	}

	//////////////////////////////////////////////////////////////////////////
	bool TerminateThread(HANDLE hThread, DWORD dwExitCode)
	{
		return FALSE != ::TerminateThread(hThread, dwExitCode);
	}

	//////////////////////////////////////////////////////////////////////////
	EWAIT WaitForThread(HANDLE hThread, DWORD dwTimeOut, int* pnOutRet/* = NULL*/)
	{
		DWORD dwRet = ::WaitForSingleObject(hThread, dwTimeOut);
		switch(dwRet)
		{
		case WAIT_OBJECT_0:
			break;

		case WAIT_TIMEOUT:
			return WAIT_TIMEOUT_;

		case WAIT_ABANDONED:
			return WAIT_ABANDONED_;

		case WAIT_FAILED:
			return WAIT_FAILED_;

		default:
			return (EWAIT)dwRet;
		}

		if( pnOutRet )
		{
			DWORD dwRet = 0;
			if( ::GetExitCodeThread(hThread, &dwRet) )
				(*pnOutRet) = dwRet;
			else
				Log_Debug("GetExitCodeThread calling failure");
		}

		return WAIT_OBJECT_0_;
	}

	//////////////////////////////////////////////////////////////////////////
	void CloseThreadHandle(HANDLE hThread)
	{
		::CloseHandle(hThread);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE JoinThread(HANDLE hThread, int* pnOutRet/* = NULL*/)
	{
		DWORD dwRet = ::WaitForSingleObject(hThread, INFINITE);
		if( WAIT_OBJECT_0 != dwRet )
			return dwRet;

		if( pnOutRet )
		{
			DWORD dwRet = 0;
			if( ::GetExitCodeThread(hThread, &dwRet) )
				(*pnOutRet) = dwRet;
			else
				Log_Debug("GetExitCodeThread calling failure");
		}
		
		::CloseHandle(hThread);
		return ERROR_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	E_THREAD_PRIORITY GetThreadPriority(HANDLE hThread)
	{
		int nNativePriority = ::GetThreadPriority(hThread);
		switch(nNativePriority)
		{
		case THREAD_PRIORITY_IDLE:
		case THREAD_PRIORITY_LOWEST:		return THREAD_PRIORITY_BELOW_LOW	;
		case THREAD_PRIORITY_BELOW_NORMAL:	return THREAD_PRIORITY_LOW			;
		case THREAD_PRIORITY_NORMAL:		return THREAD_PRIORITY_MIDDLE		;
		case THREAD_PRIORITY_ABOVE_NORMAL:	return THREAD_PRIORITY_HIGH			;
		case THREAD_PRIORITY_HIGHEST:
		case THREAD_PRIORITY_TIME_CRITICAL:	return THREAD_PRIORITY_ABOVE_HIGH	;
		}
		return THREAD_PRIORITY_ERROR;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE TerminateProcess(HANDLE hProcess)
	{
		if( FALSE == ::TerminateProcess(hProcess, -1) )
			return ::GetLastError();
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	EWAIT WaitForProcess(HANDLE hProcess, DWORD dwTimeOut, int* pOutExitCode)
	{
		Log_Info("Wait for process...");
		DWORD dwRet = ::WaitForSingleObject(hProcess, dwTimeOut);
		Log_Info("Wait finished with ret:%u", dwRet);

		switch(dwRet)
		{
		case WAIT_OBJECT_0:
			break;

		case WAIT_TIMEOUT:
			return WAIT_TIMEOUT_;

		case WAIT_ABANDONED:
			return WAIT_ABANDONED_;

		case WAIT_FAILED:
			return WAIT_FAILED_;

		default:
			return (EWAIT)dwRet;
		}

		DWORD dwExitCode = 0xFFFFFFFF;
		if( FALSE == ::GetExitCodeProcess(hProcess, &dwExitCode) )
		{
			Log_Debug("GetExitCodeProcess failure");
			return WAIT_OBJECT_0_;
		}

		if ((DWORD)-128 <= dwExitCode)
			Log_Info("User defined error, %d", (int)dwExitCode);
		else if(0xC0000000 <= dwExitCode)
			Log_Info("Process terminated with SEH exception-code:0x%08X", dwExitCode);
		else if(0x80000000 <= dwExitCode )
			Log_Info("Process terminated with HRESULT rethrown as an SEH:0x%08X", dwExitCode);

		if (pOutExitCode)
			*pOutExitCode = (int)dwExitCode;

		return WAIT_OBJECT_0_;
	}

	//////////////////////////////////////////////////////////////////////////
	void CloseProcessHandle(HANDLE hProcess)
	{
		::CloseHandle(hProcess);
	}

	//////////////////////////////////////////////////////////////////////////
	void ClosePipeHandle(HANDLE hProcess)
	{
		::CloseHandle(hProcess);
	}

	//////////////////////////////////////////////////////////////////////////
	void ExitProcess(int nExitCode)
	{
		::ExitProcess(nExitCode);
	}

	//////////////////////////////////////////////////////////////////////////
	bool SetThreadPriority(HANDLE hThread, E_THREAD_PRIORITY nPriority)
	{
		BOOL bRet = FALSE;

		switch(nPriority)
		{
		case THREAD_PRIORITY_BELOW_LOW:
			bRet = ::SetThreadPriority(hThread, THREAD_PRIORITY_LOWEST);
			break;
		case THREAD_PRIORITY_LOW:
			bRet = ::SetThreadPriority(hThread, THREAD_PRIORITY_BELOW_NORMAL);
			break;
		case THREAD_PRIORITY_MIDDLE:
			bRet = ::SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL);
			break;
		case THREAD_PRIORITY_HIGH:
			bRet = ::SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
			break;
		case THREAD_PRIORITY_ABOVE_HIGH:
			bRet = ::SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
			break;
		}

		return bRet != FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	void CloseMutex(HANDLE hMutex)
	{
		::CloseHandle(hMutex);
	}

	//////////////////////////////////////////////////////////////////////////
	EWAIT WaitForMutex(HANDLE hMutex, DWORD dwTimeOut)
	{
		DWORD dwRet = ::WaitForSingleObject(hMutex, dwTimeOut);
		switch(dwRet)
		{
		case WAIT_OBJECT_0:
			return WAIT_OBJECT_0_;

		case WAIT_TIMEOUT:
			return WAIT_TIMEOUT_;

		case WAIT_ABANDONED:
			return WAIT_ABANDONED_;

		case WAIT_FAILED:
			return WAIT_FAILED_;

		default:
			return (EWAIT)dwRet;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReleaseMutex(HANDLE hMutex)
	{
		return FALSE != ::ReleaseMutex(hMutex);
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE InitializeCriticalSection(void)
	{
		::CRITICAL_SECTION* pCriticalSection = new ::CRITICAL_SECTION;
		::InitializeCriticalSection(pCriticalSection);
		return (HANDLE)pCriticalSection;
	}

	//////////////////////////////////////////////////////////////////////////
	void DeleteCriticalSection(HANDLE hCS)
	{
		::CRITICAL_SECTION* pCriticalSection = (::CRITICAL_SECTION*)hCS;
		::DeleteCriticalSection(pCriticalSection);
		delete pCriticalSection;
	}

	//////////////////////////////////////////////////////////////////////////
	void EnterCriticalSection(HANDLE hCS)
	{
		::CRITICAL_SECTION* pCriticalSection = (::CRITICAL_SECTION*)hCS;
		::EnterCriticalSection(pCriticalSection);
	}

	//////////////////////////////////////////////////////////////////////////
	void LeaveCriticalSection(HANDLE hCS)
	{
		::CRITICAL_SECTION* pCriticalSection = (::CRITICAL_SECTION*)hCS;
		::LeaveCriticalSection(pCriticalSection);
	}

	//////////////////////////////////////////////////////////////////////////
	void DestroyPipe(HANDLE hPipe)
	{
		::CloseHandle(hPipe);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE SendThroughPipe(HANDLE hPipe, const void* pData, size_t tDataSize)
	{
		ECODE	dwRet		= ERROR_SUCCESS;
		try
		{
			DWORD dwWritten	= 0;
			if( FALSE == ::WriteFile(hPipe, pData, (DWORD)tDataSize, &dwWritten, NULL) )
			{
				dwRet = ::GetLastError();
				throw exception_format("WriteFile calling failure, %u", dwRet);
			}

			if( dwWritten != tDataSize )
			{
				dwRet = ERROR_WRITE_FAULT;
				throw exception_format("WriteFile written size mismatched, trysize:%u, written:%u", tDataSize, dwWritten);
			}
		}
		catch(std::exception& e)
		{
			Log_Debug("%s", e.what());
			return dwRet;
		}

		return ERROR_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE RecvFromPipe(HANDLE hPipe, void* pData, size_t tDataSize)
	{
		ECODE nRet = ERROR_SUCCESS;
		try
		{
			//while(1)
			//{
			//	DWORD dwTotalByteAvailable = 0;
			//	if( FALSE == ::PeekNamedPipe(hPipe, NULL, 0, NULL, &dwTotalByteAvailable, NULL) )
			//	{
			//		nRet = GetLastError();
			//		printf("PeekNamedPipe failure, %d", nRet);
			//		if( ERROR_INVALID_HANDLE == nRet )
			//			return nRet;
			//	}

			//	if( dwTotalByteAvailable )
			//		break;

			//	printf("RecvFromPipe waiting...\n");
			//	::WaitForSingleObject(hPipe, 1000);		// for context switch
			//}

			DWORD dwRead = 0;
			if( FALSE == ::ReadFile(hPipe, pData, (DWORD)tDataSize, &dwRead, NULL) )
			{
				nRet = ::GetLastError();
				throw exception_format("ReadFile calling failure, %u", nRet);
			}

			nRet = ERROR_READ_FAULT;
			if( dwRead != tDataSize )
				throw exception_format("ReadFile calling failure, read size mismatched, trysize:%u, read:%u", tDataSize, dwRead);
		}
		catch(std::exception& e)
		{
			Log_Debug("%s", e.what());
			return nRet;
		}

		return ERROR_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE ReleaseSemaphore(HANDLE hSemaphore, int nReleaseCount)
	{
		if( ::ReleaseSemaphore(hSemaphore, nReleaseCount, NULL) )
			return ERROR_SUCCESS;
		return ::GetLastError();
	}

	//////////////////////////////////////////////////////////////////////////
	EWAIT WaitForSemaphore(HANDLE hSemaphore, DWORD dwTimeOut)
	{
		DWORD dwRet = ::WaitForSingleObject(hSemaphore, dwTimeOut);
		switch(dwRet)
		{
		case WAIT_OBJECT_0:
			return WAIT_OBJECT_0_;

		case WAIT_TIMEOUT:
			return WAIT_TIMEOUT_;

		case WAIT_ABANDONED:
			return WAIT_ABANDONED_;

		case WAIT_FAILED:
			return WAIT_FAILED_;

		default:
			return (EWAIT)dwRet;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void CloseSemaphore(HANDLE hSemaphore)
	{
		::CloseHandle(hSemaphore);
	}

	//////////////////////////////////////////////////////////////////////////
	#undef CreateEvent
	HANDLE CreateEvent(bool bManualReset, bool bInitialState)
	{
		return ::CreateEventA(NULL, bManualReset, bInitialState, NULL);
	}

	//////////////////////////////////////////////////////////////////////////
	bool SetEvent(HANDLE hEvent)
	{
		return FALSE != ::SetEvent(hEvent);
	}

	//////////////////////////////////////////////////////////////////////////
	bool PulseEvent(HANDLE hEvent)
	{
		return FALSE != ::PulseEvent(hEvent);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ResetEvent(HANDLE hEvent)
	{
		return FALSE != ::ResetEvent(hEvent);
	}

	//////////////////////////////////////////////////////////////////////////
	EWAIT WaitForEvent(HANDLE hEvent, DWORD dwTimeOut)
	{
		DWORD dwRet = ::WaitForSingleObject(hEvent, dwTimeOut);
		switch(dwRet)
		{
		case WAIT_OBJECT_0:
			return WAIT_OBJECT_0_;

		case WAIT_TIMEOUT:
			return WAIT_TIMEOUT_;

		case WAIT_ABANDONED:
			return WAIT_ABANDONED_;

		case WAIT_FAILED:
			return WAIT_FAILED_;

		default:
			return (EWAIT)dwRet;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void CloseEvent(HANDLE hEvent)
	{
		::CloseHandle(hEvent);
	}

	//////////////////////////////////////////////////////////////////////////
	#undef CreateFileMapping
	HANDLE CreateFileMapping(HANDLE hFile, E_PAGE_PROTECTION nPageProtect, DWORD dwFileMapAccess, QWORD qwMaximumSize, DWORD dwKey)
	{
		HANDLE hRet = ::CreateFileMappingA(hFile == NULL? INVALID_HANDLE_VALUE : hFile
			, NULL, nPageProtect, (qwMaximumSize >> 32) & 0xFFFFFFFF, qwMaximumSize & 0xFFFFFFFF
			, 0 == dwKey? NULL : StringFromA(dwKey).c_str());
		if( NULL == hRet )
			return NULL;

		Internal()->RegisterSharedMemHandle(hRet, dwFileMapAccess);
		return hRet;
	}

	//////////////////////////////////////////////////////////////////////////
	#undef OpenFileMapping
	HANDLE OpenFileMapping(DWORD dwFileMapAccess, DWORD dwKey)
	{
		HANDLE hRet = ::OpenFileMappingA(dwFileMapAccess, FALSE, dwKey == 0? NULL : StringFromA(dwKey).c_str());
		if( NULL == hRet )
			return NULL;

		Internal()->RegisterSharedMemHandle(hRet, dwFileMapAccess);
		return hRet;
	}

	//////////////////////////////////////////////////////////////////////////
	LPVOID MapViewOfFile(HANDLE hFileMapping, QWORD qwOffset, size_t tLength)
	{
		DWORD dwFileMapAccess = 0;
		if( EC_SUCCESS != Internal()->GetSharedMemHandle(hFileMapping, dwFileMapAccess) )
		{
			SetLastError(EC_NO_DATA);
			Log_Debug("GetSharedMemHandle(0x%08X) is not found", hFileMapping);
			return NULL;
		}

		LPVOID pRet = ::MapViewOfFile(hFileMapping, dwFileMapAccess, (qwOffset >> 32) & 0xFFFFFFFF, qwOffset & 0xFFFFFFFF, tLength);
		if( NULL == pRet )
			return NULL;

		return pRet;
	}

	//////////////////////////////////////////////////////////////////////////
	bool FlushViewOfFile(const void* lpBaseAddress, size_t dwNumberOfBytesToFlush)
	{
		return FALSE != ::FlushViewOfFile(lpBaseAddress, dwNumberOfBytesToFlush);
	}

	//////////////////////////////////////////////////////////////////////////
	bool UnmapViewOfFile(HANDLE hFileMapping, LPCVOID lpBaseAddress)
	{
		return FALSE != ::UnmapViewOfFile(lpBaseAddress);
	}

	//////////////////////////////////////////////////////////////////////////
	void CloseFileMappingHandle(HANDLE hFileMapping)
	{
		::CloseHandle(hFileMapping);
		Internal()->UnregisterSharedMemHandle(hFileMapping);
	}
}