#include "stdafx.h"
#include "Log.h"
#include "LogWorker.h"
#include "KernelObject.h"
#include "Environment.h"
#include "FileSystem.h"

namespace core
{
	using namespace internal;

	//////////////////////////////////////////////////////////////////////////
	void InitLog(std::tstring strFilePath, bool bEncrypt, DWORD dwInputFlag)
	{
		std::tstring strPath = ExtractDirectory(strFilePath);
		if( !strPath.empty() )
			CreateDirectory(strPath.c_str());

		ST_LOG_CONTEXT stLogContext;
		stLogContext.strOutputFile		= WCSFromTCS(strFilePath);
		stLogContext.strID				= MBSFromTCS(ExtractFileNameWithoutExt(GetFileName()));
		stLogContext.dwInputFlag		= dwInputFlag;
		stLogContext.dwOutputFlag		= bEncrypt? LOG_OUTPUT_ENCFILE : LOG_OUTPUT_FILE;
		stLogContext.dwOutputFlag		|= LOG_OUTPUT_DBGWND;
		stLogContext.fpCustomOutput		= NULL;
		InitGlobalLogContext(stLogContext);
	}

	//////////////////////////////////////////////////////////////////////////	
	void InitLog(const ST_LOG_INIT_PARAM& stParam)
	{
		std::tstring strLogDir = ExtractDirectory(stParam.strLogPath);
		if( !strLogDir.empty() )
			CreateDirectory(strLogDir.c_str());

		ST_LOG_CONTEXT stLogContext;
		stLogContext.strOutputFile		= WCSFromTCS(stParam.strLogPath)		;
		stLogContext.strID				= MBSFromTCS(ExtractFileName(stParam.strModulePath));
		stLogContext.dwInputFlag		= stParam.dwInputFlag					;
		stLogContext.dwOutputFlag		= stParam.dwOutputFlag					;
		stLogContext.dwMaxFileSize		= stParam.dwMaxFileSize					;
		stLogContext.dwMaxFileCount		= stParam.dwMaxFileCount				;
		stLogContext.nRotationType		= LOG_ROTATION_SIZE						;
		stLogContext.fpCustomOutput		= stParam.fpCustomOutput				;
		InitGlobalLogContext(stLogContext);
	}

	//////////////////////////////////////////////////////////////////////////	
	void InitLog(const ST_LOG_INIT_PARAM_EX& stParam)
	{
		std::tstring strLogDir = ExtractDirectory(stParam.strLogFile);
		if (!strLogDir.empty())
			CreateDirectory(strLogDir.c_str());

		ST_LOG_CONTEXT stLogContext;
		stLogContext.strOutputFile		= WCSFromTCS(stParam.strLogFile)		;
		stLogContext.strID				= MBSFromTCS(stParam.strID)				;
		stLogContext.dwInputFlag		= stParam.dwInputFlag					;
		stLogContext.dwOutputFlag		= stParam.dwOutputFlag					;
		stLogContext.dwMaxFileSize		= stParam.dwMaxFileSize					;
		stLogContext.dwMaxFileCount		= stParam.dwMaxFileCount				;
		stLogContext.nRotationType		= stParam.nLogRotation					;
		stLogContext.fpCustomOutput		= stParam.fpCustomOutput				;
		InitGlobalLogContext(stLogContext);
	}

	//////////////////////////////////////////////////////////////////////////
	void Log_Debug(const TCHAR* pszFormat, ...)
	{
		va_list list;
		va_start(list, pszFormat);
		Log_FormatV(LOG_DEBUG, pszFormat, list);
		va_end(list);
	}

	//////////////////////////////////////////////////////////////////////////
	void Log_Info(const TCHAR* pszFormat, ...)
	{
		va_list list;
		va_start(list, pszFormat);
		Log_FormatV(LOG_INFO, pszFormat, list);
		va_end(list);
	}

	//////////////////////////////////////////////////////////////////////////
	void Log_Warn(const TCHAR* pszFormat, ...)
	{
		va_list list;
		va_start(list, pszFormat);
		Log_FormatV(LOG_WARN, pszFormat, list);
		va_end(list);
	}

	//////////////////////////////////////////////////////////////////////////
	void Log_Error(const TCHAR* pszFormat, ...)
	{
		va_list list;
		va_start(list, pszFormat);
		Log_FormatV(LOG_ERROR, pszFormat, list);
		va_end(list);
	}

	//////////////////////////////////////////////////////////////////////////
	void Log_Trace(const TCHAR* pszFormat, ...)
	{
		va_list list;
		va_start(list, pszFormat);
		Log_FormatV(LOG_TRACE, pszFormat, list);
		va_end(list);
	}
}