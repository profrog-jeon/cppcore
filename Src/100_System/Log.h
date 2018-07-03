#pragma once

#include <stdarg.h>

namespace core
{
	enum E_LOG_INPUT_TYPE
	{
		LOG_DEBUG				= 0x01 << 0,
		LOG_INFO				= 0x01 << 1,
		LOG_WARN				= 0x01 << 2,
		LOG_ERROR				= 0x01 << 3,

		LOG_DEFAULT	= LOG_INFO|LOG_WARN|LOG_ERROR,
		LOG_ALL		= LOG_INFO|LOG_WARN|LOG_ERROR|LOG_DEBUG,
	};

	enum E_LOG_OUTPUT_TYPE
	{
		LOG_OUTPUT_FILE			= 0x01 << 0,
		LOG_OUTPUT_ENCFILE		= 0x01 << 1,
		LOG_OUTPUT_CONSOLE		= 0x01 << 2,
		LOG_OUTPUT_DBGWND		= 0x01 << 3,

		LOG_OUTPUT_DEFAULT		= LOG_OUTPUT_FILE|LOG_OUTPUT_CONSOLE|LOG_OUTPUT_DBGWND,
	};

	struct ST_LOG_INIT_PARAMA
	{
		std::string		strLogPath		;
		std::string		strModulePath	;
		DWORD			dwInputFlag		;	// LOG_INFO|LOG_WARN|LOG_ERROR
		DWORD			dwOutputFlag	;	// LOG_OUTPUT_FILE|LOG_OUTPUT_CONSOLE|LOG_OUTPUT_DBGWND
		DWORD			dwMaxFileSize	;
		DWORD			dwMaxFileCount	;
		void (*fpCustomOutput)(const char* pszPrefix, const char* pszMsg);

		ST_LOG_INIT_PARAMA(void) : dwInputFlag(LOG_DEFAULT), dwOutputFlag(LOG_OUTPUT_DEFAULT), dwMaxFileSize(10 * 1000 * 1000), dwMaxFileCount(3), fpCustomOutput(NULL)	{}
	};

	struct ST_LOG_INIT_PARAMW
	{
		std::wstring	strLogPath		;
		std::wstring	strModulePath	;
		DWORD			dwInputFlag		;	// LOG_INFO|LOG_WARN|LOG_ERROR
		DWORD			dwOutputFlag	;	// LOG_OUTPUT_FILE|LOG_OUTPUT_CONSOLE|LOG_OUTPUT_DBGWND
		DWORD			dwMaxFileSize	;
		DWORD			dwMaxFileCount	;
		void (*fpCustomOutput)(const char* pszPrefix, const char* pszMsg);

		ST_LOG_INIT_PARAMW(void) : dwInputFlag(LOG_DEFAULT), dwOutputFlag(LOG_OUTPUT_DEFAULT), dwMaxFileSize(10 * 1000 * 1000), dwMaxFileCount(3), fpCustomOutput(NULL)	{}
	};

	void InitLog(std::string strPath, bool bEncrypt = false, DWORD dwInputFlag = LOG_INFO|LOG_WARN|LOG_ERROR);
	void InitLog(std::wstring strPath, bool bEncrypt = false, DWORD dwInputFlag = LOG_INFO|LOG_WARN|LOG_ERROR);

	void InitLog(const ST_LOG_INIT_PARAMA& stParam);
	void InitLog(const ST_LOG_INIT_PARAMW& stParam);

	void Log_FormatV(const char* pszPrefix, const char* pszFormat, va_list vaList);
	void Log_FormatV(const wchar_t* pszPrefix, const wchar_t* pszFormat, va_list vaList);

	void Log_Debug(const char* pszFormat, ...);
	void Log_Debug(const wchar_t* pszFormat, ...);
	void Log_Info(const char* pszFormat, ...);
	void Log_Info(const wchar_t* pszFormat, ...);
	void Log_Warn(const char* pszFormat, ...);
	void Log_Warn(const wchar_t* pszFormat, ...);
	void Log_Error(const char* pszFormat, ...);
	void Log_Error(const wchar_t* pszFormat, ...);

#ifdef UNICODE
	typedef ST_LOG_INIT_PARAMW	ST_LOG_INIT_PARAM;
#else
	typedef ST_LOG_INIT_PARAMA	ST_LOG_INIT_PARAM;
#endif

}
