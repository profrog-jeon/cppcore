#pragma once

#include "../__Common/Type.h"

namespace core
{
#define TZ_MAX_CHARS	50

	//////////////////////////////////////////////////////////////////////////
	struct ST_TZ_TYPE
	{
		int		nGMTOffset	;
		BYTE	IsDST		;
		BYTE	btAbbrind	;
		BYTE	IsGMT		;
		BYTE	IsSTD		;
	};

	//////////////////////////////////////////////////////////////////////////
	typedef std::map<DWORD, ST_TZ_TYPE>		CTZTypeMap;
	typedef CTZTypeMap::iterator			CTZTypeMapIt;
	struct ST_TZ_INFO
	{
		char szAbbreviation[TZ_MAX_CHARS];
		std::vector<ST_TZ_TYPE> vecType;
		CTZTypeMap mapTransitTime;
	};

	ECODE ParseTZFile(LPCSTR pszFileName, ST_TZ_INFO& outInfo);
	void PrintOut(ST_TZ_INFO& info);
}
