#include "stdafx.h"
#include "StdString.h"
#include "StdStringLegacy.h"
#include "POSIX.h"
#include <stack>

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	static const size_t g_tCharSize = sizeof(TCHAR);
	static const size_t g_tMaxCchLength = 0x7FFFFFFF;

	//////////////////////////////////////////////////////////////////////////
	inline ECODE StringCchCopyWorker(LPTSTR pszDest, size_t tDestCch, LPCTSTR pszSrc, size_t tCopyCch)
	{
		if( (0 == tDestCch) || (tDestCch > g_tMaxCchLength) )
			return EC_INVALID_ARGUMENT;

		size_t tSrcCch = 0;
		ECODE nRet = StringCchLength(pszSrc, g_tMaxCchLength, &tSrcCch);
		if( EC_SUCCESS != nRet )
			return nRet;

		size_t tFinalSrcCch = MIN(tSrcCch, tCopyCch);
		size_t tFinalCopyCch = MIN(tFinalSrcCch, tDestCch-1);
		memcpy(pszDest, pszSrc, tFinalCopyCch * g_tCharSize);

		pszDest[tFinalCopyCch] = 0;

		if( tFinalSrcCch >= tDestCch )
			return EC_NOT_ENOUGH_MEMORY;

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	inline ECODE StringCchVPrintfWorker(LPTSTR pszDest, size_t tDestCch, LPCTSTR pszFormat, va_list vaList)
	{
		if( (0 == tDestCch) || (tDestCch > 2147483647) )
			return EC_INVALID_ARGUMENT;

#if !defined(_MSC_VER) & defined(UNICODE)
		std::tstring strFormat(pszFormat);
		size_t tLength = strFormat.length();

		size_t i;
		for(i=0; (i+1)<tLength; i++)
		{
			if( TEXT('%') != strFormat[i] )
				continue;

			switch(strFormat[i+1])
			{
			case TEXT('s'):		strFormat[i+1] = TEXT('S');			break;
			case TEXT('S'):		strFormat[i+1] = TEXT('s');			break;
			}
		}
		int nWritten = vsntprintf(pszDest, tDestCch, strFormat.c_str(), vaList);
#else
		int nWritten = vsntprintf(pszDest, tDestCch, pszFormat, vaList);		
#endif

		size_t tNullIndex = tDestCch-1;
		pszDest[tNullIndex] = 0;

		// Windows returns -1, when buffer is out of memory
		if( nWritten < 0 )
			return EC_NOT_ENOUGH_MEMORY;

		// Linux returns required buffer size, when buffer is out of memory
		if( nWritten >= (int)tDestCch )
			return EC_NOT_ENOUGH_MEMORY;

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	inline ECODE StringCchGetsWorker(LPTSTR pszDest, size_t tDestCch)
	{
		if( 0 == tDestCch )
			return EC_INVALID_ARGUMENT;

		size_t i;
		for(i=0; i<tDestCch-1; i++)
		{
			TCHAR ch = gettc(stdin);

			if( -1 == ch )
				break;

			if( '\n' == ch )
				break;

			pszDest[i] = ch;
		}

		pszDest[i] = 0;
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	inline ECODE StringCchLengthWorker(LPCTSTR pszContext, size_t tContextBuffLen, size_t* pLengthCch) 
	{
		if( pLengthCch )
			*pLengthCch = 0;

		if( 0 == tContextBuffLen )
			return EC_INVALID_ARGUMENT;

		size_t i;
		for(i=0; i<tContextBuffLen && *pszContext; i++, pszContext++)
			;
		if( *pszContext )
			return EC_INVALID_ARGUMENT;

		if( tContextBuffLen == i )
			return EC_INVALID_ARGUMENT;

		if( pLengthCch )
			*pLengthCch = i;
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE StringCchCopy(LPTSTR pszDest, size_t tDestCch, LPCTSTR pszSrc)
	{
		return StringCchCopyWorker(pszDest, tDestCch, pszSrc, g_tMaxCchLength);
	}

	ECODE StringCchCopyN(LPTSTR pszDest, size_t tDestCch, LPCTSTR pszSrc, size_t tCopyCch)
	{
		return StringCchCopyWorker(pszDest, tDestCch, pszSrc, tCopyCch);
	}

	ECODE StringCchCat(LPTSTR pszDest, size_t tDestCch, LPCTSTR pszSrc)
	{
		size_t tPrevLength = SafeStrLen(pszDest, tDestCch);
		pszDest += tPrevLength;
		tDestCch -= tPrevLength;
		return StringCchCopyWorker(pszDest, tDestCch, pszSrc, g_tMaxCchLength);
	}

	ECODE StringCchCatN(LPTSTR pszDest, size_t tDestCch, LPCTSTR pszSrc, size_t tAppendCch)
	{
		size_t tPrevLength = SafeStrLen(pszDest, tDestCch);
		pszDest += tPrevLength;
		tDestCch -= tPrevLength;
		return StringCchCopyWorker(pszDest, tDestCch, pszSrc, tAppendCch);
	}

	ECODE StringCchPrintf(LPTSTR pszDest, size_t tDestCch, LPCTSTR pszFormat, ...)
	{
		va_list list;
		va_start(list, pszFormat);
		ECODE nRet = StringCchVPrintfWorker(pszDest, tDestCch, pszFormat, list);
		va_end(list);
		return nRet;
	}

	ECODE StringCchVPrintf(LPTSTR pszDest, size_t tDestCch, LPCTSTR pszFormat, va_list vaList)
	{
		return StringCchVPrintfWorker(pszDest, tDestCch, pszFormat, vaList);
	}

	ECODE StringCchGets(LPTSTR pszDest, size_t tDestCch)
	{
		return StringCchGetsWorker(pszDest, tDestCch);
	}

	ECODE StringCchLength(LPCTSTR pszContext, size_t tMaxCch, size_t* pLengthCch)
	{
		return StringCchLengthWorker(pszContext, tMaxCch, pLengthCch);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE StringCbCopy(LPTSTR pszDest, size_t tDestCb, LPCTSTR pszSrc)
	{
		return StringCchCopyWorker(pszDest, tDestCb / g_tCharSize, pszSrc, g_tMaxCchLength);
	}

	ECODE StringCbCopyN(LPTSTR pszDest, size_t tDestCch, LPCTSTR pszSrc, size_t tCopyCch)
	{
		size_t tPrevLength = SafeStrLen(pszDest, tDestCch);
		pszDest += tPrevLength;
		tDestCch -= tPrevLength;
		return StringCchCopyWorker(pszDest, tDestCch / g_tCharSize, pszSrc, tCopyCch);
	}

	ECODE StringCbCat(LPTSTR pszDest, size_t tDestCb, LPCTSTR pszSrc)
	{
		size_t tPrevLength = SafeStrLen(pszDest, tDestCb / g_tCharSize);
		pszDest += tPrevLength;
		tDestCb -= tPrevLength * g_tCharSize;
		return StringCchCopyWorker(pszDest, tDestCb / g_tCharSize, pszSrc, g_tMaxCchLength);
	}

	ECODE StringCbCatN(LPTSTR pszDest, size_t tDestCb, LPCTSTR pszSrc, size_t tAppendCb)
	{
		size_t tPrevLength = SafeStrLen(pszDest, tDestCb / g_tCharSize);
		pszDest += tPrevLength;
		tDestCb -= tPrevLength * g_tCharSize;
		return StringCchCopyWorker(pszDest, tDestCb / g_tCharSize, pszSrc, tAppendCb / g_tCharSize);
	}

	ECODE StringCbPrintf(LPTSTR pszDest, size_t tDestCb, LPCTSTR pszFormat, ...)
	{
		va_list list;
		va_start(list, pszFormat);
		ECODE nRet = StringCchVPrintfWorker(pszDest, tDestCb / g_tCharSize, pszFormat, list);
		va_end(list);
		return nRet;
	}

	ECODE StringCbVPrintf(LPTSTR pszDest, size_t tDestCb, LPCTSTR pszFormat, va_list vaList)
	{
		return StringCchVPrintfWorker(pszDest, tDestCb / g_tCharSize, pszFormat, vaList);
	}

	ECODE StringCbGets(LPTSTR pszDest, size_t tDestCb)
	{
		return StringCchGetsWorker(pszDest, tDestCb / g_tCharSize);
	}

	ECODE StringCbLength(LPCTSTR pszContext, size_t tMaxCb, size_t* pLengthCch)
	{
		return StringCchLengthWorker(pszContext, tMaxCb / g_tCharSize, pLengthCch);
	}

	//////////////////////////////////////////////////////////////////////////
	inline ECODE StringExWorker(LPTSTR pszDest, size_t tDestCch, ECODE nRet, LPTSTR* ppszDestEnd, size_t* pRemainCch, DWORD dwFlags)
	{
		size_t tNullIndex = SafeStrLen(pszDest, tDestCch);
		size_t tRemainCch = tDestCch - tNullIndex;
		LPTSTR pszDestEnd = pszDest + tNullIndex;
		if( EC_SUCCESS == nRet)
		{
			if( (STRSAFE_FILL_BEHIND_NULL_ & dwFlags) && tRemainCch )
				memset(pszDestEnd + 1, dwFlags & 0xFF, (tRemainCch - 1) * g_tCharSize);
			if( ppszDestEnd )
				*ppszDestEnd = (LPTSTR)pszDestEnd;
			if( pRemainCch )
				*pRemainCch = tRemainCch;
		}
		else
		{
			if( STRSAFE_FILL_ON_FAILURE_ & dwFlags )
			{
				memset(pszDest, dwFlags & 0xFF, tDestCch * g_tCharSize);
				pszDest[tDestCch - 1] = 0;
			}

			if( STRSAFE_NULL_ON_FAILURE_ & dwFlags )
				*pszDest = 0;

			if( STRSAFE_NO_TRUNCATION_ & dwFlags )
				*pszDest = 0;
		}
		return nRet;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE StringCchCopyEx(LPTSTR pszDest, size_t tDestCch, LPCTSTR pszSrc, LPTSTR* ppszDestEnd, size_t* pRemainingCch, DWORD dwFlags)
	{
		if( (STRSAFE_IGNORE_NULLS_ & dwFlags) && (NULL == pszSrc) )
			pszSrc = TEXT("");

		ECODE nRet = StringCchCopyWorker(pszDest, tDestCch, pszSrc, g_tMaxCchLength);
		return StringExWorker(pszDest, tDestCch, nRet, ppszDestEnd, pRemainingCch, dwFlags);
	}

	ECODE StringCchCopyNEx(LPTSTR pszDest, size_t tDestCch, LPCTSTR pszSrc, size_t tCopyCch, LPTSTR* ppszDestEnd, size_t* pRemainingCch, DWORD dwFlags)
	{
		if( (STRSAFE_IGNORE_NULLS_ & dwFlags) && (NULL == pszSrc) )
			pszSrc = TEXT("");

		ECODE nRet = StringCchCopyWorker(pszDest, tDestCch, pszSrc, tCopyCch);
		return StringExWorker(pszDest, tDestCch, nRet, ppszDestEnd, pRemainingCch, dwFlags);
	}

	ECODE StringCchCatEx(LPTSTR pszDest, size_t tDestCch, LPCTSTR pszSrc, LPTSTR* ppszDestEnd, size_t* pRemainingCch, DWORD dwFlags)
	{
		if( (STRSAFE_IGNORE_NULLS_ & dwFlags) && (NULL == pszSrc) )
			pszSrc = TEXT("");

		size_t tPrevLength = SafeStrLen(pszDest, tDestCch);
		pszDest += tPrevLength;
		tDestCch -= tPrevLength;

		ECODE nRet = StringCchCopyWorker(pszDest, tDestCch, pszSrc, g_tMaxCchLength);
		return StringExWorker(pszDest, tDestCch, nRet, ppszDestEnd, pRemainingCch, dwFlags);
	}

	ECODE StringCchCatNEx(LPTSTR pszDest, size_t tDestCch, LPCTSTR pszSrc, size_t tAppendCch, LPTSTR* ppszDestEnd, size_t* pRemainingCch, DWORD dwFlags)
	{
		if( (STRSAFE_IGNORE_NULLS_ & dwFlags) && (NULL == pszSrc) )
			pszSrc = TEXT("");

		size_t tPrevLength = SafeStrLen(pszDest, tDestCch);
		pszDest += tPrevLength;
		tDestCch -= tPrevLength;

		ECODE nRet = StringCchCopyWorker(pszDest, tDestCch, pszSrc, tAppendCch);
		return StringExWorker(pszDest, tDestCch, nRet, ppszDestEnd, pRemainingCch, dwFlags);
	}

	ECODE StringCchPrintfEx(LPTSTR pszDest, size_t tDestCch, LPTSTR* ppszDestEnd, size_t* pRemainingCch, DWORD dwFlags, LPCTSTR pszFormat, ...)
	{
		if( (STRSAFE_IGNORE_NULLS_ & dwFlags) && (NULL == pszFormat) )
			pszFormat = TEXT("");

		va_list list;
		va_start(list, pszFormat);
		ECODE nRet = StringCchVPrintfWorker(pszDest, tDestCch, pszFormat, list);
		va_end(list);
		return StringExWorker(pszDest, tDestCch, nRet, ppszDestEnd, pRemainingCch, dwFlags);
	}

	ECODE StringCchVPrintfEx(LPTSTR pszDest, size_t tDestCch, LPTSTR* ppszDestEnd, size_t* pRemainingCch, DWORD dwFlags, LPCTSTR pszFormat, va_list vaList)
	{
		if( (STRSAFE_IGNORE_NULLS_ & dwFlags) && (NULL == pszFormat) )
			pszFormat = TEXT("");

		ECODE nRet = StringCchVPrintfWorker(pszDest, tDestCch, pszFormat, vaList);
		return StringExWorker(pszDest, tDestCch, nRet, ppszDestEnd, pRemainingCch, dwFlags);
	}

	ECODE StringCchGetsEx(LPTSTR pszDest, size_t tDestCch, LPTSTR* ppszDestEnd, size_t* pRemainingCch, DWORD dwFlags)
	{
		ECODE nRet = StringCchGetsWorker(pszDest, tDestCch);
		return StringExWorker(pszDest, tDestCch, nRet, ppszDestEnd, pRemainingCch, dwFlags);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE StringCbCopyEx(LPTSTR pszDest, size_t tDestCb, LPCTSTR pszSrc, LPTSTR* ppszDestEnd, size_t* pRemainingCb, DWORD dwFlags)
	{
		if( (STRSAFE_IGNORE_NULLS_ & dwFlags) && (NULL == pszSrc) )
			pszSrc = TEXT("");

		ECODE nRet = StringCchCopyWorker(pszDest, tDestCb / g_tCharSize, pszSrc, g_tMaxCchLength);
		nRet = StringExWorker(pszDest, tDestCb / g_tCharSize, nRet, ppszDestEnd, pRemainingCb, dwFlags);
		if( pRemainingCb )
			*pRemainingCb *= g_tCharSize;
		return nRet;
	}

	ECODE StringCbCopyNEx(LPTSTR pszDest, size_t tDestCb, LPCTSTR pszSrc, size_t tCopyCb, LPTSTR* ppszDestEnd, size_t* pRemainingCb, DWORD dwFlags)
	{
		if( (STRSAFE_IGNORE_NULLS_ & dwFlags) && (NULL == pszSrc) )
			pszSrc = TEXT("");

		ECODE nRet = StringCchCopyWorker(pszDest, tDestCb / g_tCharSize, pszSrc, tCopyCb / g_tCharSize);
		nRet = StringExWorker(pszDest, tDestCb / g_tCharSize, nRet, ppszDestEnd, pRemainingCb, dwFlags);
		if( pRemainingCb )
			*pRemainingCb *= g_tCharSize;
		return nRet;
	}

	ECODE StringCbCatEx(LPTSTR pszDest, size_t tDestCb, LPCTSTR pszSrc, LPTSTR* ppszDestEnd, size_t* pRemainingCb, DWORD dwFlags)
	{
		if( (STRSAFE_IGNORE_NULLS_ & dwFlags) && (NULL == pszSrc) )
			pszSrc = TEXT("");

		size_t tPrevLength = SafeStrLen(pszDest, tDestCb / g_tCharSize);
		pszDest += tPrevLength;
		tDestCb -= tPrevLength * g_tCharSize;

		ECODE nRet = StringCchCopyWorker(pszDest, tDestCb / g_tCharSize, pszSrc, g_tMaxCchLength);
		nRet = StringExWorker(pszDest, tDestCb / g_tCharSize, nRet, ppszDestEnd, pRemainingCb, dwFlags);
		if( pRemainingCb )
			*pRemainingCb *= g_tCharSize;
		return nRet;
	}	

	ECODE StringCbCatNEx(LPTSTR pszDest, size_t tDestCb, LPCTSTR pszSrc, size_t tAppendCb, LPTSTR* ppszDestEnd, size_t* pRemainingCb, DWORD dwFlags)
	{
		if( (STRSAFE_IGNORE_NULLS_ & dwFlags) && (NULL == pszSrc) )
			pszSrc = TEXT("");

		size_t tPrevLength = SafeStrLen(pszDest, tDestCb / g_tCharSize);
		pszDest += tPrevLength;
		tDestCb -= tPrevLength * g_tCharSize;

		ECODE nRet = StringCchCopyWorker(pszDest, tDestCb / g_tCharSize, pszSrc, tAppendCb / g_tCharSize);
		nRet = StringExWorker(pszDest, tDestCb / g_tCharSize, nRet, ppszDestEnd, pRemainingCb, dwFlags);
		if( pRemainingCb )
			*pRemainingCb *= g_tCharSize;
		return nRet;
	}

	ECODE StringCbPrintfEx(LPTSTR pszDest, size_t tDestCb, LPTSTR* ppszDestEnd, size_t* pRemainingCb, DWORD dwFlags, LPCTSTR pszFormat, ...)
	{
		tDestCb /= g_tCharSize;
		if( (STRSAFE_IGNORE_NULLS_ & dwFlags) && (NULL == pszFormat) )
			pszFormat = TEXT("");

		va_list list;
		va_start(list, pszFormat);
		ECODE nRet = StringCchVPrintfWorker(pszDest, tDestCb / g_tCharSize, pszFormat, list);
		va_end(list);
		nRet = StringExWorker(pszDest, tDestCb / g_tCharSize, nRet, ppszDestEnd, pRemainingCb, dwFlags);
		if( pRemainingCb )
			*pRemainingCb *= g_tCharSize;
		return nRet;
	}

	ECODE StringCbVPrintfEx(LPTSTR pszDest, size_t tDestCb, LPTSTR* ppszDestEnd, size_t* pRemainingCb, DWORD dwFlags, LPCTSTR pszFormat, va_list vaList)
	{
		tDestCb /= g_tCharSize;
		if( (STRSAFE_IGNORE_NULLS_ & dwFlags) && (NULL == pszFormat) )
			pszFormat = TEXT("");

		ECODE nRet = StringCchVPrintfWorker(pszDest, tDestCb / g_tCharSize, pszFormat, vaList);
		nRet = StringExWorker(pszDest, tDestCb / g_tCharSize, nRet, ppszDestEnd, pRemainingCb, dwFlags);
		if( pRemainingCb )
			*pRemainingCb *= g_tCharSize;
		return nRet;
	}

	ECODE StringCbGetsEx(LPTSTR pszDest, size_t tDestCb, LPTSTR* ppszDestEnd, size_t* pRemainingCb, DWORD dwFlags)
	{
		tDestCb /= g_tCharSize;

		ECODE nRet = StringCchGetsWorker(pszDest, tDestCb / g_tCharSize);
		nRet = StringExWorker(pszDest, tDestCb / g_tCharSize, nRet, ppszDestEnd, pRemainingCb, dwFlags);
		if( pRemainingCb )
			*pRemainingCb *= g_tCharSize;
		return nRet;
	}
}
