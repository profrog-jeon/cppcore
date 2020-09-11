#include "stdafx.h"
#include "BOM.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	E_BOM_TYPE GetCurrentBOM(void)
	{
#ifdef _MSC_VER
#ifdef UNICODE
		return BOM_UTF16;
#else
		return BOM_EUCKR;
#endif
#else
#ifdef UNICODE
		return BOM_UTF32;
#else
		return BOM_UTF8;
#endif
#endif
	}
}