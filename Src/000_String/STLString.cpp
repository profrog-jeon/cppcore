#include "stdafx.h"
#include "STLString.h"
#include "StdString.h"
#include "StdStringLegacy.h"
#include <list>

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	std::tstring Format(LPCTSTR pszFormat, ...)
	{
		std::tstring strRet;

		int nRet = 0;
		do
		{
			size_t tPreSize = strRet.size();
			size_t tNewSize = (tPreSize == 0)? 512 : tPreSize * 2;
            
            // Exceptional escape: recommended that "do not use WideChar" in MacOSX.
            //                     It is a problem related with setlocale(LS_ALL, "ko_KR")
            if( tNewSize > 100 * 1024 * 1024 )
                return TEXT("This context is exceeded 100(MB).");
            
			strRet.resize(tNewSize);

			va_list list;
			va_start(list, pszFormat);
			nRet = SafeSVPrintf((TCHAR*)strRet.c_str(), tNewSize, pszFormat, list);
			va_end(list);

			if( nRet > 0 )
				strRet.resize(nRet);
		}	while(nRet < 0);
		
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	static inline std::tstring ReplaceWorker(std::tstring strContext, std::tstring strTarget, std::tstring strReplace)
	{
		if (strTarget.empty())
			return strContext;

		const size_t tContextLen = strContext.length();
		const int nTargetLen = (int)strTarget.length();
		const int nReplaceLen = (int)strReplace.length();

		std::list<size_t> listSrcIndex;
		size_t tPos = 0;
		while ((tPos = strContext.find(strTarget, tPos)) != std::string::npos)
		{
			listSrcIndex.push_back(tPos);
			tPos += nTargetLen;
		}

		size_t tNewSize = tContextLen - (nTargetLen - nReplaceLen) * listSrcIndex.size();
		std::tstring strRet;
		strRet.resize(tNewSize);

		LPCTSTR pszSource = strContext.c_str();
		LPTSTR pszDest = (LPTSTR)strRet.c_str();
		LPCTSTR pReplace = strReplace.c_str();
		size_t tSrcPos = 0;
		for (size_t tSrcIndex : listSrcIndex)
		{
			memcpy(pszDest, pszSource, (tSrcIndex - tSrcPos) * sizeof(TCHAR));
			pszSource += (tSrcIndex - tSrcPos);
			pszDest += (tSrcIndex - tSrcPos);

			memcpy(pszDest, pReplace, nReplaceLen * sizeof(TCHAR));
			pszSource += nTargetLen;
			pszDest += nReplaceLen;

			tSrcPos = tSrcIndex + nTargetLen;
		}

		const size_t tRemainedSize = tContextLen - tSrcPos;
		if (0 < tRemainedSize)
			memcpy(pszDest, pszSource, tRemainedSize * sizeof(TCHAR));
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring Replace(LPCTSTR pszContext, std::tstring strTarget, std::tstring strReplace)
	{
		std::tstring strContext(pszContext);
		return ReplaceWorker(strContext, strTarget, strReplace);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring& Replace(std::tstring& strContext, std::tstring strTarget, std::tstring strReplace)
	{
		return (strContext = ReplaceWorker(strContext, strTarget, strReplace));
	}

	//////////////////////////////////////////////////////////////////////////
	inline static std::tstring& MakeLowerWorker(std::tstring& strContext)
	{
		LPTSTR pContext = (LPTSTR)strContext.c_str();
		LPTSTR pTarget = pContext + strContext.length();
		for (; pContext < pTarget; pContext++)
		{
			if (*pContext < TEXT('A'))
				continue;

			if (*pContext > TEXT('Z'))
				continue;

			*pContext += 32;// TEXT('a') - TEXT('A');
		}
		return strContext;
	}

	//////////////////////////////////////////////////////////////////////////
	inline static std::tstring& MakeUpperWorker(std::tstring& strContext)
	{
		LPTSTR pContext = (LPTSTR)strContext.c_str();
		LPTSTR pTarget = pContext + strContext.length();
		for (; pContext < pTarget; pContext++)
		{
			if (*pContext < TEXT('a'))
				continue;

			if (*pContext > TEXT('z'))
				continue;

			*pContext -= 32;// TEXT('a') - TEXT('A');
		}
		return strContext;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring MakeLower(LPCTSTR pszContext)
	{
		std::tstring strTempContext(pszContext);
		return MakeLowerWorker(strTempContext);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring MakeLower(const std::tstring& strContext)
	{
		std::tstring strTempContext(strContext.c_str());
		return MakeLowerWorker(strTempContext);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring& BecomeLower(std::tstring& strContext)
	{
		return MakeLowerWorker(strContext);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring MakeUpper(LPCTSTR pszContext)
	{
		std::tstring strTempContext(pszContext);
		return MakeUpperWorker(strTempContext);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring MakeUpper(const std::tstring& strContext)
	{
		std::tstring strTempContext(strContext.c_str());
		return MakeUpperWorker(strTempContext);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring& BecomeUpper(std::tstring& strContext)
	{
		return MakeUpperWorker(strContext);
	}

	//////////////////////////////////////////////////////////////////////////
	inline static std::tstring& TrimLeftWorker(std::tstring& strContext, LPCTSTR pszWhiteSpace)
	{
		size_t nPos = 0;
		nPos = strContext.find_first_not_of(pszWhiteSpace, 0);
		if (std::string::npos == nPos)
			strContext.clear();
		else if(nPos != 0)
			strContext = strContext.substr(nPos);

		return strContext;
	}

	//////////////////////////////////////////////////////////////////////////
	inline static std::tstring& TrimRightWorker(std::tstring& strContext, LPCTSTR pszWhiteSpace)
	{
		const size_t tLength = strContext.length();

		size_t nPos = strContext.find_last_not_of(pszWhiteSpace, tLength);
		if (std::string::npos == nPos)
			strContext.clear();
		else if ((nPos + 1) != tLength)
			strContext.resize(nPos + 1);

		return strContext;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring TrimLeft(LPCTSTR pszContext, LPCTSTR pszWhiteSpace)
	{
		std::tstring strContext(pszContext);
		return TrimLeftWorker(strContext, pszWhiteSpace);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring& TrimLeft(std::tstring& strContext, LPCTSTR pszWhiteSpace)
	{
		return TrimLeftWorker(strContext, pszWhiteSpace);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring TrimRight(LPCTSTR pszContext, LPCTSTR pszWhiteSpace)
	{
		std::tstring strContext(pszContext);
		return TrimRightWorker(strContext, pszWhiteSpace);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring& TrimRight(std::tstring& strContext, LPCTSTR pszWhiteSpace)
	{
		return TrimRightWorker(strContext, pszWhiteSpace);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring Trim(LPCTSTR pszContext, LPCTSTR pszWhiteSpace)
	{
		std::tstring strContext(pszContext);
		strContext = TrimLeftWorker(strContext, pszWhiteSpace);
		strContext = TrimRightWorker(strContext, pszWhiteSpace);
		return strContext;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring& Trim(std::tstring& strContext, LPCTSTR pszWhiteSpace)
	{
		TrimLeftWorker(strContext, pszWhiteSpace);
		TrimRightWorker(strContext, pszWhiteSpace);
		return strContext;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring Tokenize(const std::tstring& strContext, int& nOffset)
	{
		LPCTSTR g_pszStringHelperWhiteSpaceT= TEXT(" \r\t\n");
		if( strContext.empty() )
			return TEXT("");

		int nStartIndex = (int)strContext.find_first_not_of(g_pszStringHelperWhiteSpaceT, nOffset);
		int nEndIndex = (int)strContext.find_first_of(g_pszStringHelperWhiteSpaceT, nStartIndex);

		if( nStartIndex == nEndIndex )
		{
			if( -1 == nStartIndex )
				nOffset = -1;
			else
				nOffset++;
			return TEXT("");
		}

		nOffset = nEndIndex;
		return strContext.substr(nStartIndex, nEndIndex - nStartIndex);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring Tokenize(const std::tstring& strContext, std::tstring strDelimiter, int& nOffset)
	{
		if( nOffset < 0 )
			return TEXT("");

		size_t tEndIndex = strContext.find_first_of(strDelimiter, nOffset);

		if( std::tstring::npos == tEndIndex )
		{
			std::tstring strRet = strContext.substr(nOffset);
			nOffset = -1;
			return strRet;
		}

		std::tstring strRet = strContext.substr(nOffset, tEndIndex - nOffset);
		nOffset = (int)tEndIndex + 1;
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t TokenizeToArray(std::tstring strContext, std::tstring strDelimiter, std::vector<std::tstring>& outTokens, bool bSkipEmpty)
	{
		if (std::tstring::npos == strContext.find_first_of(strDelimiter))
		{
			outTokens.push_back(Trim(strContext));
			return outTokens.size();
		}

		int nOffset = 0;

		do
		{
			std::tstring strToken = Tokenize(strContext, strDelimiter, nOffset);
			Trim(strToken);
			if( strToken.empty() && bSkipEmpty )
				continue;

			outTokens.push_back(strToken);
		}	while(nOffset >= 0);

		return outTokens.size();
	}

	//////////////////////////////////////////////////////////////////////////
	void Swap(std::tstring& strLeft, std::tstring& strRight)
	{
		std::tstring strTemp;
		strTemp		= strLeft;
		strLeft		= strRight;
		strRight	= strTemp;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring Split(const std::tstring& strContext, std::tstring strDelimiter, std::tstring* pstrValue)
	{
		int nIndex = (int)strContext.find(strDelimiter);
		if( nIndex < 0 )
			return strContext;

		if( pstrValue )
			(*pstrValue) = strContext.substr(nIndex+strDelimiter.length());
		return strContext.substr(0, nIndex);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring ReverseSplit(const std::tstring& strContext, std::tstring strDelimiter, std::tstring* pstrValue)
	{
		int nIndex = (int)strContext.rfind(strDelimiter);
		if (nIndex < 0)
			return strContext;

		if (pstrValue)
			(*pstrValue) = strContext.substr(nIndex + strDelimiter.length());
		return strContext.substr(0, nIndex);
	}

	//////////////////////////////////////////////////////////////////////////	
	std::tstring Left(const std::tstring& strContext, size_t tCount)
	{
		size_t tLength = strContext.length();
		size_t tTryCount = tLength < tCount ? tLength : tCount;
		return strContext.substr(0, tTryCount);
	}

	//////////////////////////////////////////////////////////////////////////	
	std::tstring Right(const std::tstring& strContext, size_t tCount)
	{
		size_t tLength = strContext.length();
		size_t tTryCount = tLength < tCount? tLength : tCount;
		return strContext.substr(tLength - tTryCount);
	}

	//////////////////////////////////////////////////////////////////////////
	bool StrCmpWithWildcard(std::tstring strContext, std::tstring strPattern)
	{
		return SafeStrCmpWithWildcard(strContext.c_str(), strContext.length(), strPattern.c_str());
	}
}
