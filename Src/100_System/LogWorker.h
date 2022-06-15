#pragma once

#include <stdarg.h>
#include "Log.h"

namespace core
{
	namespace internal
	{
		struct ST_LOG_CONTEXT
		{
			HANDLE			hFileCS;
			DWORD			dwProcessID;
			DWORD			dwInputFlag;
			DWORD			dwOutputFlag;
			DWORD			dwMaxFileSize;
			DWORD			dwMaxFileCount;
			E_LOG_ROTATION	nRotationType;
			std::wstring	strOutputFile;
			std::string		strID;
			ST_SYSTEMTIME	stLastTime;
			ST_SYSTEMTIME	stLastCheckTime;
			void(*fpCustomOutput)(const char* pszPrefix, const char* pszMsg);

			ST_LOG_CONTEXT(void);
			~ST_LOG_CONTEXT(void);
		};

		class CGlobalLogContext : public ST_LOG_CONTEXT
		{
			CGlobalLogContext(void);
			~CGlobalLogContext(void);

		public:
			static CGlobalLogContext* GetInstance(void)
			{
				static CGlobalLogContext instance;
				return &instance;
			}
		};

		inline CGlobalLogContext* GlobalLog(void)		{	return CGlobalLogContext::GetInstance();	}

		void Log_FormatV(DWORD dwInputType, const char* pszFormat, va_list vaList);
		void Log_FormatV(DWORD dwInputType, const wchar_t* pszFormat, va_list vaList);
		void Log_FormatVEx(ST_LOG_CONTEXT* pContext, DWORD dwInputType, const char* pszFormat, va_list vaList);
		void Log_FormatVEx(ST_LOG_CONTEXT* pContext, DWORD dwInputType, const wchar_t* pszFormat, va_list vaList);
	}
}