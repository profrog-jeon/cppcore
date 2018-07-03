#pragma once

#include <stdarg.h>

namespace core
{
	bool IsWhiteSpace(char cChar);
	bool IsWhiteSpace(wchar_t cChar);
	int SafeStrLen(const char* pszContext, size_t tMaxCch);
	int SafeStrLen(const wchar_t* pszContext, size_t tMaxCch);
	char* SafeStrCpy(char* pszDest, size_t tDestCch, const char* pszSrc);
	wchar_t* SafeStrCpy(wchar_t* pszDest, size_t tDestCch, const wchar_t* pszSrc);
	char* SafeStrCat(char* pszDest, size_t tDestCch, const char* pszSrc);
	wchar_t* SafeStrCat(wchar_t* pszDest, size_t tDestCch, const wchar_t* pszSrc);
	const char* SafeStrStr(const char* pszDest, size_t tDestCch, const char* pszSrc);
	const wchar_t* SafeStrStr(const wchar_t* pszDest, size_t tDestCch, const wchar_t* pszSrc);
	int SafeSPrintf(char* pszDest, size_t tDestCch, const char* pszFormat, ...);
	int SafeSPrintf(wchar_t* pszDest, size_t tDestCch, const wchar_t* pszFormat, ...);
	int SafeSVPrintf(char* pszDest, size_t tDestCch, const char* pszFormat, va_list vaList);
	int SafeSVPrintf(wchar_t* pszDest, size_t tDestCch, const wchar_t* pszFormat, va_list vaList);
	size_t SafeFindStr(const char* pszDest, size_t tDestCch, const char* pszKey, size_t tPos = 0);
	size_t SafeFindStr(const wchar_t* pszDest, size_t tCchDest, const wchar_t* pszKey, size_t tPos = 0);
	size_t SafeReverseFindStr(const char* pszDest, size_t tDestCch, const char* pszKey, size_t tPos = 0xFFFFFFFF);
	size_t SafeReverseFindStr(const wchar_t* pszDest, size_t tCchDest, const wchar_t* pszKey, size_t tPos = 0xFFFFFFFF);
	int SafeStrCmp(const char* pszSrc, const char* pszTarget, size_t tMaxCch);
	int SafeStrCmp(const wchar_t* pszSrc, const wchar_t* pszTarget, size_t tMaxCch);
	bool SafeStrCmpWithWildcard(const char* pszDest, size_t tDestCch, const char* pszPattern);
	bool SafeStrCmpWithWildcard(const wchar_t* pszDest, size_t tDestCch, const wchar_t* pszPattern);
}