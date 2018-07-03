#include "stdafx.h"
#include "Thread.h"

namespace core
{
	int InternalThreadCaller(void* pContext)
	{
		INTERNAL_COMMON_THREAD_DATA* pThreadData = (INTERNAL_COMMON_THREAD_DATA*)pContext;
		int nErrCode = pThreadData->ThreadFunc();
		delete pThreadData;
		return nErrCode;
	}
}
