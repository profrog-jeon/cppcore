#pragma once

#include "../__Common/Type.h"

#include <string>
#include <vector>

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	enum eTokenType
	{
		TT_UNKNOWN,
		TT_BRACE_OPEN,
		TT_BRACE_CLOSE,
		TT_SQURE_BRACKET_OPEN,
		TT_SQURE_BRACKET_CLOSE,
		TT_COMMA,
		TT_COLON,
		TT_KEY,
		TT_VALUE
	};

	//////////////////////////////////////////////////////////////////////////
	struct sTokenA
	{
		eTokenType nType;
		std::string strValue;
		std::string strParsingStack;

		sTokenA(std::string strInValue) : nType(TT_UNKNOWN), strValue(strInValue) {}
		sTokenA(eTokenType nInType, std::string strInValue) : nType(nInType), strValue(strInValue) {}
	};
	struct sTokenW
	{
		eTokenType nType;
		std::wstring strValue;
		std::wstring strParsingStack;

		sTokenW(std::wstring strInValue) : nType(TT_UNKNOWN), strValue(strInValue) {}
		sTokenW(eTokenType nInType, std::wstring strInValue) : nType(nInType), strValue(strInValue) {}
	};

	//////////////////////////////////////////////////////////////////////////
	struct sChunkA
	{
		std::string strKey;
		std::vector<std::string> vecToken;
		void Clear()
		{
			strKey.clear();
			vecToken.clear();
		}
	};
	struct sChunkW
	{
		std::wstring strKey;
		std::vector<std::wstring> vecToken;
		void Clear()
		{
			strKey.clear();
			vecToken.clear();
		}
	};

	//////////////////////////////////////////////////////////////////////////
	typedef std::vector<sTokenA>  CTokenVecA;
	typedef std::vector<sTokenW>  CTokenVecW;
	typedef std::vector<sChunkA>  CChunkVecA;
	typedef std::vector<sChunkW>  CChunkVecW;
	typedef std::vector<std::tstring>	CTStringVec;
}
