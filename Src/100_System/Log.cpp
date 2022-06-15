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

		GlobalLog()->strOutputFile		= WCSFromTCS(strFilePath);
		GlobalLog()->strID				= MBSFromTCS(ExtractFileNameWithoutExt(GetFileName()));
		GlobalLog()->dwInputFlag		= dwInputFlag;
		GlobalLog()->dwOutputFlag		= bEncrypt? LOG_OUTPUT_ENCFILE : LOG_OUTPUT_FILE;
		GlobalLog()->dwOutputFlag		|= LOG_OUTPUT_DBGWND;
		GlobalLog()->fpCustomOutput	= NULL;
	}

	//////////////////////////////////////////////////////////////////////////	
	void InitLog(const ST_LOG_INIT_PARAM& stParam)
	{
		std::tstring strLogDir = ExtractDirectory(stParam.strLogPath);
		if( !strLogDir.empty() )
			CreateDirectory(strLogDir.c_str());

		GlobalLog()->strOutputFile		= WCSFromTCS(stParam.strLogPath)		;
		GlobalLog()->strID				= MBSFromTCS(ExtractFileName(stParam.strModulePath));
		GlobalLog()->dwInputFlag		= stParam.dwInputFlag					;
		GlobalLog()->dwOutputFlag		= stParam.dwOutputFlag					;
		GlobalLog()->dwMaxFileSize		= stParam.dwMaxFileSize					;
		GlobalLog()->dwMaxFileCount	= stParam.dwMaxFileCount				;
		GlobalLog()->nRotationType		= LOG_ROTATION_SIZE						;
		GlobalLog()->fpCustomOutput	= stParam.fpCustomOutput				;
	}

	//////////////////////////////////////////////////////////////////////////	
	void InitLog(const ST_LOG_INIT_PARAM_EX& stParam)
	{
		std::tstring strLogDir = ExtractDirectory(stParam.strLogFile);
		if (!strLogDir.empty())
			CreateDirectory(strLogDir.c_str());

		GlobalLog()->strOutputFile		= WCSFromTCS(stParam.strLogFile)		;
		GlobalLog()->strID				= MBSFromTCS(stParam.strID)				;
		GlobalLog()->dwInputFlag		= stParam.dwInputFlag					;
		GlobalLog()->dwOutputFlag		= stParam.dwOutputFlag					;
		GlobalLog()->dwMaxFileSize		= stParam.dwMaxFileSize					;
		GlobalLog()->dwMaxFileCount	= stParam.dwMaxFileCount				;
		GlobalLog()->nRotationType		= stParam.nLogRotation					;
		GlobalLog()->fpCustomOutput	= stParam.fpCustomOutput				;
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