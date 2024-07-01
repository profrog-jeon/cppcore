#include "stdafx.h"
#include "LogWorker.h"
#include "Log.h"
#include "KernelObject.h"
#include "Environment.h"
#include "FileSystem.h"
#include "InterlockedOperation.h"

namespace core
{
	namespace internal
	{
		//////////////////////////////////////////////////////////////////////////
		ST_LOG_CONTEXT::ST_LOG_CONTEXT(void)
			: hFileCS(NULL), dwProcessID(0), dwInputFlag(LOG_DEFAULT), dwOutputFlag(LOG_OUTPUT_CONSOLE | LOG_OUTPUT_DBGWND), dwMaxFileSize(10 * 1000 * 1000), dwMaxFileCount(3)
			, nRotationType(LOG_ROTATION_SIZE), strOutputFile(), strID(), fpCustomOutput(NULL)
		{
			dwProcessID = GetCurrentProcessId();
			strID = ExtractFileName(GetFileNameA());

			GetLocalTime(&stLastCheckTime);
		}

		//////////////////////////////////////////////////////////////////////////
		ST_LOG_CONTEXT::~ST_LOG_CONTEXT(void)
		{
		}

		//////////////////////////////////////////////////////////////////////////
		CGlobalLogContext::CGlobalLogContext(void)
			: ST_LOG_CONTEXT()
		{
			// -----------------------------------------------------------------------
			// 보기 드문 긴 주석 하나 달겠음. by profrog
			// -----------------------------------------------------------------------
			// 이 함수가 불리기 전에 글로벌 로그 컨텍스트에서 hFileCS는 NULL인 채로 유지됨
			//
			// 1) 어짜피 디폴트 설정은 파일 출력이 없으므로 문제 될 것이 없고,
			// 2) 파일 출력하기 위해서는 이 함수를 반드시 호출해야 하므로
			//
			// 따라서 이 시점에 FileCS 핸들을 생성하더라도 문제없음
			// 또한, 글로벌 로그 컨텍스트에서 생성한 CS 객체는 파괴하지 않을 것임
			// 프로세스 종료 시점에 사용하는 로그 메시지로부터 crash 방지 목적
			// -----------------------------------------------------------------------
			hFileCS = InitializeCriticalSection();
		}

		//////////////////////////////////////////////////////////////////////////
		CGlobalLogContext::~CGlobalLogContext(void)
		{
		}

		//////////////////////////////////////////////////////////////////////////
		static inline std::wstring BuildLocalTimeStringW(void)
		{
			ST_SYSTEMTIME stLocalTime;
			GetLocalTime(&stLocalTime);

			std::wstring strRet = Format(L"%04u%02u%02uT%02u%02u%02u_%03u"
				, stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay, stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond, stLocalTime.wMilliseconds);
			return strRet;
		}

		//////////////////////////////////////////////////////////////////////////
		inline static ECODE GrepLogFile(LPCWSTR pszDir, LPCWSTR pszFilePattern, std::set<std::wstring>& setFiles)
		{
			const std::wstring	strCurPath = pszDir;
			const std::wstring	strFilePattern = strCurPath + L"/" + pszFilePattern;

			ST_FILE_FINDDATAW stFindFileData = { 0, };
			HANDLE hFindFile = FindFirstFileW(strFilePattern.c_str(), &stFindFileData);
			if (NULL == hFindFile)
				return GetLastError();

			do
			{
				if (stFindFileData.bIsDirectory)
					continue;

				std::wstring strFilePath = strCurPath + L"/" + stFindFileData.strFileName;
				setFiles.insert(strFilePath);
			} while (FindNextFileW(hFindFile, &stFindFileData));
			FindClose(hFindFile);

			return EC_SUCCESS;
		}

		//////////////////////////////////////////////////////////////////////////
		inline static bool IsSameDay(const ST_SYSTEMTIME& stOldTime, const ST_SYSTEMTIME& stNewTime)
		{
			return (stOldTime.wDay == stNewTime.wDay);
		}

		//////////////////////////////////////////////////////////////////////////
		inline static bool IsSameHour(const ST_SYSTEMTIME& stOldTime, const ST_SYSTEMTIME& stNewTime)
		{
			if (stOldTime.wDay != stNewTime.wDay)
				return false;

			if (stOldTime.wHour != stNewTime.wHour)
				return false;

			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		inline static bool IsSameMinute(const ST_SYSTEMTIME& stOldTime, const ST_SYSTEMTIME& stNewTime)
		{
			if (stOldTime.wDay != stNewTime.wDay)
				return false;

			if (stOldTime.wHour != stNewTime.wHour)
				return false;

			if (stOldTime.wMinute != stNewTime.wMinute)
				return false;

			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		inline static bool IsSameSecond(const ST_SYSTEMTIME& stOldTime, const ST_SYSTEMTIME& stNewTime)
		{
			if (stOldTime.wDay != stNewTime.wDay)
				return false;

			if (stOldTime.wHour != stNewTime.wHour)
				return false;

			if (stOldTime.wMinute != stNewTime.wMinute)
				return false;

			if (stOldTime.wSecond != stNewTime.wSecond)
				return false;

			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		inline static void Internal_FileRotateWorker(ST_LOG_CONTEXT* pContext, size_t tCurFileSize);
		inline static void Internal_FileWorker(ST_LOG_CONTEXT* pContext, std::string strOutputMsgA)
		{
			EnterCriticalSection(pContext->hFileCS);

			size_t tCurFileSize = 0;
			HANDLE hFile = CreateFileW(pContext->strOutputFile.c_str(), FILE_APPEND_DATA_, OPEN_ALWAYS_, 0);
			if (hFile)
			{
				DWORD dwWritten = 0;
				WriteFile(hFile, strOutputMsgA.c_str(), strOutputMsgA.length(), &dwWritten);
				tCurFileSize = (size_t)SetFilePointer(hFile, 0, FILE_CURRENT_);
				if (-1 == tCurFileSize)
					tCurFileSize = (size_t)SetFilePointer(hFile, 0, FILE_END_);
				CloseFile(hFile);

				if (-1 == tCurFileSize)
				{
					printf("CurrentFileSize is not queried.(Err:%d)\n", GetLastError());
					tCurFileSize = 0;
				}
			}
			else
			{
				wprintf(L"Log file cannot be opened, %u, %s\n", GetLastError(), pContext->strOutputFile.c_str());
				printf("%s", strOutputMsgA.c_str());
			}

			Internal_FileRotateWorker(pContext, tCurFileSize);
			LeaveCriticalSection(pContext->hFileCS);
		}
		//////////////////////////////////////////////////////////////////////////
		inline static void Internal_FileRotateWorker(ST_LOG_CONTEXT* pContext, size_t tCurFileSize)
		{
			static DWORD s_dwTiming = 0;
			if ((InterlockedIncrement_(&s_dwTiming) & 0x3F) != 0)
				return;

			bool bUnderMaxFileSize	= tCurFileSize < (size_t)pContext->dwMaxFileSize;
			bool bIsSameDay			= IsSameDay(pContext->stLastCheckTime, pContext->stLastTime);
			bool bIsSameHour		= IsSameHour(pContext->stLastCheckTime, pContext->stLastTime);
			bool bIsSameMinute		= IsSameMinute(pContext->stLastCheckTime, pContext->stLastTime);
			bool bIsSameSecond		= IsSameSecond(pContext->stLastCheckTime, pContext->stLastTime);

			switch (pContext->nRotationType)
			{
			case LOG_ROTATION_SIZE:
				if( bUnderMaxFileSize )						return;
				break;
			case LOG_ROTATION_DATE:
				if (bIsSameDay)								return;
				break;
			case LOG_ROTATION_SIZE_OR_DATE:
				if (bIsSameDay && bUnderMaxFileSize)		return;
				break;
			case LOG_ROTATION_SIZE_OR_HOUR:
				if (bIsSameHour && bUnderMaxFileSize)		return;
				break;
			case LOG_ROTATION_SIZE_OR_MINUTE:
				if (bIsSameMinute && bUnderMaxFileSize)		return;
				break;
			case LOG_ROTATION_SIZE_OR_SECOND:
				if (bIsSameSecond && bUnderMaxFileSize)		return;
				break;
			default:
				break;
			}
				
			pContext->stLastCheckTime = pContext->stLastTime;

			std::wstring strDir = ExtractDirectory(pContext->strOutputFile);
			std::wstring strFile = ExtractFileNameWithoutExt(pContext->strOutputFile);
			std::wstring strEXT = ExtractFileExt(pContext->strOutputFile);
			std::wstring strSystemTime = BuildLocalTimeStringW();
			std::wstring strNewFilename = Format(L"%s/%s_%s.%s", strDir.c_str(), strFile.c_str(), strSystemTime.c_str(), strEXT.c_str());

			MoveFileW(pContext->strOutputFile.c_str(), strNewFilename.c_str());

			std::wstring strFilePattern = Format(L"%s*.%s", strFile.c_str(), strEXT.c_str());
			std::set<std::wstring> setFiles;
			GrepLogFile(strDir.c_str(), strFilePattern.c_str(), setFiles);

			strFilePattern += L".zip";
			GrepLogFile(strDir.c_str(), strFilePattern.c_str(), setFiles);

			while (setFiles.size() >= pContext->dwMaxFileCount)
			{
				std::wstring strOldestFile = *setFiles.begin();
				DeleteFileW(strOldestFile.c_str());
				setFiles.erase(strOldestFile);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		static inline void Internal_LogWorker(ST_LOG_CONTEXT* pContext, DWORD dwInputType, std::string strLog)
		{
			std::string strOutputMsgA;

			LPCSTR pszPrefix;
			switch (dwInputType)
			{
			case LOG_INFO:		pszPrefix = "INFO";		break;
			case LOG_WARN:		pszPrefix = "WARN";		break;
			case LOG_ERROR:		pszPrefix = "ERROR";	break;
			case LOG_DEBUG:		pszPrefix = "DEBUG";	break;
			case LOG_TRACE:		pszPrefix = "TRACE";	break;
			default:
				pszPrefix = "????";						break;
			}

			{
				ST_SYSTEMTIME stTime;
				GetLocalTime(&stTime);

				strOutputMsgA = core::Format("%04u-%02u-%02u %02u:%02u:%02u.%03u [p%04u|t%04u|e%u] %-5.5s %s - %s\n"
					, stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds
					, pContext->dwProcessID, core::GetCurrentThreadId(), GetLastError()
					, pszPrefix
					, pContext->strID.c_str()
					, strLog.c_str());

				//strOutputMsgA = Format("[%04u-%02u-%02u|%02u:%02u:%02u.%03u|p%04u|t%04u|e%04u|%s] %s %s\n"
				//	, stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds, pContext->dwProcessID
				//	, GetCurrentThreadId(), GetLastError(), pContext->strID.c_str()
				//	, pszPrefix, strLog.c_str());

				pContext->stLastTime = stTime;
			}

			if (pContext->dwOutputFlag & LOG_OUTPUT_FILE)
				Internal_FileWorker(pContext, strOutputMsgA);
			else if (pContext->dwOutputFlag & LOG_OUTPUT_ENCFILE)
			{
				std::string strEncoded = DefaultEncodeStringA(strOutputMsgA) + "\n";
				Internal_FileWorker(pContext, strEncoded);
			}

			if (pContext->dwOutputFlag & LOG_OUTPUT_CONSOLE)
			{
				if (LOG_ERROR == dwInputType)
					fprintf(stderr, "%s %s\n", pszPrefix, strLog.c_str());
				else
					printf("%s %s\n", pszPrefix, strLog.c_str());
			}

			if (pContext->dwOutputFlag & LOG_OUTPUT_DBGWND)
				OutputDebugStringA("%s", strOutputMsgA.c_str());

			if (pContext->fpCustomOutput)
				pContext->fpCustomOutput(pszPrefix, strLog.c_str());
		}

		//////////////////////////////////////////////////////////////////////////
		void Log_FormatV(DWORD dwInputType, const char* pszFormat, va_list vaList)
		{
			if ((GlobalLog()->dwInputFlag & dwInputType) == 0)
				return;

			const size_t	tBuffLen = 2048;
			char			szBuff[tBuffLen];
			SafeSVPrintf(szBuff, tBuffLen, pszFormat, vaList);
			Internal_LogWorker(GlobalLog(), dwInputType, szBuff);
		}

		//////////////////////////////////////////////////////////////////////////
		void Log_FormatV(DWORD dwInputType, const wchar_t* pszFormat, va_list vaList)
		{
			if ((GlobalLog()->dwInputFlag & dwInputType) == 0)
				return;

			const size_t	tBuffLen = 2048;
			wchar_t			szBuff[tBuffLen];
			SafeSVPrintf(szBuff, tBuffLen, pszFormat, vaList);
			Internal_LogWorker(GlobalLog(), dwInputType, MBSFromWCS(szBuff));
		}

		//////////////////////////////////////////////////////////////////////////
		void Log_FormatVEx(ST_LOG_CONTEXT* pContext, DWORD dwInputType, const char* pszFormat, va_list vaList)
		{
			const size_t	tBuffLen = 2048;
			char			szBuff[tBuffLen];
			SafeSVPrintf(szBuff, tBuffLen, pszFormat, vaList);
			Internal_LogWorker(pContext, dwInputType, szBuff);
		}

		//////////////////////////////////////////////////////////////////////////
		void Log_FormatVEx(ST_LOG_CONTEXT* pContext, DWORD dwInputType, const wchar_t* pszFormat, va_list vaList)
		{
			const size_t	tBuffLen = 2048;
			wchar_t			szBuff[tBuffLen];
			SafeSVPrintf(szBuff, tBuffLen, pszFormat, vaList);
			Internal_LogWorker(pContext, dwInputType, MBSFromWCS(szBuff));
		}
	}
}