#pragma once

#include "../__Common/ErrorCode.h"

#ifdef UNICODE
#define GetCurrentBOM	GetCurrentBOMW
#else
#define GetCurrentBOM	GetCurrentBOMA
#endif

namespace core
{
	enum E_BOM_TYPE
	{
		BOM_UTF32_BE		= 0,
		BOM_UTF32			,
		BOM_UTF16_BE		,
		BOM_UTF16			,
		BOM_UTF8			,		
		BOM_UTF_EBCDIC		,
		BOM_GB_18030		,		
		BOM_UTF1			,
		BOM_SCSU			,
		BOM_BOCU_1			,

		BOM_COUNT			,
		BOM_UNDEFINED		= 0xFFFFFFFF,
	};

	struct ST_BOM_INFO
	{
		size_t		tSize;
		BYTE		btSeq[5];
	};

	E_BOM_TYPE GetCurrentBOMA(void);
	E_BOM_TYPE GetCurrentBOMW(void);

	E_BOM_TYPE ReadBOM(FILE* pFile);
	E_BOM_TYPE ReadBOM(const unsigned char* pszContents, size_t tLength, ST_BOM_INFO& outInfo);
	ECODE WriteBOM(FILE* pFile, E_BOM_TYPE nType);

	const ST_BOM_INFO* GetBOMInfo(E_BOM_TYPE nType);
}
