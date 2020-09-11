#pragma once

namespace core
{
	struct ST_MEMORYLEAK_FINDER
	{
		ST_MEMORYLEAK_FINDER(int nSetBreakAllocCounter = 0);
		~ST_MEMORYLEAK_FINDER(void);
	};
}
