#include "stdafx.h"
#include "StdStringLegacy.h"
#include "POSIX.h"
#include <stack>
#include <locale.h>

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	static const size_t g_tCharSize = sizeof(TCHAR);
    
    struct __INTERNAL_ST_DEFAULT_LOCALE_FOR_MAC
    {
        __INTERNAL_ST_DEFAULT_LOCALE_FOR_MAC(void)
        {
            setlocale(LC_ALL, "ko_KR");
        }
    };
#if defined(__APPLE__) && defined(UNICODE)
    __INTERNAL_ST_DEFAULT_LOCALE_FOR_MAC g_GlobalLocale;
#endif

	//////////////////////////////////////////////////////////////////////////
	bool IsWhiteSpace(TCHAR cChar)
	{
		if( TEXT(' ') == cChar )
			return true;
		if( TEXT('\t') == cChar )
			return true;
		if( TEXT('\n') == cChar )
			return true;
		if( TEXT('\r') == cChar )
			return true;
		if( TEXT('\b') == cChar )
			return true;
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	int SafeStrLen(LPCTSTR pszSrc, size_t tMaxCch)
	{
		size_t i;
		for(i=0; i<tMaxCch; i++)
		{
			if( 0 == pszSrc[i] )
				break;
		}
		return (int)i;
	}

	//////////////////////////////////////////////////////////////////////////
	TCHAR* SafeStrCpy(LPTSTR pszDest, size_t tDestCch, LPCTSTR pszSrc)
	{
		if( 0 == tDestCch )
			return pszDest;

		size_t tSrcCch = SafeStrLen(pszSrc, tDestCch);
		size_t tCopyLen = MIN(tSrcCch, tDestCch);
		memcpy(pszDest, pszSrc, tCopyLen * g_tCharSize);

		size_t tNullIndex = MIN((tDestCch - 1), tCopyLen);
		pszDest[tNullIndex] = 0;
		return pszDest;
	}

	//////////////////////////////////////////////////////////////////////////
	TCHAR* SafeStrCat(LPTSTR pszDest, size_t tDestCch, LPCTSTR pszSrc)
	{
		if( 0 == tDestCch )
			return pszDest;

		size_t tDestLen = SafeStrLen(pszDest, tDestCch);
		return SafeStrCpy(pszDest + tDestLen, tDestCch - tDestLen, pszSrc);
	}

	//////////////////////////////////////////////////////////////////////////
	static inline bool StrExactMatchWorker(LPCTSTR pszDest, LPCTSTR pszSrc, size_t tLen)
	{
		size_t i;
		for(i=0; i<tLen; i++)
		{
			if( pszDest[i] != pszSrc[i] )
				return false;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	LPCTSTR SafeStrStr(LPCTSTR pszDest, size_t tDestCch, LPCTSTR pszSrc)
	{
		size_t tSrcCch = SafeStrLen(pszSrc, tDestCch);
		if( tDestCch < tSrcCch )
			return NULL;

		size_t nMaxDestSearchLen = tDestCch - tSrcCch + 1;
		size_t i;
		for(i=0; i<nMaxDestSearchLen; i++)
		{
			if( StrExactMatchWorker(pszDest+i, pszSrc, tSrcCch) )
				return pszDest + i;
		}
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	int SafeSPrintf(LPTSTR pszDest, size_t tDestCch, LPCTSTR pszFormat, ...)
	{
		va_list list;
		va_start(list, pszFormat);
		int nRet = SafeSVPrintf(pszDest, tDestCch, pszFormat, list);
		va_end(list);

		if( nRet < 0 )
		{
			pszDest[tDestCch-1] = 0;
			return -1;
		}
		return nRet;
	}

	//////////////////////////////////////////////////////////////////////////
	int SafeSVPrintf(LPTSTR pszDest, size_t tDestCch, LPCTSTR pszFormat, va_list vaList)
	{		
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
		int nRet = vsntprintf(pszDest, tDestCch, strFormat.c_str(), vaList);
#else
		int nRet = vsntprintf(pszDest, tDestCch, pszFormat, vaList);		
#endif
		if( nRet < 0 )
			return nRet;	// Windows returns -1, when buffer is out of memory
		if( nRet >= (int)tDestCch )	// Linux returns required buffer size, when buffer is out of memory
			return -1;
		return nRet;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t SafeFindStr(LPCTSTR pszDest, size_t tCchDest, LPCTSTR pszKey, size_t tPos)
	{
		const size_t tTCharSize = sizeof(TCHAR);
		size_t tCchKey = SafeStrLen(pszKey, tCchDest);
		if( 0 == tCchKey )
			return -1;
		if (tCchKey == tCchDest)
			return memcmp(pszDest, pszKey, tCchKey + tTCharSize) == 0? 0 : -1;

		size_t nMaxDestSearchLen = tCchDest - tCchKey + 1;
		size_t i;
		for(i=tPos; i<nMaxDestSearchLen; i++)
		{
			if( StrExactMatchWorker(pszDest+i, pszKey, tCchKey) )
				return i;
		}
		return -1;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t SafeReverseFindStr(LPCTSTR pszDest, size_t tCchDest, LPCTSTR pszKey, size_t tPos)
	{
		size_t tCchKey = SafeStrLen(pszKey, tCchDest);
		if( tCchDest < tCchKey )
			return -1;

		size_t tMaxDestSearchLen = tCchDest - tCchKey + 1;
		int i;
		for(i=(int)MIN(tPos, tMaxDestSearchLen); i>=0; i--)
		{
			if( StrExactMatchWorker(pszDest+i, pszKey, tCchKey) )
				return i;
		}
		return -1;
	}

	//////////////////////////////////////////////////////////////////////////
	int SafeStrCmp(LPCTSTR pszSrc, LPCTSTR pszTarget, size_t tMaxCch)
	{
		DWORD i;
		for(i=0; i<tMaxCch && pszSrc[i] && pszTarget[i]; i++)
		{
			TCHAR btSrc = pszSrc[i];
			TCHAR btTarget = pszTarget[i];
			if( btSrc < btTarget )
				return -1;
			if( btSrc > btTarget )
				return 1;
		}

		if( tMaxCch == i )
			return 0;

		if( pszSrc[i] < pszTarget[i] )
			return -1;
		if( pszSrc[i] > pszTarget[i] )
			return 1;
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	static inline void TokenizeWildCardPattern(std::tstring strWildCard, std::vector<std::tstring>& vecTokens)
	{
		static const std::tstring s_WildCards = TEXT("?*");
		size_t tBeginIndex = 0;
		size_t i;
		for(i=0; i<strWildCard.size(); i++)
		{
			const TCHAR cCurChar = strWildCard.at(i);
			if( std::tstring::npos == s_WildCards.find(cCurChar) )
				continue;

			if( tBeginIndex != i )
				vecTokens.push_back(strWildCard.substr(tBeginIndex, i-tBeginIndex));				

			TCHAR szToken[2] = { cCurChar, 0 };
			vecTokens.push_back(szToken);
			tBeginIndex = i + 1;
			continue;
		}

		if( tBeginIndex != i )
			vecTokens.push_back(strWildCard.substr(tBeginIndex));
	}

	//////////////////////////////////////////////////////////////////////////
	struct ST_WILDCARDSEARCH
	{
		size_t tWildCardPos;
		size_t tContextPos;

		ST_WILDCARDSEARCH(void) : tWildCardPos(0), tContextPos(0)	{}
		ST_WILDCARDSEARCH(size_t w, size_t c) : tWildCardPos(w), tContextPos(c)	{}
	};

	//////////////////////////////////////////////////////////////////////////
	bool SafeStrCmpWithWildcard(LPCTSTR pszDest, size_t tDestCch, LPCTSTR pszPattern)
	{
		std::vector<std::tstring> vecPatternTokens;
		TokenizeWildCardPattern(pszPattern, vecPatternTokens);

		std::stack<ST_WILDCARDSEARCH> stackSearchingScenario;
		stackSearchingScenario.push(ST_WILDCARDSEARCH(0, 0));
		while(!stackSearchingScenario.empty())
		{
			ST_WILDCARDSEARCH stCur = stackSearchingScenario.top();
			stackSearchingScenario.pop();

			bool bAsteriskMark = false;
			size_t tQuestionMarkCount = 0;

			for(; stCur.tWildCardPos<vecPatternTokens.size(); stCur.tWildCardPos++)
			{
				const std::tstring& strPatternToken = vecPatternTokens[stCur.tWildCardPos];
				if( TEXT("*") == strPatternToken )
				{
					bAsteriskMark = true;
					continue;
				}

				if( TEXT("?") == strPatternToken )
				{
					if( bAsteriskMark )
					{
						tQuestionMarkCount++;
						continue;
					}

					// failure, one charactor wildcard is exceeding context length
					if( stCur.tContextPos + 1 > tDestCch )
						break;
					stCur.tContextPos++;
					continue;
				}

				if( bAsteriskMark )
				{
					// failure, question mark was exceeded remaining context length
					if( stCur.tContextPos + tQuestionMarkCount > tDestCch )
						break;

					stCur.tContextPos += tQuestionMarkCount;
					tQuestionMarkCount = 0;

					do
					{
						int nIndex = SafeFindStr(pszDest + stCur.tContextPos, tDestCch - stCur.tContextPos, strPatternToken.c_str());
						if( nIndex < 0 )
							break;

						stCur.tContextPos += nIndex;
						stackSearchingScenario.push(stCur);
						stCur.tContextPos += strPatternToken.length();
					}	while(stCur.tContextPos < tDestCch);
					break;
				}

				// failure, token text is exceeding context length
				if( stCur.tContextPos + strPatternToken.length() > tDestCch )
					break;

				// failure, token text is matched to context
				if( SafeStrCmp(pszDest + stCur.tContextPos, strPatternToken.c_str(), strPatternToken.length()) )
					break;

				stCur.tContextPos += strPatternToken.length();
			}

			if( stCur.tWildCardPos == vecPatternTokens.size() )
			{
				if( stCur.tContextPos == tDestCch )
					return true;

				if( bAsteriskMark && (stCur.tContextPos + tQuestionMarkCount <= tDestCch) )
					return true;
			}
		}

		return false;
	}
}	
