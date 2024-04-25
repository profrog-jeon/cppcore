#include "stdafx.h"
#include "KernelObject.h" 
#include "Log.h"
#include "FileSystem.h"
#include "Utility.h"
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
			DWORD dwErr = ::GetLastError();
			if (ERROR_PATH_NOT_FOUND != dwErr)
				return NULL;

			std::tstring strDir = ExtractDirectory(lpFileName);
			std::tstring strFileName = ExtractFileName(lpFileName);
			CCurrentDirectorySettter stWorkingDir(strDir);

			hFile = ::CreateFile(strFileName.c_str(), dwDesiredAccess, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, (DWORD)nDisposition, dwFlagsAndAttributes, hTemplateFile);
			if (INVALID_HANDLE_VALUE == hFile)
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
			Log_Debug("%s", e.what());

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
			Log_Debug("%s", e.what());

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
			Log_Debug(TEXT("OpenMutex(%s) operation failed, %d"), pszName, GetLastError());
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
			Log_Debug("CreateSemaphore(%d, %s) failure.", nInitialCount, pszName);
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
			Log_Debug("OpenSemaphore(%s) failure.", pszName);
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

		// This option is reserved for future, currently DO NOT apply for GUI-programs
		//stStartupInfo.dwFlags = STARTF_USESHOWWINDOW;
		//stStartupInfo.wShowWindow = SW_HIDE;

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

			if (pStartupInfo && 
				(pStartupInfo->hStdInput || pStartupInfo->hStdOutput || pStartupInfo->hStdError) )
			{
				stStartupInfo.dwFlags |= STARTF_USESTDHANDLES;
				stStartupInfo.hStdInput = pStartupInfo->hStdInput;
				stStartupInfo.hStdOutput = pStartupInfo->hStdOutput;
				stStartupInfo.hStdError = pStartupInfo->hStdError;
			}

#ifdef UNICODE
			if( !::CreateProcessW(NULL, (LPTSTR)strCmdLine.c_str(), NULL, NULL, TRUE, 0, NULL, pszDirectory, &stStartupInfo, &stProcessInfo) )
#else
			if( !::CreateProcessA(NULL, (LPTSTR)strCmdLine.c_str(), NULL, NULL, TRUE, 0, NULL, pszDirectory, &stStartupInfo, &stProcessInfo) )
#endif
				throw exception_format(TEXT("CreateProcess calling failure, CmdLine:%s, %d"), strCmdLine.c_str(), GetLastError());

			Log_Info("Finished calling CreateProcess.");

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
	// Deprecated, CANNOT hide console window
	// But open unofficially for legacy.
	int ShellExecuteByPipe_Old(std::tstring strCmdLine, std::string& strOutput)
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
				strOutput += szBuf;
		}
		catch (std::exception& e)
		{
			Log_Debug(TEXT("%s"), e.what());
			if( pPipe )
				::_pclose(pPipe);
			return -1;
		}

		return ::_pclose(pPipe);
	}

	//////////////////////////////////////////////////////////////////////////
	static void ReadContextFromPipe(HANDLE hStdOut, std::string& strOutput)
	{
		DWORD dwAvail = 0;
		while (::PeekNamedPipe(hStdOut, NULL, 0, NULL, &dwAvail, NULL))
		{
			if (0 == dwAvail)
				break;

			const DWORD dwBuffSize = 32;
			char szBuffer[dwBuffSize + 1];

			DWORD dwReadSize = 0;
			if (!ReadFile(hStdOut, szBuffer, MIN(dwAvail, dwBuffSize), &dwReadSize))
				break;

			szBuffer[dwReadSize] = 0;
			size_t tPreSize = strOutput.length();
			strOutput.resize(tPreSize + dwReadSize);
			memcpy((LPBYTE)strOutput.c_str() + tPreSize, szBuffer, dwReadSize);

			printf("%s", szBuffer);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	static ECODE ShellExecuteByPipeWorker(std::tstring strCmdLine, std::string& strOutput, DWORD& dwExitCode)
	{
		PROCESS_INFORMATION stProcessInfo = { 0, };
		HANDLE hStdOutPair[2] = { 0, };
		try
		{
			// Set the bInheritHandle flag so pipe handles are inherited. 
			SECURITY_ATTRIBUTES saAttr;
			saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
			saAttr.bInheritHandle = TRUE;
			saAttr.lpSecurityDescriptor = NULL;

			// Create a pipe for the child process's STDOUT. 			
			if (!::CreatePipe(&hStdOutPair[0], &hStdOutPair[1], &saAttr, 0))
				throw exception_format(TEXT("hStdOutPair CreatePipe"));

			// Ensure the read handle to the pipe for STDOUT is not inherited.
			if (!::SetHandleInformation(hStdOutPair[0], HANDLE_FLAG_INHERIT, 0))
				throw exception_format(TEXT("Stdout SetHandleInformation"));

			STARTUPINFO stStartupInfo = { 0, };
			stStartupInfo.cb = sizeof(stStartupInfo);
			stStartupInfo.dwFlags = STARTF_USESHOWWINDOW;
			stStartupInfo.wShowWindow = SW_HIDE;
			stStartupInfo.dwFlags |= STARTF_USESTDHANDLES;
			stStartupInfo.hStdInput = NULL;
			stStartupInfo.hStdOutput = hStdOutPair[1];
			stStartupInfo.hStdError = hStdOutPair[1];

			if (!::CreateProcess(NULL, (LPTSTR)strCmdLine.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &stStartupInfo, &stProcessInfo))
				throw exception_format(TEXT("Failed to CreateProcess(%s)"), strCmdLine.c_str());

			while (WAIT_TIMEOUT == ::WaitForSingleObject(stProcessInfo.hProcess, 100))
				ReadContextFromPipe(hStdOutPair[0], strOutput);
			ReadContextFromPipe(hStdOutPair[0], strOutput);

			if (!GetExitCodeProcess(stProcessInfo.hProcess, &dwExitCode))
				Log_Warn(TEXT("GetExitCodeProcess failure."));

			CloseProcessHandle(stProcessInfo.hProcess);
			CloseProcessHandle(stProcessInfo.hThread);
			ClosePipeHandle(hStdOutPair[0]);
			ClosePipeHandle(hStdOutPair[1]);
		}
		catch (std::exception & e)
		{
			Log_Debug("%s", e.what());
			if(stProcessInfo.hProcess)
				CloseProcessHandle(stProcessInfo.hProcess);
			if (stProcessInfo.hThread)
				CloseProcessHandle(stProcessInfo.hThread);
			if(hStdOutPair[0])
				ClosePipeHandle(hStdOutPair[0]);
			if (hStdOutPair[1])
				ClosePipeHandle(hStdOutPair[1]);
			return EC_INTERNAL_ERROR;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	int ShellExecuteByPipe(std::tstring strCmdLine, std::string& strOutput)
	{
		DWORD dwExitCode = 0;
		ECODE nRet = ShellExecuteByPipeWorker(strCmdLine, strOutput, dwExitCode);
		if (EC_SUCCESS != nRet)
			return ShellExecuteByPipe_Old(strCmdLine, strOutput);

		return (int)dwExitCode;
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
			stProcInfo.dwPID = aProcesses[i];
			stProcInfo.strName = strName;
			vecProcesses.push_back(stProcInfo);
		}

		return vecProcesses.size();
	}

}