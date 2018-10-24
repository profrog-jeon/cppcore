#include "stdafx.h"
#include "Log.h"
#include "LogInstance.h"
#include "KernelObject.h"
#include "Environment.h"
#include "FileSystem.h"

namespace core
{
	extern ST_LOG_CONTEXT g_stLogContext;

	//////////////////////////////////////////////////////////////////////////
	void InitLog(std::tstring strFilePath, bool bEncrypt, DWORD dwInputFlag)
	{
		std::tstring strPath = ExtractDirectory(strFilePath);
		if( !strPath.empty() )
			CreateDirectory(strPath.c_str());

		g_stLogContext.strOutputPath	= WCSFromTCS(strFilePath);
		g_stLogContext.dwInputFlag		= dwInputFlag;
		g_stLogContext.dwOutputFlag		= bEncrypt? LOG_OUTPUT_ENCFILE : LOG_OUTPUT_FILE;
		g_stLogContext.dwOutputFlag		|= LOG_OUTPUT_DBGWND;
		g_stLogContext.fpCustomOutput	= NULL;
	}

	//////////////////////////////////////////////////////////////////////////	
	void InitLog(const ST_LOG_INIT_PARAM& stParam)
	{
		std::tstring strPath = ExtractDirectory(stParam.strLogPath);
		if( !strPath.empty() )
			CreateDirectory(strPath.c_str());

		g_stLogContext.strOutputPath	= WCSFromTCS(stParam.strLogPath);
		g_stLogContext.strModuleName	= MBSFromTCS(ExtractFileName(stParam.strModulePath));
		g_stLogContext.dwInputFlag		= stParam.dwInputFlag			;
		g_stLogContext.dwOutputFlag		= stParam.dwOutputFlag			;
		g_stLogContext.dwMaxFileSize	= stParam.dwMaxFileSize			;
		g_stLogContext.dwMaxFileCount	= stParam.dwMaxFileCount		;
		g_stLogContext.fpCustomOutput	= stParam.fpCustomOutput		;
	}

	//////////////////////////////////////////////////////////////////////////
	static std::wstring BuildLocalTimeStringW(void)
	{
		ST_SYSTEMTIME stLocalTime;
		GetLocalTime(&stLocalTime);

		std::wstring strRet = Format(L"%04u%02u%02uT%02u%02u%02u_%03u"
			, stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay, stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond, stLocalTime.wMilliseconds);
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	static ECODE EnumLogFile(LPCWSTR pszDir, LPCWSTR pszFilePattern, std::set<std::wstring>& setFiles)
	{
		const std::wstring	strCurPath		= pszDir;
		const std::wstring	strFilePattern	= strCurPath + L"/" + pszFilePattern;

		ST_FILE_FINDDATAW stFindFileData	= { 0, };
		HANDLE hFindFile = FindFirstFileW(strFilePattern.c_str(), &stFindFileData);
		if( NULL == hFindFile )
			return GetLastError();

		do
		{
			if( stFindFileData.bIsDirectory )
				continue;

			std::wstring strFilePath = strCurPath + L"/" + stFindFileData.strFileName;
			setFiles.insert(strFilePath);
		}	while(FindNextFileW(hFindFile, &stFindFileData));
		FindClose(hFindFile);

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	static inline void Internal_FileWorker(std::string strOutputMsgA)
	{
		EnterCriticalSection(g_stLogContext.hFileCS);

		size_t tCurFileSize = 0;
#ifdef _MSC_VER
		FILE* pFile = _wfopen(g_stLogContext.strOutputPath.c_str(), L"a+b");
#else
		FILE* pFile = fopen(MBSFromWCS(g_stLogContext.strOutputPath).c_str(), "a+b");
#endif
		if( pFile )
		{
			fputs(strOutputMsgA.c_str(), pFile);
			tCurFileSize = ::ftell(pFile);
			fclose(pFile);
		}
		else
		{
			wprintf(L"Log file cannot be opened, %u, %s\n", GetLastError(), g_stLogContext.strOutputPath.c_str());
			printf("%s", strOutputMsgA.c_str());
		}

		// Log file rotation
		static int s_nTiming = 0;
		if( ((s_nTiming++ & 0x0F) == 0) && (tCurFileSize > g_stLogContext.dwMaxFileSize) )
		{
			std::wstring strDir = ExtractDirectory(g_stLogContext.strOutputPath);
			std::wstring strFile = ExtractFileNameWithoutExt(g_stLogContext.strOutputPath);
			std::wstring strEXT = ExtractFileExt(g_stLogContext.strOutputPath);
			std::wstring strSystemTime = BuildLocalTimeStringW();
			std::wstring strNewFilename = Format(L"%s/%s_%s.%s", strDir.c_str(), strFile.c_str(), strSystemTime.c_str(), strEXT.c_str());
			MoveFileW(g_stLogContext.strOutputPath.c_str(), strNewFilename.c_str());

			std::wstring strFilePattern = Format(L"%s*.%s", strFile.c_str(), strEXT.c_str());
			std::set<std::wstring> setFiles;
			EnumLogFile(strDir.c_str(), strFilePattern.c_str(), setFiles);

			while(setFiles.size() >= g_stLogContext.dwMaxFileCount)
			{
				DeleteFileW(setFiles.begin()->c_str());
				setFiles.erase(setFiles.begin());
			}
		}

		LeaveCriticalSection(g_stLogContext.hFileCS);
	}

	//////////////////////////////////////////////////////////////////////////
	static inline void Internal_LogWorker(LPCSTR pszPrefix, std::string strLog)
	{
		std::string strOutputMsgA;
		{
			ST_SYSTEMTIME stTime;
			GetLocalTime(&stTime);
			strOutputMsgA = Format("[%04u-%02u-%02u|%02u:%02u:%02u.%03u|p%04u|t%04u|e%04u|%s] %s %s\n"
				, stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds, g_stLogContext.dwProcessID
				, GetCurrentThreadId(), GetLastError(), g_stLogContext.strModuleName.c_str()
				, pszPrefix, strLog.c_str());
		}

		if( g_stLogContext.dwOutputFlag & LOG_OUTPUT_FILE )
			Internal_FileWorker(strOutputMsgA);
		else if( g_stLogContext.dwOutputFlag & LOG_OUTPUT_ENCFILE )
		{
			std::string strEncoded = DefaultEncodeStringA(strOutputMsgA) + "\n";
			Internal_FileWorker(strEncoded);
		}

		if( g_stLogContext.dwOutputFlag & LOG_OUTPUT_CONSOLE )
		{
			if( 0 == SafeStrCmp(pszPrefix, "[ERROR]", 10) )
				fprintf(stderr, "%s %s\n", pszPrefix, strLog.c_str());
			else
				printf("%s %s\n", pszPrefix, strLog.c_str());
		}

		if( g_stLogContext.dwOutputFlag & LOG_OUTPUT_DBGWND )
			OutputDebugStringA("%s", strOutputMsgA.c_str());

		if( g_stLogContext.fpCustomOutput )
			g_stLogContext.fpCustomOutput(pszPrefix, strLog.c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	void Log_FormatV(LPCSTR pszPrefix, const TCHAR* pszFormat, va_list vaList)
	{
		const size_t	tBuffLen = 2048;
		TCHAR			szBuff[tBuffLen];
		SafeSVPrintf(szBuff, tBuffLen, pszFormat, vaList);
#ifdef UNICODE
		Internal_LogWorker(pszPrefix, UTF8FromTCS(szBuff));
#else
		Internal_LogWorker(pszPrefix, szBuff);
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	void Log_Debug(const TCHAR* pszFormat, ...)
	{
		if( 0 == (g_stLogContext.dwInputFlag & LOG_DEBUG) )
			return;

		va_list list;
		va_start(list, pszFormat);
		Log_FormatV("[DEBUG]", pszFormat, list);
		va_end(list);
	}

	//////////////////////////////////////////////////////////////////////////
	void Log_Info(const TCHAR* pszFormat, ...)
	{
		if( 0 == (g_stLogContext.dwInputFlag & LOG_INFO) )
			return;

		va_list list;
		va_start(list, pszFormat);
		Log_FormatV("[INFO]", pszFormat, list);
		va_end(list);
	}

	//////////////////////////////////////////////////////////////////////////
	void Log_Warn(const TCHAR* pszFormat, ...)
	{
		if( 0 == (g_stLogContext.dwInputFlag & LOG_WARN) )
			return;

		va_list list;
		va_start(list, pszFormat);
		Log_FormatV("[WARNING]", pszFormat, list);
		va_end(list);
	}

	//////////////////////////////////////////////////////////////////////////
	void Log_Error(const TCHAR* pszFormat, ...)
	{
		if( 0 == (g_stLogContext.dwInputFlag & LOG_ERROR) )
			return;

		va_list list;
		va_start(list, pszFormat);
		Log_FormatV("[ERROR]", pszFormat, list);
		va_end(list);
	}
}