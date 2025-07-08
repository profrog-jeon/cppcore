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

	typedef std::vector<std::tstring>	CTStringVec;
}
