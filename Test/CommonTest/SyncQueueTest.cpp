#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
int __internal_SyncQueueTest_HelperThread(void* pContext)
{
	TSyncQueue<int>* pSyncQueue = (TSyncQueue<int>*)pContext;

	printf("try to wait\n");
	int nItem = 0;
	pSyncQueue->Pop(&nItem);
	printf("wait finished, %d\n", nItem);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, SyncQueueTest_Pop)
{
	TSyncQueue<int> TestQueue;
	ASSERT_EQ(EC_SUCCESS, TestQueue.Create());
	HANDLE hThread = CreateThread(__internal_SyncQueueTest_HelperThread, &TestQueue);
	ASSERT_NE((HANDLE)NULL, hThread);
	Sleep(10);	
	TestQueue.Push(3);
	printf("data pushed\n");

	DWORD dwStartTick = GetTickCount();
	JoinThread(hThread);
	DWORD dwElapsedTick = GetTickCount() - dwStartTick;
	EXPECT_LT(dwElapsedTick, (DWORD)1000);
	TestQueue.Destroy();
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, SyncQueueTest_Destroy)
{
	TSyncQueue<int> TestQueue;
	ASSERT_EQ((DWORD)EC_SUCCESS, TestQueue.Create());
	HANDLE hThread = CreateThread(__internal_SyncQueueTest_HelperThread, &TestQueue);
	ASSERT_NE((HANDLE)NULL, hThread);
	Sleep(500);
	TestQueue.Destroy();

	DWORD dwStartTick = GetTickCount();
	JoinThread(hThread);
	DWORD dwElapsedTick = GetTickCount() - dwStartTick;
	EXPECT_LT(dwElapsedTick, (DWORD)1000);
}
