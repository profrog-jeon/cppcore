#include "stdafx.h"
#include "STLString.h"
#include "StdString.h"
#include "StdStringLegacy.h"

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
	std::tstring Replace(LPCTSTR pszContext, std::tstring strTarget, std::tstring strReplace)
	{
		std::tstring strContext(pszContext);

		size_t tPos = 0;
		while((tPos = strContext.find(strTarget, tPos)) != std::string::npos )
		{
			strContext.replace(tPos, strTarget.length(), strReplace.c_str()); 
			tPos += strReplace.length();
		}

		return strContext;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring& Replace(std::tstring& strContext, std::tstring strTarget, std::tstring strReplace)
	{
		size_t tPos = 0;
		while((tPos = strContext.find(strTarget, tPos)) != std::string::npos )
		{
			strContext.replace(tPos, strTarget.length(), strReplace.c_str()); 
			tPos += strReplace.length();
		}

		return strContext;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring MakeLower(LPCTSTR pszContext)
	{
		std::tstring strContext(pszContext);

		size_t i = 0;
		for (i=0; i<strContext.length(); i++)
		{
			if( strContext[i] < TEXT('A') )
				continue;

			if( strContext[i] > TEXT('Z') )
				continue;

			strContext[i] += (TEXT('a') - TEXT('A'));
		}
		return strContext;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring MakeLower(std::tstring strContext)
	{
		size_t i = 0;
		for (i=0; i<strContext.length(); i++)
		{
			if( strContext[i] < TEXT('A') )
				continue;

			if( strContext[i] > TEXT('Z') )
				continue;

			strContext[i] += (TEXT('a') - TEXT('A'));
		}
		return strContext;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring MakeUpper(LPCTSTR pszContext)
	{
		std::tstring strContext(pszContext);

		size_t i = 0;
		for (i=0; i<strContext.length(); i++)
		{
			if( strContext[i] < TEXT('a') )
				continue;

			if( strContext[i] > TEXT('z') )
				continue;

			strContext[i] -= TEXT('a') - TEXT('A');
		}
		return strContext;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring MakeUpper(std::tstring strContext)
	{
		size_t i = 0;
		for (i=0; i<strContext.length(); i++)
		{
			if( strContext[i] < TEXT('a') )
				continue;

			if( strContext[i] > TEXT('z') )
				continue;

			strContext[i] -= TEXT('a') - TEXT('A');
		}
		return strContext;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring& BecomeLower(std::tstring& strContext)
	{
		size_t i = 0;
		for (i = 0; i<strContext.length(); i++)
		{
			if (strContext[i] < TEXT('A'))
				continue;

			if (strContext[i] > TEXT('Z'))
				continue;

			strContext[i] += (TEXT('a') - TEXT('A'));
		}
		return strContext;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring& BecomeUpper(std::tstring& strContext)
	{
		size_t i = 0;
		for (i = 0; i<strContext.length(); i++)
		{
			if (strContext[i] < TEXT('a'))
				continue;

			if (strContext[i] > TEXT('z'))
				continue;

			strContext[i] -= TEXT('a') - TEXT('A');
		}
		return strContext;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring Trim(LPCTSTR pszContext, LPCTSTR pszWhiteSpace)
	{
		std::tstring strContext(pszContext);
		strContext = TrimLeft(strContext, pszWhiteSpace);
		strContext = TrimRight(strContext, pszWhiteSpace);
		return strContext;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring& Trim(std::tstring& strContext, LPCTSTR pszWhiteSpace)
	{
		TrimLeft(strContext, pszWhiteSpace);
		TrimRight(strContext, pszWhiteSpace);
		return strContext;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring TrimLeft(LPCTSTR pszContext, LPCTSTR pszWhiteSpace)
	{
		std::tstring strContext(pszContext);

		size_t nPos = 0;
		nPos = strContext.find_first_not_of(pszWhiteSpace, 0);
		if( std::string::npos == nPos )
			strContext.clear();
		else
			strContext = strContext.substr(nPos);

		return strContext;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring& TrimLeft(std::tstring& strContext, LPCTSTR pszWhiteSpace)
	{
		size_t nPos = 0;
		nPos = strContext.find_first_not_of(pszWhiteSpace, 0);
		if( std::string::npos == nPos )
			strContext.clear();
		else
			strContext = strContext.substr(nPos);

		return strContext;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring TrimRight(LPCTSTR pszContext, LPCTSTR pszWhiteSpace)
	{
		std::tstring strContext(pszContext);

		size_t nPos = 0;
		nPos = strContext.find_last_not_of(pszWhiteSpace, strContext.length());
		if( std::string::npos == nPos )
			strContext.clear();
		else
			strContext.resize(nPos+1);

		return strContext;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring& TrimRight(std::tstring& strContext, LPCTSTR pszWhiteSpace)
	{
		size_t nPos = 0;
		nPos = strContext.find_last_not_of(pszWhiteSpace, strContext.length());
		if( std::string::npos == nPos )
			strContext.clear();
		else
			strContext.resize(nPos+1);

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
