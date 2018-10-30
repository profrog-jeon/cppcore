#include "stdafx.h"
#include <vector>

//////////////////////////////////////////////////////////////////////////
struct ST_MUTEX_TEST_DATA
{
	LPCTSTR pszMutexKey;
	std::vector<int> vecResult;
};

//////////////////////////////////////////////////////////////////////////
static int MutexTestThreadFunc(void* pContext)
{
	ST_MUTEX_TEST_DATA* pTestData = (ST_MUTEX_TEST_DATA*)pContext;
	CMutex mutex(pTestData->pszMutexKey);

	Sleep(100);
	{
		CMutex::Owner Lock(mutex);
		pTestData->vecResult.push_back(5);
		pTestData->vecResult.push_back(6);
		pTestData->vecResult.push_back(7);
		pTestData->vecResult.push_back(8);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, MutexTest)
{
	ST_MUTEX_TEST_DATA	testData;
	testData.pszMutexKey = TEXT("1944");

	CMutex mutex(testData.pszMutexKey);

	HANDLE	hThread = CreateThread(MutexTestThreadFunc, &testData);
	EXPECT_TRUE(hThread != NULL);

	{
		CMutex::Owner Lock(mutex);

		testData.vecResult.push_back(1);		Sleep(100);
		testData.vecResult.push_back(2);		Sleep(100);
		testData.vecResult.push_back(3);		Sleep(100);
		testData.vecResult.push_back(4);		Sleep(100);
	}

	JoinThread(hThread);

	EXPECT_GT(testData.vecResult.size(), 0U);

	size_t i;
	for(i=1; i<testData.vecResult.size(); i++)
		EXPECT_EQ(1, testData.vecResult[i] - testData.vecResult[i-1]);
}

