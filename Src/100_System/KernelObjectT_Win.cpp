#include "stdafx.h"
#include "KernelObject.h" 
#include "Log.h"
#undef TEXT
#undef EnumProcesses
#include "System_Win.h"
#include <Windows.h>
#include <process.h>
#include <psapi.h>

#ifdef UNICODE
#define _tpopen		_wpopen
#else
#define _tpopen		_popen
#endif

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	HANDLE CreateFile(LPCTSTR lpFileName, DWORD dwDesiredAccess, E_FILE_DISPOSITION nDisposition, DWORD dwFlagsAndAttributes, DWORD dwMode, HANDLE hTemplateFile)
	{
		HANDLE hFile = ::CreateFile(lpFileName, dwDesiredAccess, FILE_SHARE_WRITE|FILE_SHARE_READ, NULL, (DWORD)nDisposition, dwFlagsAndAttributes, hTemplateFile);
		if( INVALID_HANDLE_VALUE == hFile )
		{
			Log_Error(TEXT("CreateFile(%s) has failed"), lpFileName);
			return NULL;
		}
		return hFile;
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE CreatePipe(LPCTSTR pszName)
	{
		DWORD			dwRet = ERROR_SUCCESS;
		HANDLE			hPipe = INVALID_HANDLE_VALUE;

		try
		{
			std::tstring	strPipeName = Format(TEXT("\\\\.\\pipe\\%s"), pszName);
#ifdef UNICODE
			hPipe = ::CreateNamedPipeW(strPipeName.c_str(),
#else
			hPipe = ::CreateNamedPipeA(strPipeName.c_str(),
#endif
				PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
				PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
				PIPE_UNLIMITED_INSTANCES,
				4096,
				4096,
				NMPWAIT_WAIT_FOREVER,
				NULL);

			if (INVALID_HANDLE_VALUE == hPipe)
				throw exception_format("CreateNamedPipe calling failure");

		}
		catch(std::exception& e)
		{
			Log_Error("%s", e.what());

			if( INVALID_HANDLE_VALUE != hPipe )
				::CloseHandle(hPipe);

			return NULL;
		}

		return hPipe;
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE OpenPipe(LPCTSTR pszName)
	{
		HANDLE hPipe = INVALID_HANDLE_VALUE;

		try
		{
			std::tstring strPipeName = Format(TEXT("\\\\.\\pipe\\%s"), pszName);
			
			hPipe = ::CreateFile(strPipeName.c_str(),
				GENERIC_READ | GENERIC_WRITE, 
				0, 
				NULL,
				OPEN_EXISTING, 
				FILE_ATTRIBUTE_NORMAL, 
				NULL);

			if( INVALID_HANDLE_VALUE == hPipe )
				throw exception_format("CreateFile with pipe name has failed");
		}
		catch(std::exception& e)
		{
			Log_Error("%s", e.what());

			if( INVALID_HANDLE_VALUE != hPipe )
				::CloseHandle(hPipe);

			return NULL;
		}

		return (HANDLE)hPipe;
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE CreateMutex(bool bInitialOwner, LPCTSTR pszName)
	{
		HANDLE hMutex = ::CreateMutex(NULL, bInitialOwner, pszName);
		if( (hMutex) && (ERROR_ALREADY_EXISTS == GetLastError()) )
			::SetLastError(EC_ALREADY_EXISTS);

		return hMutex;
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE OpenMutex(LPCTSTR pszName)
	{
		HANDLE hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, pszName);
		if( NULL == hMutex )
		{
			Log_Error(TEXT("OpenMutex(%s) operation failed, %d"), pszName, GetLastError());
			return NULL;
		}
		return hMutex;
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE CreateSemaphore(int nInitialCount, LPCTSTR pszName)
	{
		HANDLE hSemaphore = ::CreateSemaphore(NULL, nInitialCount, 0x7FFFFFFF, pszName);
		if( NULL == hSemaphore )
		{
			Log_Error("CreateSemaphore(%d, %s) failure.", nInitialCount, pszName);
			return NULL;
		}

		return hSemaphore;
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE OpenSemaphore(LPCTSTR pszName)
	{
		HANDLE hSemaphore = ::OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, pszName);
		if( NULL == hSemaphore )
		{
			Log_Error("OpenSemaphore(%s) failure.", pszName);
			return NULL;
		}

		return hSemaphore;
	}

	//////////////////////////////////////////////////////////////////////////
	static inline HANDLE CreateProcessWorker(LPCTSTR pszFilePath, LPCTSTR pszDirectory, const std::vector<std::tstring>& vecArgs, ST_STARTUPINFO* pStartupInfo, ST_PROCESSINFO* pProcessInfo)
	{
		PROCESS_INFORMATION stProcessInfo = {0,};
		STARTUPINFO stStartupInfo = {0,};
		stStartupInfo.cb = sizeof(stStartupInfo);

		try
		{
			Log_Info("--------------------------");
			Log_Info(TEXT("- FilePath   :%s"), pszFilePath	);
			Log_Info(TEXT("- Directory  :%s"), pszDirectory? pszDirectory : TEXT(""));

			std::tstring strParam = vecArgs.empty()? TEXT("") : vecArgs[0];

			size_t i;
			for(i=1; i<vecArgs.size(); i++)
				strParam += TEXT(" ") + vecArgs[i];
			Log_Info(TEXT("- Param      :%s"), strParam.c_str()		);

			std::tstring strCmdLine = std::tstring(pszFilePath) + TEXT(" ") + strParam;
			Log_Info(TEXT("Try to call CreateProcess. CmdLine:%s"), strCmdLine.c_str());

			// Set the bInheritHandle flag so pipe handles are inherited. 
			SECURITY_ATTRIBUTES saAttr;
			saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
			saAttr.bInheritHandle = TRUE;
			saAttr.lpSecurityDescriptor = NULL;

			// Create a pipe for the child process's STDOUT. 
			HANDLE hStdOutPair[2] = { 0, };
			if( !::CreatePipe(&hStdOutPair[0], &hStdOutPair[1], &saAttr, 0) )
				throw exception_format(TEXT("StdoutRd CreatePipe"));

			// Ensure the read handle to the pipe for STDOUT is not inherited.
			if( !::SetHandleInformation(hStdOutPair[0], HANDLE_FLAG_INHERIT, 0) )
				throw exception_format(TEXT("Stdout SetHandleInformation"));

			// Create a pipe for the child process's STDIN. 
			HANDLE hStdInPair[2] = { 0, };
			if( !::CreatePipe(&hStdInPair[0], &hStdInPair[1], &saAttr, 0) )
				throw exception_format(TEXT("Stdin CreatePipe"));

			// Ensure the write handle to the pipe for STDIN is not inherited. 
			if( !::SetHandleInformation(hStdInPair[1], HANDLE_FLAG_INHERIT, 0) )
				throw exception_format(TEXT("Stdin SetHandleInformation"));

			if( pStartupInfo )
			{
				stStartupInfo.dwFlags |= STARTF_USESTDHANDLES;
				stStartupInfo.hStdInput = hStdInPair[0];
				stStartupInfo.hStdOutput = hStdOutPair[1];
				stStartupInfo.hStdError = hStdOutPair[1];
			}

#ifdef UNICODE
			if( !::CreateProcessW(NULL, (LPTSTR)strCmdLine.c_str(), NULL, NULL, TRUE, 0, NULL, pszDirectory, &stStartupInfo, &stProcessInfo) )
#else
			if( !::CreateProcessA(NULL, (LPTSTR)strCmdLine.c_str(), NULL, NULL, TRUE, 0, NULL, pszDirectory, &stStartupInfo, &stProcessInfo) )
#endif
				throw exception_format(TEXT("CreateProcess calling failure, CmdLine:%s"), strCmdLine.c_str());

			Log_Info("Finished calling CreateProcess.");

			if( pStartupInfo )
			{
				pStartupInfo->hStdInput = hStdInPair[1];
				pStartupInfo->hStdOutput = hStdOutPair[0];
				pStartupInfo->hStdError = hStdOutPair[0];
			}
			else
			{
				::CloseHandle(hStdInPair[0]);
				::CloseHandle(hStdInPair[1]);
				::CloseHandle(hStdOutPair[0]);
				::CloseHandle(hStdOutPair[1]);
			}

			if( pProcessInfo )
			{
				pProcessInfo->dwProcessId = stProcessInfo.dwProcessId;
				pProcessInfo->hProcess = stProcessInfo.hProcess;
				pProcessInfo->dwThreadId = stProcessInfo.dwThreadId;
			}
			::CloseHandle(stProcessInfo.hThread);
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			Log_Info("--------------------------");
			if( stProcessInfo.hProcess )
				::CloseHandle(stProcessInfo.hProcess);
			if( stProcessInfo.hThread )
				::CloseHandle(stProcessInfo.hThread);
			if( stStartupInfo.hStdError )
				::CloseHandle(stStartupInfo.hStdError);
			if( stStartupInfo.hStdInput )
				::CloseHandle(stStartupInfo.hStdInput);
			if( stStartupInfo.hStdOutput )
				::CloseHandle(stStartupInfo.hStdOutput);
			return NULL;
		}
		Log_Info("--------------------------");

		return stProcessInfo.hProcess;
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE CreateProcess(LPCTSTR pszFilePath, LPCTSTR pszDirectory, ST_STARTUPINFO* pStartupInfo, ST_PROCESSINFO* pProcessInfo)
	{
		std::vector<std::tstring> vecArgs;
		return CreateProcessWorker(pszFilePath, pszDirectory, vecArgs, pStartupInfo, pProcessInfo);
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE CreateProcess(LPCTSTR pszFilePath, LPCTSTR pszDirectory, const std::vector<std::tstring>& vecArgs, ST_STARTUPINFO* pStartupInfo, ST_PROCESSINFO* pProcessInfo)
	{
		return CreateProcessWorker(pszFilePath, pszDirectory, vecArgs, pStartupInfo, pProcessInfo);
	}

	//////////////////////////////////////////////////////////////////////////
	int ShellExecuteByPipe(std::tstring strCmdLine, std::tstring& strOutput)
	{
		FILE* pPipe = NULL;
		try
		{
			pPipe = ::_tpopen(strCmdLine.c_str(), TEXT("r"));
			if( NULL == pPipe )
				throw exception_format(TEXT("Execute(%s) has failed."), strCmdLine.c_str());

			const size_t tBuffSize = 64;
			char szBuf[tBuffSize];
			while(::fgets(szBuf, tBuffSize, pPipe))
				strOutput += TCSFromMBS(szBuf);
		}
		catch (std::exception& e)
		{
			Log_Error(TEXT("%s"), e.what());
			if( pPipe )
				::_pclose(pPipe);
			return -1;
		}

		return ::_pclose(pPipe);
	}

	//////////////////////////////////////////////////////////////////////////
	// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
	// and compile with -DPSAPI_VERSION=1
	std::tstring GetProcessName(DWORD processID)
	{
		TCHAR szProcessName[MAX_PATH] = { 0, };

		// Get a handle to the process.
		HANDLE hProcess = ::OpenProcess( PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, processID );

		// Get the process name.
		if (NULL == hProcess )
			return TEXT("");

		HMODULE hMod;
		DWORD cbNeeded;
		if( ::EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded) )
			::GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) );

		::CloseHandle( hProcess );

		return std::tstring(szProcessName);
	}

	//////////////////////////////////////////////////////////////////////////
#ifdef UNICODE
	size_t EnumProcessesW(std::tstring strNamePattern, std::vector<ST_PROCESS_INFO>& vecProcesses)
#else
	size_t EnumProcessesA(std::tstring strNamePattern, std::vector<ST_PROCESS_INFO>& vecProcesses)
#endif
	{
		// Get the list of process identifiers.
		DWORD aProcesses[1024], cbNeeded;
#if (PSAPI_VERSION > 1)
		if ( FALSE == ::K32EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded ) )
			return 0;
#else
		if (FALSE == ::EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
			return 0;
#endif

		// Calculate how many process identifiers were returned.
		DWORD cProcesses = cbNeeded / sizeof(DWORD);

		// Print the name and process identifier for each process.
		DWORD i;
		for(i=0; i<cProcesses; i++)
		{
			if( aProcesses[i] == 0 )
				continue;

			std::tstring strName = GetProcessName(aProcesses[i]);
			if( strName.empty() )
				continue;

			if( !SafeStrCmpWithWildcard(strName.c_str(), strName.length(), strNamePattern.c_str()) )
				continue;

			ST_PROCESS_INFO stProcInfo;
			stProcInfo.tPID = aProcesses[i];
			stProcInfo.strName = strName;
			vecProcesses.push_back(stProcInfo);
		}

		return vecProcesses.size();
	}

}