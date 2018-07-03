#include "stdafx.h"
#include "Log.h"
#include <vector>

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	void CLog::Init(LPCTSTR pszFilePath)
	{
		ST_LOG_INIT_PARAM stParam;
		stParam.strLogPath = pszFilePath;
		stParam.dwInputFlag = LOG_DEFAULT;
		stParam.dwOutputFlag = LOG_OUTPUT_FILE;
		stParam.fpCustomOutput = NULL;

		InitLog(pszFilePath);
	}

	//////////////////////////////////////////////////////////////////////////
	void CLog::Init(const ST_LOG_INIT_PARAM& stParam)
	{
		InitLog(stParam);
	}

	//////////////////////////////////////////////////////////////////////////
	void CLog::Debug(LPCTSTR pszFormat, ...)
	{
		va_list vaList;
		va_start(vaList, pszFormat);
		Log_FormatV(TEXT("[DEBUG]"), pszFormat, vaList);
		va_end(vaList);
	}

	//////////////////////////////////////////////////////////////////////////
	void CLog::Info(LPCTSTR pszFormat, ...)
	{
		va_list vaList;
		va_start(vaList, pszFormat);
		Log_FormatV(TEXT("[INFO]"), pszFormat, vaList);
		va_end(vaList);
	}

	//////////////////////////////////////////////////////////////////////////
	void CLog::Warn(LPCTSTR pszFormat, ...)
	{
		va_list vaList;
		va_start(vaList, pszFormat);
		Log_FormatV(TEXT("[WARN]"), pszFormat, vaList);
		va_end(vaList);
	}

	//////////////////////////////////////////////////////////////////////////
	void CLog::Error(LPCTSTR pszFormat, ...)
	{
		va_list vaList;
		va_start(vaList, pszFormat);
		Log_FormatV(TEXT("[ERROR]"), pszFormat, vaList);
		va_end(vaList);
	}
}
