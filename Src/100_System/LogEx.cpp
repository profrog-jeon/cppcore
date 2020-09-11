#include "stdafx.h"
#include "LogEx.h"
#include "Log.h"
#include "LogWorker.h"
#include "FileSystem.h"
#include "KernelObject.h"

namespace core
{
	using namespace internal;

	//////////////////////////////////////////////////////////////////////////
	HANDLE CreateLogHandle(const ST_LOG_INIT_PARAM_EX& stParam)
	{
		std::tstring strLogDir = ExtractDirectory(stParam.strLogFile);
		if (!strLogDir.empty())
			CreateDirectory(strLogDir.c_str());

		ST_LOG_CONTEXT* pLogContext		= new ST_LOG_CONTEXT();
		pLogContext->hFileCS			= InitializeCriticalSection();
		pLogContext->strOutputFile		= WCSFromTCS(stParam.strLogFile);
		pLogContext->strID				= MBSFromTCS(stParam.strID);
		pLogContext->dwInputFlag		= stParam.dwInputFlag;
		pLogContext->dwOutputFlag		= stParam.dwOutputFlag;
		pLogContext->dwMaxFileSize		= stParam.dwMaxFileSize;
		pLogContext->dwMaxFileCount		= stParam.dwMaxFileCount;
		pLogContext->nRotationType		= stParam.nLogRotation;
		pLogContext->fpCustomOutput		= stParam.fpCustomOutput;
		return (HANDLE)pLogContext;
	}

	//////////////////////////////////////////////////////////////////////////
	void DestroyLogHandle(HANDLE hLog)
	{
		ST_LOG_CONTEXT* pContext = (ST_LOG_CONTEXT*)hLog;
		DeleteCriticalSection(pContext->hFileCS);
		delete pContext;
	}

	//////////////////////////////////////////////////////////////////////////
	void Debug(HANDLE hLog, const TCHAR* pszFormat, ...)
	{
		ST_LOG_CONTEXT* pContext = (ST_LOG_CONTEXT*)hLog;
		if (0 == (pContext->dwInputFlag & LOG_DEBUG))
			return;

		va_list list;
		va_start(list, pszFormat);
		Log_FormatVEx(pContext, LOG_DEBUG, pszFormat, list);
		va_end(list);
	}

	void Debug(HANDLE hLog, const TCHAR* pszFormat, va_list list)
	{
		ST_LOG_CONTEXT* pContext = (ST_LOG_CONTEXT*)hLog;
		if (0 == (pContext->dwInputFlag & LOG_DEBUG))
			return;

		Log_FormatVEx(pContext, LOG_DEBUG, pszFormat, list);
	}
	//////////////////////////////////////////////////////////////////////////
	void Info(HANDLE hLog, const TCHAR* pszFormat, ...)
	{
		ST_LOG_CONTEXT* pContext = (ST_LOG_CONTEXT*)hLog;
		if (0 == (pContext->dwInputFlag & LOG_INFO))
			return;

		va_list list;
		va_start(list, pszFormat);
		Log_FormatVEx(pContext, LOG_INFO, pszFormat, list);
		va_end(list);
	}

	void Info(HANDLE hLog, const TCHAR* pszFormat, va_list list)
	{
		ST_LOG_CONTEXT* pContext = (ST_LOG_CONTEXT*)hLog;
		if (0 == (pContext->dwInputFlag & LOG_INFO))
			return;

		Log_FormatVEx(pContext, LOG_INFO, pszFormat, list);
	}
	//////////////////////////////////////////////////////////////////////////
	void Warn(HANDLE hLog, const TCHAR* pszFormat, ...)
	{
		ST_LOG_CONTEXT* pContext = (ST_LOG_CONTEXT*)hLog;
		if (0 == (pContext->dwInputFlag & LOG_WARN))
			return;

		va_list list;
		va_start(list, pszFormat);
		Log_FormatVEx(pContext, LOG_WARN, pszFormat, list);
		va_end(list);
	}

	void Warn(HANDLE hLog, const TCHAR* pszFormat, va_list list)
	{
		ST_LOG_CONTEXT* pContext = (ST_LOG_CONTEXT*)hLog;
		if (0 == (pContext->dwInputFlag & LOG_WARN))
			return;

		Log_FormatVEx(pContext, LOG_WARN, pszFormat, list);
	}
	//////////////////////////////////////////////////////////////////////////
	void Error(HANDLE hLog, const TCHAR* pszFormat, ...)
	{
		ST_LOG_CONTEXT* pContext = (ST_LOG_CONTEXT*)hLog;
		if (0 == (pContext->dwInputFlag & LOG_ERROR))
			return;

		va_list list;
		va_start(list, pszFormat);
		Log_FormatVEx(pContext, LOG_ERROR, pszFormat, list);
		va_end(list);
	}

	void Error(HANDLE hLog, const TCHAR* pszFormat, va_list list)
	{
		ST_LOG_CONTEXT* pContext = (ST_LOG_CONTEXT*)hLog;
		if (0 == (pContext->dwInputFlag & LOG_ERROR))
			return;

		Log_FormatVEx(pContext, LOG_ERROR, pszFormat, list);
	}
	//////////////////////////////////////////////////////////////////////////
	void Trace(HANDLE hLog, const TCHAR* pszFormat, ...)
	{
		ST_LOG_CONTEXT* pContext = (ST_LOG_CONTEXT*)hLog;
		if (0 == (pContext->dwInputFlag & LOG_TRACE))
			return;

		va_list list;
		va_start(list, pszFormat);
		Log_FormatVEx(pContext, LOG_TRACE, pszFormat, list);
		va_end(list);
	}

	void Trace(HANDLE hLog, const TCHAR* pszFormat, va_list list)
	{
		ST_LOG_CONTEXT* pContext = (ST_LOG_CONTEXT*)hLog;
		if (0 == (pContext->dwInputFlag & LOG_TRACE))
			return;

		Log_FormatVEx(pContext, LOG_TRACE, pszFormat, list);
	}
}