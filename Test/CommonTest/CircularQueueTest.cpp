#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, CircularQueueTest)
{
	const int nCount = 64;

	CCircularQueue<int> CircularQueue;
	CircularQueue.Init(nCount, 0xFFFFFFFF - 10);

	DWORD i;
	for(i=0; i<nCount; i++)
	{
		EXPECT_EQ(EC_SUCCESS, CircularQueue.Push(i));
	}

	for(i=0; i<nCount; i++)
	{
		int tRead = 0;
		EXPECT_EQ(EC_SUCCESS, CircularQueue.Pop(tRead));
		EXPECT_EQ(i, tRead);
	}

	for(i=0; i<nCount; i++)
	{
		EXPECT_EQ(EC_SUCCESS, CircularQueue.Push(i));
	}

	for(i=0; i<nCount; i++)
	{
		int tRead = 0;
		EXPECT_EQ(EC_SUCCESS, CircularQueue.Pop(tRead));
		EXPECT_EQ(i, tRead);
	}

	for(; i<nCount; i++)
	{
		int tRead = 0;
		EXPECT_EQ(EC_NO_DATA, CircularQueue.Pop(tRead));
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, CircularQueue_EmptyTest)
{
	CCircularQueue<int> CircularQueue;

	const size_t nCount = 10;

	DWORD i;
	for(i=0; i<nCount * 2; i++)
	{
		int tRead = 0;
		EXPECT_EQ(EC_NO_DATA, CircularQueue.Pop(tRead));
	}
}

//////////////////////////////////////////////////////////////////////////
class CircularQueueTest : public testing::Test
{
public:
	int m_nMaxTestCount;
	int m_nFlag;
	bool m_bCircularQueueTestContinue;

	CCircularQueue<int> m_Queue;
	BYTE* m_btFlags;

	void SetUp()
	{
		m_nFlag = -1;
		m_nMaxTestCount = 1000;
		m_bCircularQueueTestContinue = true;
		m_btFlags = new BYTE[m_nMaxTestCount];
		memset(m_btFlags, 0, m_nMaxTestCount);
		m_Queue.Init(m_nMaxTestCount, 0xFFFFFFF0);
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
			hProduceThread[i] = CreateThread<CircularQueueTest>(this, &CircularQueueTest::PushThread, NULL);		

		for(i=0; i<tConsumerCount; i++)
			hConsumeThread[i] = CreateThread<CircularQueueTest>(this, &CircularQueueTest::PopThread, NULL);

		for(i=0; i<tProducerCount; i++)
			JoinThread(hProduceThread[i]);

		core::Sleep(tWaitTimeOut);

		m_bCircularQueueTestContinue = false;
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
		while(m_bCircularQueueTestContinue )
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
		while(m_bCircularQueueTestContinue)
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
TEST_F(CircularQueueTest, CircularQueue_SingleThreadTest)
{
	DWORD i;
	for(i=0; i<(size_t)m_nMaxTestCount; i++)
		m_Queue.Push((int)i);

	EXPECT_EQ((DWORD)m_nMaxTestCount, m_Queue.Count());

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
TEST_F(CircularQueueTest, CircularQueue_PushTest)
{
	const size_t tMaxThread = 20;
	HANDLE hPushThread[tMaxThread];

	DWORD i;
	for(i=0; i<tMaxThread; i++)
	{
		hPushThread[i] = CreateThread<CircularQueueTest>(this, &CircularQueueTest::PushThread, NULL);
	}

	for(i=0; i<tMaxThread; i++)
		JoinThread(hPushThread[i]);

	EXPECT_EQ((DWORD)m_nMaxTestCount, m_Queue.Count());

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
static const size_t g_tMaxThreadCount = 4;

//////////////////////////////////////////////////////////////////////////
TEST_F(CircularQueueTest, CircularQueue_PopTest)
{
	const size_t tMaxThread = 20;

	DWORD i;
	for(i=0; i<(size_t)m_nMaxTestCount; i++)
		m_Queue.Push((int)i);

	EXPECT_EQ((DWORD)m_nMaxTestCount, m_Queue.Count());

	Test(0, g_tMaxThreadCount, 1000);
}

//////////////////////////////////////////////////////////////////////////
TEST_F(CircularQueueTest, CircularQueue_MPSCTest)
{
	Test(g_tMaxThreadCount, 1, 1000);
}

//////////////////////////////////////////////////////////////////////////
TEST_F(CircularQueueTest, CircularQueue_SPMCTest)
{
	Test(1, g_tMaxThreadCount, 1000); 
}

//////////////////////////////////////////////////////////////////////////
TEST_F(CircularQueueTest, CircularQueue_MPMCTest)
{
	Test(g_tMaxThreadCount, g_tMaxThreadCount, 1000); 
}
