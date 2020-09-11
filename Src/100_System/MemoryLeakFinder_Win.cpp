#include "stdafx.h"
#include "MemoryLeakFinder.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

namespace core
{
	ST_MEMORYLEAK_FINDER::ST_MEMORYLEAK_FINDER(int nSetBreakAllocCounter)
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		if (nSetBreakAllocCounter > 0)
			_CrtSetBreakAlloc(nSetBreakAllocCounter);
	}
	ST_MEMORYLEAK_FINDER::~ST_MEMORYLEAK_FINDER(void)
	{
	}
}
