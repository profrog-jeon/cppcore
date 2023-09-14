#pragma once

#include <string>
#include <vector>
#include "FmtTypes.h"

namespace fmt_internal
{
	struct sGroupingData
	{
		std::tstring strName;
		E_GROUPING_TYPE nType;
		size_t tTotalCount;
		size_t tSequence;

		sGroupingData(E_GROUPING_TYPE t, size_t tCount)
			: strName(), nType(t), tTotalCount(tCount), tSequence(0)	{}

		sGroupingData(std::tstring inName, E_GROUPING_TYPE t, size_t tCount)
			: strName(inName), nType(t), tTotalCount(tCount), tSequence(0)	{}
	};

	void TokenToVector(std::tstring& strContext, std::tstring strDelimiter, std::vector<std::tstring>& outTokenVec);
	void TokenToVector(std::tstring& strContext, const TCHAR cSeperator, const TCHAR cQuotator, std::vector<std::tstring>& outTokenVec);
	void TokenToVectorByExactDelimiter(std::tstring& strContext, std::tstring strExactDelimiter, std::vector<std::tstring>& outTokenVec);

	std::tstring WrapupSpecialChar(std::tstring strValue, TCHAR cSeperator, TCHAR cQuotator);
	std::tstring StripSpecialChar(std::tstring strValue, TCHAR cSeperator, TCHAR cQuotator);
}

