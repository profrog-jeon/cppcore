#pragma once

#include "../__Common/Type.h"

#include <string>
#include <vector>

#ifdef UNICODE
#define fmt_internal	fmt_internalW
#else
#define fmt_internal	fmt_internalA
#endif

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	enum E_GROUPING_TYPE
	{
		GT_ROOT, 
		GT_DICTIONARY, 
		GT_ARRAY, 
		GT_OBJECT
	};

	//////////////////////////////////////////////////////////////////////////
	enum E_TOKEN_TYPE
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
	struct ST_JSON_CHUNK
	{
		E_TOKEN_TYPE nType;
		std::tstring strValue;
		std::tstring strParsingStack;

		ST_JSON_CHUNK(std::tstring strInValue) : nType(TT_UNKNOWN), strValue(strInValue) {}
		ST_JSON_CHUNK(E_TOKEN_TYPE nInType, std::tstring strInValue) : nType(nInType), strValue(strInValue) {}
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_JSON_TOKEN
	{
		std::tstring strKey;
		std::vector<std::tstring> vecToken;
		void Clear()
		{
			strKey.clear();
			vecToken.clear();
		}
	};

	//////////////////////////////////////////////////////////////////////////
	typedef std::vector<ST_JSON_CHUNK>  CTokenVec;
	typedef std::vector<ST_JSON_TOKEN>  CChunkVec;
	typedef std::vector<std::tstring>	CTStringVec;
}
