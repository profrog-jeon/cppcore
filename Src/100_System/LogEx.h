#pragma once

#include <stdarg.h>
#include "Log.h"

#ifdef UNICODE
#define DestroyLogHandle		DestroyLogHandleW
#else
#define DestroyLogHandle		DestroyLogHandleA
#endif

namespace core
{
	HANDLE CreateLogHandle(const ST_LOG_INIT_PARAM_EXA& stParam);
	HANDLE CreateLogHandle(const ST_LOG_INIT_PARAM_EXW& stParam);
	void DestroyLogHandleA(HANDLE hLog);
	void DestroyLogHandleW(HANDLE hLog);

	void Debug(HANDLE hLog, const char* pszFormat, ...);
	void Debug(HANDLE hLog, const wchar_t* pszFormat, ...);
	void Debug(HANDLE hLog, const char* pszFormat, va_list list);
	void Debug(HANDLE hLog, const wchar_t* pszFormat, va_list list);

	void Info(HANDLE hLog, const char* pszFormat, ...);
	void Info(HANDLE hLog, const wchar_t* pszFormat, ...);
	void Info(HANDLE hLog, const char* pszFormat, va_list list);
	void Info(HANDLE hLog, const wchar_t* pszFormat, va_list list);

	void Warn(HANDLE hLog, const char* pszFormat, ...);
	void Warn(HANDLE hLog, const wchar_t* pszFormat, ...);
	void Warn(HANDLE hLog, const char* pszFormat, va_list list);
	void Warn(HANDLE hLog, const wchar_t* pszFormat, va_list list);

	void Error(HANDLE hLog, const char* pszFormat, ...);
	void Error(HANDLE hLog, const wchar_t* pszFormat, ...);
	void Error(HANDLE hLog, const char* pszFormat, va_list list);
	void Error(HANDLE hLog, const wchar_t* pszFormat, va_list list);

	void Trace(HANDLE hLog, const char* pszFormat, ...);
	void Trace(HANDLE hLog, const wchar_t* pszFormat, ...);
	void Trace(HANDLE hLog, const char* pszFormat, va_list list);
	void Trace(HANDLE hLog, const wchar_t* pszFormat, va_list list);
}
