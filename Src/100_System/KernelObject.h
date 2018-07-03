#pragma once

#include "../__Common/Define.h"
#include "../__Common/Type.h"
#include "../__Common/ErrorCode.h"

#include "Struct.h"

#ifdef UNICODE
#define CreateFile			CreateFileW
#define CreateProcess		CreateProcessW
#define CreateMutex			CreateMutexW
#define OpenMutex			OpenMutexW
#define CreateSemaphore		CreateSemaphoreW
#define OpenSemaphore		OpenSemaphoreW
#else
#define CreateFile			CreateFileA
#define CreateProcess       CreateProcessA
#define CreateMutex			CreateMutexA
#define OpenMutex			OpenMutexA
#define CreateSemaphore		CreateSemaphoreA
#define OpenSemaphore		OpenSemaphoreA
#endif

namespace core
{
	enum E_FILE_DESIRED_ACCESS
	{
		GENERIC_READ_					= 0x80000000,
		GENERIC_WRITE_					= 0x40000000,
		FILE_APPEND_DATA_				= 0x0004
	};

	enum E_FILE_DISPOSITION
	{
		CREATE_NEW_						= 1,
		CREATE_ALWAYS_					= 2,
		OPEN_EXISTING_					= 3,
		OPEN_ALWAYS_					= 4,
		TRUNCATE_EXISTING_				= 5,
	};

	enum E_FILE_ATTRIBUTE
	{
		FILE_ATTRIBUTE_READONLY_			= 0x00000001,
		FILE_ATTRIBUTE_HIDDEN_				= 0x00000002,
		FILE_ATTRIBUTE_SYSTEM_				= 0x00000004,
		FILE_ATTRIBUTE_DIRECTORY_			= 0x00000010,
		FILE_ATTRIBUTE_ARCHIVE_				= 0x00000020,
		FILE_ATTRIBUTE_DEVICE_				= 0x00000040,
		FILE_ATTRIBUTE_NORMAL_				= 0x00000080,
		FILE_ATTRIBUTE_TEMPORARY_			= 0x00000100,
		FILE_ATTRIBUTE_SPARSE_FILE_			= 0x00000200,
		FILE_ATTRIBUTE_REPARSE_POINT_		= 0x00000400,
		FILE_ATTRIBUTE_COMPRESSED_			= 0x00000800,
		FILE_ATTRIBUTE_OFFLINE_				= 0x00001000,
		FILE_ATTRIBUTE_NOT_CONTENT_INDEXED_	= 0x00002000,
		FILE_ATTRIBUTE_ENCRYPTED_			= 0x00004000,
		FILE_ATTRIBUTE_VIRTUAL_				= 0x00010000,
	};

	enum E_PAGE_PROTECTION
	{
		PAGE_NOACCESS_						= 0x01,
		PAGE_READONLY_						= 0x02,
		PAGE_READWRITE_						= 0x04,
		PAGE_WRITECOPY_						= 0x08,
		PAGE_EXECUTE_						= 0x10,
		PAGE_EXECUTE_READ_					= 0x20,
		PAGE_EXECUTE_READWRITE_				= 0x40,
		PAGE_EXECUTE_WRITECOPY_				= 0x80,
	};

	enum E_FILE_MAP_ACCESS
	{
		FILE_MAP_COPY_       				= 0x0001,
		FILE_MAP_WRITE_      				= 0x0002,
		FILE_MAP_READ_       				= 0x0004,
		FILE_MAP_EXECUTE_    				= 0x0020,
		FILE_MAP_ALL_ACCESS_ 				= 0xFFFF,
	};

	enum E_FILE_MOVE_METHOD
	{
		FILE_BEGIN_							= 0,
		FILE_CURRENT_						= 1,
		FILE_END_							= 2,
	};

	enum EWAIT
	{
		WAIT_OBJECT_0_	= EC_SUCCESS	,
		WAIT_TIMEOUT_	= EC_TIMEOUT	,
		WAIT_ABANDONED_ = EC_ABANDONED	,
		WAIT_FAILED_	= -1			,
	};

	HANDLE CreateFileA(const char* lpFileName, DWORD dwDesiredAccess, E_FILE_DISPOSITION nDisposition, DWORD dwAttributes, DWORD dwMode = 0777, HANDLE hTemplateFile = NULL);
	HANDLE CreateFileW(const wchar_t* lpFileName, DWORD dwDesiredAccess, E_FILE_DISPOSITION nDisposition, DWORD dwAttributes, DWORD dwMode = 0777, HANDLE hTemplateFile = NULL);
	bool ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead);
	bool WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten);
	int64_t SetFilePointer(HANDLE hFile, int64_t nDistanceToMove, E_FILE_MOVE_METHOD nMoveMethod);
	QWORD GetFileSize(HANDLE hFile);
	bool GetFileTime(HANDLE hFile, UINT64* pCreationTime, UINT64* pLastAccessTime, UINT64* pLastWriteTime);
	bool SetFileTime(HANDLE hFile, UINT64 tCreationTime, UINT64 tLastAccessTime, UINT64 tLastWriteTime);
	void CloseFile(HANDLE hFile);

	HANDLE	CreateProcessA(const char* pszFilePath, const char* pszDirectory = NULL, ST_STARTUPINFO* pStartupInfo = NULL, ST_PROCESSINFO* pProcessInfo = NULL);
	HANDLE	CreateProcessW(const wchar_t* pszFilePath, const wchar_t* pszDirectory = NULL, ST_STARTUPINFO* pStartupInfo = NULL, ST_PROCESSINFO* pProcessInfo = NULL);
	HANDLE	CreateProcessA(const char* pszFilePath, const char* pszDirectory, const std::vector<std::string>& vecArgs, ST_STARTUPINFO* pStartupInfo = NULL, ST_PROCESSINFO* pProcessInfo = NULL);
	HANDLE	CreateProcessW(const wchar_t* pszFilePath, const wchar_t* pszDirectory, const std::vector<std::wstring>& vecArgs, ST_STARTUPINFO* pStartupInfo = NULL, ST_PROCESSINFO* pProcessInfo = NULL);
	ECODE	TerminateProcess(HANDLE hProcess);
	EWAIT	WaitForProcess(HANDLE hProcess, DWORD dwTimeOut, int* pOutExitCode);
	void	CloseProcessHandle(HANDLE hProcess);
	void	ClosePipeHandle(HANDLE hProcess);
	void	ExitProcess(int nExitCode);

	int		ShellExecuteByPipe(std::string strCmdLine, std::string& strOutput);
	int		ShellExecuteByPipe(std::wstring strCmdLine, std::wstring& strOutput);

	HANDLE	CreateThread(int (*pfEntry)(void* pContext), void* pContext, E_THREAD_PRIORITY nPriority = THREAD_PRIORITY_MIDDLE);
	bool	TerminateThread(HANDLE hThread, DWORD dwExitCode);
	EWAIT	WaitForThread(HANDLE hThread, DWORD dwTimeOut, int* pnOutRet = NULL);
	void	CloseThreadHandle(HANDLE hThread);
	ECODE	JoinThread(HANDLE hThread, int* pnOutRet = NULL);
	E_THREAD_PRIORITY GetThreadPriority(HANDLE hThread);
	bool	SetThreadPriority(HANDLE hThread, E_THREAD_PRIORITY nPriority);

	HANDLE	CreateMutexA(bool bInitialOwner, const char* pszName);
	HANDLE	CreateMutexW(bool bInitialOwner, const wchar_t* pszName);
	HANDLE	OpenMutexA(const char* pszName);
	HANDLE	OpenMutexW(const wchar_t* pszName);
	EWAIT	WaitForMutex(HANDLE hMutex, DWORD dwTimeOut);
	bool	ReleaseMutex(HANDLE hMutex);
	void	CloseMutex(HANDLE hMutex);

	HANDLE	InitializeCriticalSection(void);
	void	DeleteCriticalSection(HANDLE hCS);
	void	EnterCriticalSection(HANDLE hCS);
	void	LeaveCriticalSection(HANDLE hCS);

	HANDLE	CreatePipe(const char* pszName);
	HANDLE	CreatePipe(const wchar_t* pszName);
	HANDLE	OpenPipe(const char* pszName);
	HANDLE	OpenPipe(const wchar_t* pszName);
	void	DestroyPipe(HANDLE hPipe);

	ECODE	SendThroughPipe(HANDLE hPipe, const void* pData, size_t tDataSize);
	ECODE	RecvFromPipe(HANDLE hPipe, void* pData, size_t tDataSize);

	HANDLE	CreateSemaphoreA(int nInitialCount, const char* pszName = NULL);
	HANDLE	CreateSemaphoreW(int nInitialCount, const wchar_t* pszName = NULL);
	HANDLE	OpenSemaphoreA(const char* pszName);
	HANDLE	OpenSemaphoreW(const wchar_t* pszName);
	ECODE	ReleaseSemaphore(HANDLE hSemaphore, int nReleaseCount = 1);
	EWAIT	WaitForSemaphore(HANDLE hSemaphore, DWORD dwTimeOut);	// EC_TIMEOUT
	void	CloseSemaphore(HANDLE hSemaphore);

	#undef CreateEvent
	HANDLE CreateEvent(bool bManualReset, bool bInitialState);
	bool SetEvent(HANDLE hEvent);
	bool PulseEvent(HANDLE hEvent);
	bool ResetEvent(HANDLE hEvent);
	EWAIT WaitForEvent(HANDLE hEvent, DWORD dwTimeOut);
	void CloseEvent(HANDLE hEvent);

	// hFile:NULL -> Shared Memory
	#undef CreateFileMapping
	HANDLE CreateFileMapping(HANDLE hFile, E_PAGE_PROTECTION nPageProtect, DWORD dwFileMapAccess, QWORD qwMaximumSize, DWORD dwKey = 0);
	#undef OpenFileMapping
	HANDLE OpenFileMapping(DWORD dwFileMapAccess, DWORD dwKey);
	LPVOID MapViewOfFile(HANDLE hFileMapping, QWORD qwOffset, size_t tLength);
	bool FlushViewOfFile(const void* lpBaseAddress, size_t dwNumberOfBytesToFlush);
	bool UnmapViewOfFile(HANDLE hFileMapping, LPCVOID lpBaseAddress);
	void CloseFileMappingHandle(HANDLE hFileMapping);

	#undef EnumProcesses
	size_t EnumProcesses(std::string strNamePattern, std::vector<ST_PROCESS_INFOA>& vecProcesses);
	size_t EnumProcesses(std::wstring strNamePattern, std::vector<ST_PROCESS_INFOW>& vecProcesses);
}
