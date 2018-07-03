#pragma once

#include <stdarg.h>

namespace core
{
	struct ST_LOG_CONTEXT
	{
		HANDLE			hFileCS			;
		DWORD			dwProcessID		;
		DWORD			dwInputFlag		;
		DWORD			dwOutputFlag	;
		DWORD			dwMaxFileSize	;
		DWORD			dwMaxFileCount	;
		std::wstring	strOutputPath	;
		std::string		strModuleName	;
		void (*fpCustomOutput)(const char* pszPrefix, const char* pszMsg);

		ST_LOG_CONTEXT(void);
		~ST_LOG_CONTEXT(void);
	};
}