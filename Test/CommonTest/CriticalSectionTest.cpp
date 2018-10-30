#include "stdafx.h"
#include <vector>

//////////////////////////////////////////////////////////////////////////
struct ST_CS_TEST_DATA
{
	CCriticalSection csResult;
	std::vector<int> vecResult;
};

//////////////////////////////////////////////////////////////////////////
static int CriticalSectionTestThreadFunc(void* pContext)
{
	ST_CS_TEST_DATA* pTestData = (ST_CS_TEST_DATA*)pContext;

	Sleep(100);
	{
		CCriticalSection::Owner Lock(pTestData->csResult);
		pTestData->vecResult.push_back(5);
		pTestData->vecResult.push_back(6);
		pTestData->vecResult.push_back(7);
		pTestData->vecResult.push_back(8);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, CriticalSectionTest)
{
	ST_CS_TEST_DATA	testData;

	HANDLE	hThread = CreateThread(CriticalSectionTestThreadFunc, &testData);
	EXPECT_TRUE(hThread != NULL);

	{
		CCriticalSection::Owner Lock(testData.csResult);

		testData.vecResult.push_back(1);		Sleep(100);
		testData.vecResult.push_back(2);		Sleep(100);
		testData.vecResult.push_back(3);		Sleep(100);
		testData.vecResult.push_back(4);		Sleep(100);
	}

	JoinThread(hThread);

	size_t i;
	for(i=1; i<testData.vecResult.size(); i++)
		EXPECT_EQ(1, testData.vecResult[i] - testData.vecResult[i-1]);
}

