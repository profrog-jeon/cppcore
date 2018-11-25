#pragma once

#include <string>
#include <vector>
#include <map>

#include "FmtTypes.h"

//////////////////////////////////////////////////////////////////////////
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
	const TCHAR* GetTokenTypeString(eTokenType nType);

	//////////////////////////////////////////////////////////////////////////
	struct sToken
	{
		eTokenType nType;
		std::tstring strValue;
		std::tstring strParsingStack;

		sToken(std::tstring strInValue) : nType(TT_UNKNOWN), strValue(strInValue)	{}
		sToken(eTokenType nInType, std::tstring strInValue) : nType(nInType), strValue(strInValue)	{}
	};
	typedef std::vector<sToken>  CTokenVec;

	//////////////////////////////////////////////////////////////////////////
	struct sChunk
	{
		std::tstring strKey;
		CTStringVec vecToken;
		void Clear()
		{
			strKey.clear();
			vecToken.clear();
		}
	};
	typedef std::vector<sChunk>  CChunkVec;

	//////////////////////////////////////////////////////////////////////////
	std::tstring ConvertToJsonString(const std::tstring& strValue);
	std::tstring RestoreFromJsonString(const std::tstring& strValue);

	//////////////////////////////////////////////////////////////////////////
	size_t Scan(const TCHAR* pszContext, CTStringVec& vecToken);
	size_t Scan(std::tstring& strContext, CTStringVec& vecToken);
	bool Parse(CTStringVec& vecToken, CTokenVec& vecJsonToken, std::tstring& strErrMsgDump);
	size_t Build(CTokenVec& vecJsonToken, CChunkVec& vecJsonChunk);
	size_t ExpandArray(size_t tIndex, CChunkVec& vecJsonChunk);
}
