#pragma once

#include <stdarg.h>

// helper macro to set the fill character and specify buffer filling
#define STRSAFE_FILL_BYTE_(x)                    ((DWORD)((x & 0x000000FF) | STRSAFE_FILL_BEHIND_NULL_))
#define STRSAFE_FAILURE_BYTE_(x)                 ((DWORD)((x & 0x000000FF) | STRSAFE_FILL_ON_FAILURE_))

namespace core
{
	enum E_STRING_FLAG
	{
		STRSAFE_IGNORE_NULLS_     = 0x00000100,  // treat null string pointers as TEXT("") -- don't fault on NULL buffers
		STRSAFE_FILL_BEHIND_NULL_ = 0x00000200,  // on success, fill in extra space behind the null terminator with fill pattern
		STRSAFE_FILL_ON_FAILURE_  = 0x00000400,  // on failure, overwrite pszDest with fill pattern and null terminate it
		STRSAFE_NULL_ON_FAILURE_  = 0x00000800,  // on failure, set *pszDest = TEXT('\0')
		STRSAFE_NO_TRUNCATION_    = 0x00001000,  // instead of returning a truncated result, copy/append nothing to pszDest and null terminate it
	};

	ECODE StringCchCopy(char* pszDest, size_t tDestCch, const char* pszSrc);
	ECODE StringCchCopy(wchar_t* pszDest, size_t tDestCch, const wchar_t* pszSrc);	
	ECODE StringCchCopyN(char* pszDest, size_t tDestCch, const char* pszSrc, size_t tCopyCch);
	ECODE StringCchCopyN(wchar_t* pszDest, size_t tDestCch, const wchar_t* pszSrc, size_t tCopyCch);	
	ECODE StringCchCat(char* pszDest, size_t tDestCch, const char* pszSrc);
	ECODE StringCchCat(wchar_t* pszDest, size_t tDestCch, const wchar_t* pszSrc);	
	ECODE StringCchCatN(char* pszDest, size_t tDestCch, const char* pszSrc, size_t tAppendCch);
	ECODE StringCchCatN(wchar_t* pszDest, size_t tDestCch, const wchar_t* pszSrc, size_t tAppendCch);	
	ECODE StringCchPrintf(char* pszDest, size_t tDestCch, const char* pszFormat, ...);
	ECODE StringCchPrintf(wchar_t* pszDest, size_t tDestCch, const wchar_t* pszFormat, ...);	
	ECODE StringCchVPrintf(char* pszDest, size_t tDestCch, const char* pszFormat, va_list vaList);
	ECODE StringCchVPrintf(wchar_t* pszDest, size_t tDestCch, const wchar_t* pszFormat, va_list vaList);
	ECODE StringCchGets(char* pszDest, size_t tDestCch);
	ECODE StringCchGets(wchar_t* pszDest, size_t tDestCch);
	ECODE StringCchLength(const char* pszContext, size_t tMaxCch, size_t* pLengthCch);
	ECODE StringCchLength(const wchar_t* pszContext, size_t tMaxCch, size_t* pLengthCch);

	ECODE StringCbCopy(char* pszDest, size_t tDestCb, const char* pszSrc);
	ECODE StringCbCopy(wchar_t* pszDest, size_t tDestCb, const wchar_t* pszSrc);	
	ECODE StringCbCopyN(char* pszDest, size_t tDestCb, const char* pszSrc, size_t tCopyCb);
	ECODE StringCbCopyN(wchar_t* pszDest, size_t tDestCb, const wchar_t* pszSrc, size_t tCopyCb);	
	ECODE StringCbCat(char* pszDest, size_t tDestCb, const char* pszSrc);
	ECODE StringCbCat(wchar_t* pszDest, size_t tDestCb, const wchar_t* pszSrc);	
	ECODE StringCbCatN(char* pszDest, size_t tDestCb, const char* pszSrc, size_t tAppendCb);
	ECODE StringCbCatN(wchar_t* pszDest, size_t tDestCb, const wchar_t* pszSrc, size_t tAppendCb);	
	ECODE StringCbPrintf(char* pszDest, size_t tDestCb, const char* pszFormat, ...);
	ECODE StringCbPrintf(wchar_t* pszDest, size_t tDestCb, const wchar_t* pszFormat, ...);	
	ECODE StringCbVPrintf(char* pszDest, size_t tDestCb, const char* pszFormat, va_list vaList);
	ECODE StringCbVPrintf(wchar_t* pszDest, size_t tDestCb, const wchar_t* pszFormat, va_list vaList);	
	ECODE StringCbGets(char* pszDest, size_t tDestCb);
	ECODE StringCbGets(wchar_t* pszDest, size_t tDestCb);	
	ECODE StringCbLength(const char* pszContext, size_t tMaxCb, size_t* pLengthCb);
	ECODE StringCbLength(const wchar_t* pszContext, size_t tMaxCb, size_t* pLengthCb);

	ECODE StringCchCopyEx(char* pszDest, size_t tDestCch, const char* pszSrc, char** ppszDestEnd, size_t* pcchRemaining, DWORD dwFlags);
	ECODE StringCchCopyEx(wchar_t* pszDest, size_t tDestCch, const wchar_t* pszSrc, wchar_t** ppszDestEnd, size_t* pcchRemaining, DWORD dwFlags);
	ECODE StringCchCopyNEx(char* pszDest, size_t tDestCch, const char* pszSrc, size_t tCopyCch, char** ppszDestEnd, size_t* pRemainingCch, DWORD dwFlags);
	ECODE StringCchCopyNEx(wchar_t* pszDest, size_t tDestCch, const wchar_t* pszSrc, size_t tCopyCch, wchar_t** ppszDestEnd, size_t* pRemainingCch, DWORD dwFlags);
	ECODE StringCchCatEx(char* pszDest, size_t tDestCch, const char* pszSrc, char** ppszDestEnd, size_t* pRemainingCch, DWORD dwFlags);
	ECODE StringCchCatEx(wchar_t* pszDest, size_t tDestCch, const wchar_t* pszSrc, wchar_t** ppszDestEnd, size_t* pRemainingCch, DWORD dwFlags);
	ECODE StringCchCatNEx(char* pszDest, size_t tDestCch, const char* pszSrc, size_t tAppendCch, char** ppszDestEnd, size_t* pRemainingCch, DWORD dwFlags);
	ECODE StringCchCatNEx(wchar_t* pszDest, size_t tDestCch, const wchar_t* pszSrc, size_t tAppendCch, wchar_t** ppszDestEnd, size_t* pRemainingCch, DWORD dwFlags);
	ECODE StringCchPrintfEx(char* pszDest, size_t tDestCch, char** ppszDestEnd, size_t* pRemainingCch, DWORD dwFlags, const char* pszFormat, ...);
	ECODE StringCchPrintfEx(wchar_t* pszDest, size_t tDestCch, wchar_t** ppszDestEnd, size_t* pRemainingCch, DWORD dwFlags, const wchar_t* pszFormat, ...);
	ECODE StringCchVPrintfEx(char* pszDest, size_t tDestCch, char** ppszDestEnd, size_t* pRemainingCch, DWORD dwFlags, const char* pszFormat, va_list vaList);
	ECODE StringCchVPrintfEx(wchar_t* pszDest, size_t tDestCch, wchar_t** ppszDestEnd, size_t* pRemainingCch, DWORD dwFlags, const wchar_t* pszFormat, va_list vaList);
	ECODE StringCchGetsEx(char* pszDest, size_t tDestCch, char** ppszDestEnd, size_t* pRemainingCch, DWORD dwFlags);
	ECODE StringCchGetsEx(wchar_t* pszDest, size_t tDestCch, wchar_t** ppszDestEnd, size_t* pRemainingCch, DWORD dwFlags);

	ECODE StringCbCopyEx(char* pszDest, size_t tDestCb, const char* pszSrc, char** ppszDestEnd, size_t* pCbRemaining, DWORD dwFlags);
	ECODE StringCbCopyEx(wchar_t* pszDest, size_t tDestCb, const wchar_t* pszSrc, wchar_t** ppszDestEnd, size_t* pCbRemaining, DWORD dwFlags);
	ECODE StringCbCopyNEx(char* pszDest, size_t tDestCb, const char* pszSrc, size_t tCopyCb, char** ppszDestEnd, size_t* pRemainingCb, DWORD dwFlags);
	ECODE StringCbCopyNEx(wchar_t* pszDest, size_t tDestCb, const wchar_t* pszSrc, size_t tCopyCb, wchar_t** ppszDestEnd, size_t* pRemainingCb, DWORD dwFlags);
	ECODE StringCbCatEx(char* pszDest, size_t tDestCb, const char* pszSrc, char** ppszDestEnd, size_t* pRemainingCb, DWORD dwFlags);
	ECODE StringCbCatEx(wchar_t* pszDest, size_t tDestCb, const wchar_t* pszSrc, wchar_t** ppszDestEnd, size_t* pRemainingCb, DWORD dwFlags);
	ECODE StringCbCatNEx(char* pszDest, size_t tDestCb, const char* pszSrc, size_t tAppendCb, char** ppszDestEnd, size_t* pRemainingCb, DWORD dwFlags);
	ECODE StringCbCatNEx(wchar_t* pszDest, size_t tDestCb, const wchar_t* pszSrc, size_t tAppendCb, wchar_t** ppszDestEnd, size_t* pRemainingCb, DWORD dwFlags);
	ECODE StringCbPrintfEx(char* pszDest, size_t tDestCb, char** ppszDestEnd, size_t* pRemainingCb, DWORD dwFlags, const char* pszFormat, ...);
	ECODE StringCbPrintfEx(wchar_t* pszDest, size_t tDestCb, wchar_t** ppszDestEnd, size_t* pRemainingCb, DWORD dwFlags, const wchar_t* pszFormat, ...);
	ECODE StringCbVPrintfEx(char* pszDest, size_t tDestCb, char** ppszDestEnd, size_t* pRemainingCb, DWORD dwFlags, const char* pszFormat, va_list vaList);
	ECODE StringCbVPrintfEx(wchar_t* pszDest, size_t tDestCb, wchar_t** ppszDestEnd, size_t* pRemainingCb, DWORD dwFlags, const wchar_t* pszFormat, va_list vaList);
	ECODE StringCbGetsEx(char* pszDest, size_t tDestCb, char** ppszDestEnd, size_t* pRemainingCb, DWORD dwFlags);
	ECODE StringCbGetsEx(wchar_t* pszDest, size_t tDestCb, wchar_t** ppszDestEnd, size_t* pRemainingCb, DWORD dwFlags);
}
