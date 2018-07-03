#pragma once

#include <stdlib.h>

#define wtoi			_wtoi			

#ifdef UNICODE

#define fputts			fputws
#define tprintf			wprintf
#define vsntprintf		vswprintf
#define _tcstoui64		_wcstoui64
#define tcstod			wcstod
#define gettc			getwc

#else

#define fputts			fputs
#define tprintf			printf
#define vsntprintf		vsnprintf
#define _tcstoui64		_strtoui64
#define tcstod			strtod
#define gettc			getc

#endif
