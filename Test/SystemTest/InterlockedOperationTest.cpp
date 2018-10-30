#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
using namespace core;

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, InterlockedAnd_Test)
{
	const int nOriginal = 0xFFFF0000;
	int nDest = nOriginal;
	int nNew = 0x00FFFFFF;
	EXPECT_EQ(nOriginal, InterlockedAnd_(&nDest, nNew));
	EXPECT_EQ(nDest, 0x00FF0000);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, InterlockedOr_Test)
{
	const int nOriginal = 0xFFFF0000;
	int nDest = nOriginal;
	int nNew = 0x00FFFFFF;
	EXPECT_EQ(nOriginal, InterlockedOr_(&nDest, nNew));
	EXPECT_EQ(nDest, 0xFFFFFFFF);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, InterlockedXor_Test)
{
	const int nOriginal = 0xFFFF0000;
	int nDest = nOriginal;
	int nNew = 0x00FFFFFF;
	EXPECT_EQ(nOriginal, InterlockedXor_(&nDest, nNew));
	EXPECT_EQ(nDest, 0xFF00FFFF);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, InterlockedCompareExchange_Test)
{
	int nDest = 0;
	int nNew = 1;
	EXPECT_EQ(0, InterlockedCompareExchange_(&nDest, nNew, 0));
	EXPECT_EQ(1, InterlockedCompareExchange_(&nDest, nNew, 0));
	EXPECT_EQ(1, InterlockedCompareExchange_(&nDest, nNew, 0));
	EXPECT_EQ(nDest, nNew);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, InterlockedCompareExchangePointer_Test)
{
	void* pDest = NULL;
	void* pNew = (void*)0x01;
	EXPECT_EQ(NULL, InterlockedCompareExchangePointer_(&pDest, pNew, NULL));
	EXPECT_EQ(pDest, pNew);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, InterlockedExchangePointer_Test)
{
	void* pDest = NULL;
	void* pNew = (void*)0x01;
	EXPECT_EQ(NULL, InterlockedExchangePointer_(&pDest, pNew));
	EXPECT_EQ(pDest, pNew);
}

//////////////////////////////////////////////////////////////////////////
struct ST_INTERLOCKED_TEST_DATA
{
	int* pDestValue;
	BYTE* pnSlotArr;
	size_t tLoopCount;
};

//////////////////////////////////////////////////////////////////////////
int __internal_InterlockedIncrementTest(void* pContext)
{
	ST_INTERLOCKED_TEST_DATA* pInfo = (ST_INTERLOCKED_TEST_DATA*)pContext;

	size_t i;
	for(i=0; i<pInfo->tLoopCount; i++)
	{
		int nIndex = InterlockedIncrement_(pInfo->pDestValue);
		pInfo->pnSlotArr[nIndex] = 1;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, InterlockedIncrement_Test)
{
	const static size_t tThreadCount = 100;
	const static size_t tSlotCountPerThread = 1000;
	const static size_t tSlotCount = tThreadCount * tSlotCountPerThread;

	ST_INTERLOCKED_TEST_DATA stInfoArr[tThreadCount];
	HANDLE hThreadArr[tThreadCount] = { NULL, };

	BYTE nSlotArr[tSlotCount] = { 0, };
	int nIndex = -1;

	size_t i;
	for(i=0; i<tThreadCount; i++)
	{
		stInfoArr[i].pDestValue	= &nIndex;
		stInfoArr[i].pnSlotArr	= nSlotArr;
		stInfoArr[i].tLoopCount	= tSlotCountPerThread;
		hThreadArr[i] = CreateThread(__internal_InterlockedIncrementTest, &stInfoArr[i]);
	}

	for(i=0; i<tThreadCount; i++)
		JoinThread(hThreadArr[i]);

	for(i=0; i<tSlotCount; i++)
	{
		std::tstring strTrace = Format(TEXT("%d is ZERO"), i);
		SCOPED_TRACE(strTrace.c_str());
		EXPECT_TRUE(nSlotArr[i] != 0);
	}
}

//////////////////////////////////////////////////////////////////////////
int __internal_InterlockedDecrementTest(void* pContext)
{
	ST_INTERLOCKED_TEST_DATA* pInfo = (ST_INTERLOCKED_TEST_DATA*)pContext;

	size_t i;
	for(i=0; i<pInfo->tLoopCount; i++)
	{
		int nIndex = InterlockedDecrement_(pInfo->pDestValue);
		pInfo->pnSlotArr[nIndex] = 1;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, InterlockedDecrement_Test)
{
	const static size_t tThreadCount = 100;
	const static size_t tSlotCountPerThread = 1000;
	const static size_t tSlotCount = tThreadCount * tSlotCountPerThread;

	ST_INTERLOCKED_TEST_DATA stInfoArr[tThreadCount];
	HANDLE hThreadArr[tThreadCount] = { NULL, };

	BYTE nSlotArr[tSlotCount] = { 0, };
	int nIndex = (int)tSlotCount;

	size_t i;
	for(i=0; i<tThreadCount; i++)
	{
		stInfoArr[i].pDestValue	= &nIndex;
		stInfoArr[i].pnSlotArr	= nSlotArr;
		stInfoArr[i].tLoopCount	= tSlotCountPerThread;
		hThreadArr[i] = CreateThread(__internal_InterlockedDecrementTest, &stInfoArr[i]);
	}

	for(i=0; i<tThreadCount; i++)
		JoinThread(hThreadArr[i]);

	for(i=0; i<tSlotCount; i++)
	{
		EXPECT_TRUE(nSlotArr[i] != 0);
	}
}

//////////////////////////////////////////////////////////////////////////
int g_nInterlockedExchangedCount = 0;
int g_nInterlockedExchangedAckCount = 0;
const int g_nLoopCount = 100000;

//////////////////////////////////////////////////////////////////////////
int __internal_InterlockedExchangeTest_WorkerThread(void* pContext)
{
	int* pnTrigger = (int*)pContext;

	int i;
	for(i=0; i<g_nLoopCount; i++)
	{
		if( 0 == InterlockedExchange_(pnTrigger, 0) )
			continue;

		InterlockedIncrement_(&g_nInterlockedExchangedAckCount);
	}
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////
int __internal_InterlockedExchangeTest_TriggerThread(void* pContext)
{
	int* pnTrigger = (int*)pContext;

	int i;
	for(i=0; i<g_nLoopCount; i++)
	{
		// trigger once per 100
		if( rand() % 100 )
			continue;

		if( 0 == InterlockedExchange_(pnTrigger, 1) )
			InterlockedIncrement_(&g_nInterlockedExchangedCount);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, InterlockedExchange_Test)
{
	const static size_t tThreadCount = 100;

	g_nInterlockedExchangedCount = 0;
	g_nInterlockedExchangedAckCount = 0;

	HANDLE hThreadArr[tThreadCount] = { NULL, };

	int nTrigger = 0;

	size_t i;
	for(i=0; i<tThreadCount; i++)
	{
		hThreadArr[i] = CreateThread(__internal_InterlockedExchangeTest_WorkerThread, &nTrigger);
	}

	HANDLE hTriggerThread = CreateThread(__internal_InterlockedExchangeTest_TriggerThread, &nTrigger);
	JoinThread(hTriggerThread);

	for(i=0; i<tThreadCount; i++)
		JoinThread(hThreadArr[i]);

	// the last exchange can be remained
	if( 1 == InterlockedExchange_(&nTrigger, 0) )
		InterlockedIncrement_(&g_nInterlockedExchangedAckCount);

	EXPECT_GT(g_nInterlockedExchangedCount, 0);
	EXPECT_EQ(g_nInterlockedExchangedCount, g_nInterlockedExchangedAckCount);
}

//////////////////////////////////////////////////////////////////////////
struct ST_INTERLOCKED_ADD_TEST_DATA
{
	int* pDestValue;
	size_t tLoopCount;
};

//////////////////////////////////////////////////////////////////////////
int __internal_InterlockedAddTest(void* pContext)
{
	ST_INTERLOCKED_ADD_TEST_DATA* pInfo = (ST_INTERLOCKED_ADD_TEST_DATA*)pContext;

	size_t i;
	for(i=0; i<pInfo->tLoopCount; i++)
	{
		InterlockedExchangeAdd_(pInfo->pDestValue, 1);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, InterlockedAdd_Test)
{
	const static size_t tThreadCount = 100;

	ST_INTERLOCKED_ADD_TEST_DATA stInfoArr[tThreadCount];
	HANDLE hThreadArr[tThreadCount] = { NULL, };

	const size_t tLoopCount = 100000;
	int nDestValue = 0;

	size_t i;
	for(i=0; i<tThreadCount; i++)
	{
		stInfoArr[i].pDestValue	= &nDestValue;
		stInfoArr[i].tLoopCount = tLoopCount;
		hThreadArr[i] = CreateThread(__internal_InterlockedAddTest, &stInfoArr[i]);
	}

	for(i=0; i<tThreadCount; i++)
		JoinThread(hThreadArr[i]);

	EXPECT_EQ(nDestValue, (int)tLoopCount * tThreadCount);
}
