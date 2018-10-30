#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, LockFreeQueueTest)
{
	const int nCount = 64;

	CLockFreeQueue<int> LockFreeQueue;

	DWORD i;
	for(i=0; i<nCount; i++)
	{
		EXPECT_EQ(EC_SUCCESS, LockFreeQueue.Push(i));
	}

	for(i=0; i<nCount; i++)
	{
		int tRead = 0;
		EXPECT_EQ(EC_SUCCESS, LockFreeQueue.Pop(tRead));
		EXPECT_EQ(i, tRead);
	}

	for(i=0; i<nCount; i++)
	{
		EXPECT_EQ(EC_SUCCESS, LockFreeQueue.Push(i));
	}

	for(i=0; i<nCount; i++)
	{
		int tRead = 0;
		EXPECT_EQ(EC_SUCCESS, LockFreeQueue.Pop(tRead));
		EXPECT_EQ(i, tRead);
	}

	for(; i<nCount; i++)
	{
		int tRead = 0;
		EXPECT_EQ(EC_NO_DATA, LockFreeQueue.Pop(tRead));
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, LockFreeQueue_EmptyTest)
{
	CLockFreeQueue<int> LockFreeQueue;

	const size_t nCount = 10;

	DWORD i;
	for(i=0; i<nCount * 2; i++)
	{
		int tRead = 0;
		EXPECT_EQ(EC_NO_DATA, LockFreeQueue.Pop(tRead));
	}
}

//////////////////////////////////////////////////////////////////////////
class LockFreeQueueTest : public testing::Test
{
public:
	int m_nMaxTestCount;
	int m_nFlag;
	bool m_bLockFreeQueueTestContinue;

	CLockFreeQueue<int> m_Queue;
	BYTE* m_btFlags;

	void SetUp()
	{
		m_nFlag = -1;
		m_nMaxTestCount = 1000;
		m_bLockFreeQueueTestContinue = true;
		m_btFlags = new BYTE[m_nMaxTestCount];
		memset(m_btFlags, 0, m_nMaxTestCount);
	}

	void TearDown(void)
	{
		delete [] m_btFlags;
	}

	void Test(size_t tProducerCount, size_t tConsumerCount, size_t tWaitTimeOut)
	{
		std::vector<HANDLE> hProduceThread;
		std::vector<HANDLE> hConsumeThread;
		hProduceThread.resize(tProducerCount);
		hConsumeThread.resize(tConsumerCount);

		DWORD i;
		for(i=0; i<tProducerCount; i++)
			hProduceThread[i] = CreateThread<LockFreeQueueTest>(this, &LockFreeQueueTest::PushThread, NULL);		

		for(i=0; i<tConsumerCount; i++)
			hConsumeThread[i] = CreateThread<LockFreeQueueTest>(this, &LockFreeQueueTest::PopThread, NULL);

		for(i=0; i<tProducerCount; i++)
			JoinThread(hProduceThread[i]);

		core::Sleep(tWaitTimeOut);

		m_bLockFreeQueueTestContinue = false;
		for(i=0; i<tConsumerCount; i++)
			JoinThread(hConsumeThread[i]);

		size_t tFalseCount = 0;
		for(i=0; i<(size_t)m_nMaxTestCount; i++)
		{
			if( m_btFlags[i] )
				continue;

			tFalseCount++;
			printf("%u\n", i);
		}

		EXPECT_EQ(0, tFalseCount);
	}

	int PushThread(void* pContext)
	{
		while(m_bLockFreeQueueTestContinue )
		{
			int nValue = InterlockedIncrement_(&m_nFlag);
			if( nValue >= m_nMaxTestCount )
				break;

			m_Queue.Push(nValue);
		}

		return 0;
	}

	int PopThread(void* pContext)
	{
		while(m_bLockFreeQueueTestContinue)
		{
			int nItem = 0;
			if( EC_SUCCESS != m_Queue.Pop(nItem) )
				continue;

			if( nItem < m_nMaxTestCount )
				m_btFlags[nItem] = 1;
		}
		return 0;
	}
};

//////////////////////////////////////////////////////////////////////////
TEST_F(LockFreeQueueTest, LockFreeQueue_SingleThreadTest)
{
	DWORD i;
	for(i=0; i<(size_t)m_nMaxTestCount; i++)
		m_Queue.Push((int)i);

	int nValue = 0;
	while(EC_SUCCESS == m_Queue.Pop(nValue))
		m_btFlags[nValue] = true;

	size_t tFalseCount = 0;
	for(i=0; i<(size_t)m_nMaxTestCount; i++)
	{
		if( m_btFlags[i] )
			continue;

		tFalseCount++;
	}

	EXPECT_EQ(0, tFalseCount);
}

//////////////////////////////////////////////////////////////////////////
TEST_F(LockFreeQueueTest, LockFreeQueue_PushTest)
{
	const size_t tMaxThread = 20;
	HANDLE hPushThread[tMaxThread];

	DWORD i;
	for(i=0; i<tMaxThread; i++)
	{
		hPushThread[i] = CreateThread<LockFreeQueueTest>(this, &LockFreeQueueTest::PushThread, NULL);
	}

	for(i=0; i<tMaxThread; i++)
		JoinThread(hPushThread[i]);

	int nValue = 0;
	while(EC_SUCCESS == m_Queue.Pop(nValue))
		m_btFlags[nValue] = true;

	size_t tFalseCount = 0;
	for(i=0; i<(size_t)m_nMaxTestCount; i++)
	{
		if( m_btFlags[i] )
			continue;

		tFalseCount++;
	}

	EXPECT_EQ(0, tFalseCount);
}

//////////////////////////////////////////////////////////////////////////
TEST_F(LockFreeQueueTest, LockFreeQueue_PopTest)
{
	const size_t tMaxThread = 20;
	HANDLE hPopThread[tMaxThread];

	DWORD i;
	for(i=0; i<(size_t)m_nMaxTestCount; i++)
		m_Queue.Push((int)i);

	for(i=0; i<tMaxThread; i++)
	{
		hPopThread[i] = CreateThread<LockFreeQueueTest>(this,  &LockFreeQueueTest::PopThread, NULL);
	}

	core::Sleep(1000);
	m_bLockFreeQueueTestContinue = false;
	for(i=0; i<tMaxThread; i++)
		JoinThread(hPopThread[i]);

	size_t tFalseCount = 0;
	for(i=0; i<(size_t)m_nMaxTestCount; i++)
	{
		if( m_btFlags[i] )
			continue;

		printf("%u\n", i);
		tFalseCount++;
	}

	EXPECT_EQ(0, tFalseCount);
}

//////////////////////////////////////////////////////////////////////////
TEST_F(LockFreeQueueTest, DISABLED_LockFreeQueue_MPSCTest)
{
	Test(20, 1, 1000);
}

//////////////////////////////////////////////////////////////////////////
TEST_F(LockFreeQueueTest, DISABLED_LockFreeQueue_SPMCTest)
{
	Test(1, 20, 1000); 
}

//////////////////////////////////////////////////////////////////////////
TEST_F(LockFreeQueueTest, DISABLED_LockFreeQueue_MPMCTest)
{
	Test(10, 10, 1000); 
}
