#pragma once

#include <string>

namespace std
{
#ifdef UNICODE
	typedef std::wstring	tstring;
	typedef std::string		ntstring;
#else
	typedef std::string		tstring;
	typedef std::wstring	ntstring;
#endif
}


typedef char					CHAR;
typedef wchar_t					WCHAR;

#ifdef UNICODE
	typedef const wchar_t*			LPCTSTR;
	typedef wchar_t*				LPTSTR;
	typedef wchar_t					TCHAR;
#else
	typedef const char*				LPCTSTR;
	typedef char*					LPTSTR;
	typedef char					TCHAR;
#endif

#ifdef _MSC_VER
	typedef unsigned	__int32		uint32_t;
	typedef unsigned	__int64		uint64_t;
	typedef				__int32		int32_t;
	typedef				__int64		int64_t;
	typedef unsigned	long		DWORD,	*LPDWORD;		// For windows definition
	typedef unsigned	__int64		QWORD,	*LPQWORD;		// For windows definition
#else
	typedef	__uint32_t				uint32_t;
	typedef	__uint64_t				uint64_t;
	typedef	__int32_t				int32_t;
	typedef	__int64_t				int64_t;
	typedef unsigned	int			DWORD,	*LPDWORD;		// For 64bits Mac definition.(long size will be 64bits)
	typedef __uint64_t				QWORD,	*LPQWORD;		// For 64bits Mac definition.(long size will be 64bits)
#endif

typedef unsigned char				uchar;
typedef unsigned short				ushort;
typedef unsigned int				uint;
typedef uint64_t					ULONGLONG;
typedef void*						HANDLE;
typedef unsigned short				WORD,	*LPWORD;
typedef unsigned char				BYTE,	*LPBYTE;
typedef const unsigned char			CBYTE, *LPCBYTE;
typedef int							BOOL,	*LPBOOL;

typedef const wchar_t*				LPCWSTR;
typedef const char*					LPCSTR;
typedef wchar_t*					LPWSTR;
typedef char*						LPSTR;

typedef int							BOOL;
typedef int							INT;
typedef unsigned short				USHORT;
typedef unsigned int				UINT;
typedef uint64_t					UINT64;

typedef void*						LPVOID;
typedef const void*					LPCVOID;

typedef unsigned char 				UCHAR;
typedef UCHAR						*PUCHAR;
typedef DWORD						*PDWORD;

typedef BYTE						*PBYTE;
