#include "stdafx.h"
#include "HelperFunc.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	void TokenToVector(std::tstring& strContext, std::tstring strDelimiter, std::vector<std::tstring>& outTokenVec)
	{
		int nOffset = 0;

		while(nOffset >= 0)
		{
			std::tstring strToken = Tokenize(strContext, strDelimiter, nOffset);
			outTokenVec.push_back(strToken);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void TokenToVector(std::tstring& strContext, const TCHAR cSeperator, const TCHAR cQuotator, std::vector<std::tstring>& outTokenVec)
	{
		if( strContext.empty() )
			return;

		size_t tLength = strContext.length();
		LPCTSTR pszContext = strContext.c_str();

		size_t tLastPos = 0;
		bool bInsideQuotation = false;

		size_t i;
		for(i=0; i<tLength; i++)
		{
			if( pszContext[i] == cQuotator )
			{
				if( !bInsideQuotation )
				{
					bInsideQuotation = true;
					continue;
				}

				if( (i+1) < tLength && pszContext[i+1] == cQuotator )
				{
					i++;
					continue;
				}

				bInsideQuotation = false;
			}

			if( pszContext[i] == cSeperator )
			{
				if( bInsideQuotation )
					continue;

				outTokenVec.push_back(strContext.substr(tLastPos, i - tLastPos));
				tLastPos = i+1;
			}
		}

		outTokenVec.push_back(strContext.substr(tLastPos));
	}

	//////////////////////////////////////////////////////////////////////////
	void TokenToVectorByExactDelimiter(std::tstring& strContext, std::tstring strExactDelimiter, std::vector<std::tstring>& outTokenVec)
	{
		size_t tOffset = 0;

		while(1)
		{
			size_t tIndex = strContext.find(strExactDelimiter, tOffset);
			if( std::tstring::npos == tIndex )
				break;

			std::tstring strToken = strContext.substr(tOffset, (int)tIndex - tOffset);
			tOffset = tIndex + strExactDelimiter.length();

			outTokenVec.push_back(strToken);
		}

		outTokenVec.push_back(strContext.substr(tOffset));
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring WrapupSpecialChar(std::tstring strValue, TCHAR cSeperator, TCHAR cQuotator)
	{
		if( std::tstring::npos == strValue.find(cSeperator) && std::tstring::npos == strValue.find(cQuotator) )
			return strValue;

		TCHAR tzTarget[2] = { cQuotator, 0 };
		TCHAR tzReplace[3] = { cQuotator, cQuotator, 0 };
		Replace(strValue, tzTarget, tzReplace);
		return Format(TEXT("\"%s\""), strValue.c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring StripSpecialChar(std::tstring strValue, TCHAR cSeperator, TCHAR cQuotator)
	{
		Trim(strValue);

		size_t tLength = strValue.length();
		if( tLength < 2 )
			return strValue;

		// strip outter quotation
		if( strValue.at(0) == cQuotator && strValue.at(tLength-1) == cQuotator )
			strValue = strValue.substr(1, tLength - 2);

		TCHAR tzTarget[3] = { cQuotator, cQuotator, 0 };
		TCHAR tzReplace[2] = { cQuotator, 0 };
		Replace(strValue, tzTarget, tzReplace);
		return strValue;
	}
}

