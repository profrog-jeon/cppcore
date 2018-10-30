#include "stdafx.h"
#include "../../Src/100_System/GlobalSemaphoreMap.h"

//////////////////////////////////////////////////////////////////////////
LPCSTR g_pszSemaphoreName[] = {	"a", "b", "c", "d", "e"	};
const size_t g_tSemaphoreNameCount = sizeof(g_pszSemaphoreName) / sizeof(g_pszSemaphoreName[0]);

//////////////////////////////////////////////////////////////////////////
int ThreadFunc(void* pContext)
{
	srand(GetTickCount());
	LPCSTR pszName = g_pszSemaphoreName[rand()%g_tSemaphoreNameCount];

	std::vector<HANDLE> vecHandles;

	int nCount = 100;
	while(nCount--)
	{
		Sleep(5);

		if( rand() & 0x01 )
			vecHandles.push_back(GlobalSemMap()->Create(pszName, GetCurrentThreadId()));
		else
		{
			if( vecHandles.empty() )
				continue;

			size_t tRandomIndex = rand() % vecHandles.size();
			GlobalSemMap()->Destroy(vecHandles[tRandomIndex]);
			vecHandles.erase(vecHandles.begin() + tRandomIndex);
		}
	}

	size_t i;
	for(i=0; i<vecHandles.size(); i++)
		GlobalSemMap()->Destroy(vecHandles[i]);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, GlobalSem_RemoveTest)
{
	std::vector<HANDLE> vecHandle;
	vecHandle.push_back(GlobalSemMap()->Create("A", 1));
	vecHandle.push_back(GlobalSemMap()->Create("B", 1));
	vecHandle.push_back(GlobalSemMap()->Create("C", 1));

	GlobalSemMap()->Destroy(vecHandle[1]);
	vecHandle.erase(vecHandle.begin() + 1);

	EXPECT_STREQ("A", GlobalSemMap()->m_pSemDataArr[0].szName);
	EXPECT_STREQ("C", GlobalSemMap()->m_pSemDataArr[1].szName);
	EXPECT_STREQ("", GlobalSemMap()->m_pSemDataArr[2].szName);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-1].nNextIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-1].nPrevIndex);
	EXPECT_EQ(1, GlobalSemMap()->m_pRefCountArr[-1].nValue);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-2].nNextIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-2].nPrevIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-2].nValue);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-3].nNextIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-3].nPrevIndex);
	EXPECT_EQ(1, GlobalSemMap()->m_pRefCountArr[-3].nValue);

	GlobalSemMap()->Remove(1);

	EXPECT_STREQ("", GlobalSemMap()->m_pSemDataArr[0].szName);
	EXPECT_STREQ("", GlobalSemMap()->m_pSemDataArr[1].szName);
	EXPECT_STREQ("", GlobalSemMap()->m_pSemDataArr[2].szName);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-1].nNextIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-1].nPrevIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-1].nValue);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-2].nNextIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-2].nPrevIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-2].nValue);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-3].nNextIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-3].nPrevIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-3].nValue);

	EXPECT_TRUE(GlobalSemMap()->IsEmpty());
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, GlobalSem_SemDataDeleteTest)
{
	std::vector<HANDLE> vecHandle;
	vecHandle.push_back(GlobalSemMap()->Create("A", 1));
	vecHandle.push_back(GlobalSemMap()->Create("B", 1));
	vecHandle.push_back(GlobalSemMap()->Create("C", 1));

	GlobalSemMap()->Destroy(vecHandle[1]);
	vecHandle.erase(vecHandle.begin() + 1);

	EXPECT_STREQ("A", GlobalSemMap()->m_pSemDataArr[0].szName);
	EXPECT_STREQ("C", GlobalSemMap()->m_pSemDataArr[1].szName);
	EXPECT_STREQ("", GlobalSemMap()->m_pSemDataArr[2].szName);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-1].nNextIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-1].nPrevIndex);
	EXPECT_EQ(1, GlobalSemMap()->m_pRefCountArr[-1].nValue);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-2].nNextIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-2].nPrevIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-2].nValue);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-3].nNextIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-3].nPrevIndex);
	EXPECT_EQ(1, GlobalSemMap()->m_pRefCountArr[-3].nValue);

	vecHandle.push_back(GlobalSemMap()->Create("C", 1));

	EXPECT_STREQ("A", GlobalSemMap()->m_pSemDataArr[0].szName);
	EXPECT_STREQ("C", GlobalSemMap()->m_pSemDataArr[1].szName);
	EXPECT_STREQ("", GlobalSemMap()->m_pSemDataArr[2].szName);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-1].nNextIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-1].nPrevIndex);
	EXPECT_EQ(1, GlobalSemMap()->m_pRefCountArr[-1].nValue);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-2].nNextIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-2].nPrevIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-2].nValue);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-3].nNextIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-3].nPrevIndex);
	EXPECT_EQ(2, GlobalSemMap()->m_pRefCountArr[-3].nValue);

	size_t i;
	for(i=0; i<vecHandle.size(); i++)
		GlobalSemMap()->Destroy(vecHandle[i]);

	EXPECT_TRUE(GlobalSemMap()->IsEmpty());
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, GlobalSem_SemDataDeleteAndNewTest)
{
	std::vector<HANDLE> vecHandle;
	vecHandle.push_back(GlobalSemMap()->Create("A", 1));
	vecHandle.push_back(GlobalSemMap()->Create("B", 1));
	vecHandle.push_back(GlobalSemMap()->Create("C", 1));

	GlobalSemMap()->Destroy(vecHandle[1]);
	vecHandle.erase(vecHandle.begin() + 1);

	EXPECT_STREQ("A", GlobalSemMap()->m_pSemDataArr[0].szName);
	EXPECT_STREQ("C", GlobalSemMap()->m_pSemDataArr[1].szName);
	EXPECT_STREQ("", GlobalSemMap()->m_pSemDataArr[2].szName);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-1].nNextIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-1].nPrevIndex);
	EXPECT_EQ(1, GlobalSemMap()->m_pRefCountArr[-1].nValue);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-2].nNextIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-2].nPrevIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-2].nValue);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-3].nNextIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-3].nPrevIndex);
	EXPECT_EQ(1, GlobalSemMap()->m_pRefCountArr[-3].nValue);

	vecHandle.push_back(GlobalSemMap()->Create("D", 1));

	EXPECT_STREQ("A", GlobalSemMap()->m_pSemDataArr[0].szName);
	EXPECT_STREQ("C", GlobalSemMap()->m_pSemDataArr[1].szName);
	EXPECT_STREQ("D", GlobalSemMap()->m_pSemDataArr[2].szName);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-1].nNextIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-1].nPrevIndex);
	EXPECT_EQ(1, GlobalSemMap()->m_pRefCountArr[-1].nValue);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-2].nNextIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-2].nPrevIndex);
	EXPECT_EQ(1, GlobalSemMap()->m_pRefCountArr[-2].nValue);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-3].nNextIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-3].nPrevIndex);
	EXPECT_EQ(1, GlobalSemMap()->m_pRefCountArr[-3].nValue);

	size_t i;
	for(i=0; i<vecHandle.size(); i++)
		GlobalSemMap()->Destroy(vecHandle[i]);

	EXPECT_TRUE(GlobalSemMap()->IsEmpty());
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, GlobalSem_SimpleInsertTest)
{
	LPCSTR pszName = "test";

	std::vector<HANDLE> vecHandle;

	int nCount = 5;
	while(nCount--)
		vecHandle.push_back(GlobalSemMap()->Create(pszName, GetCurrentProcessId()));

	EXPECT_STREQ(pszName, GlobalSemMap()->m_pSemDataArr[0].szName);
	EXPECT_STREQ("", GlobalSemMap()->m_pSemDataArr[1].szName);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-1].nNextIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-1].nPrevIndex);
	EXPECT_EQ(5, GlobalSemMap()->m_pRefCountArr[-1].nValue);

	size_t i;
	for(i=0; i<vecHandle.size(); i++)
		GlobalSemMap()->Destroy(vecHandle[i]);

	EXPECT_TRUE(GlobalSemMap()->IsEmpty());
}

//////////////////////////////////////////////////////////////////////////
#ifndef __APPLE__
TEST(SystemTest, GlobalSem_DualInsertTest)
{
	LPCSTR pszName = "test";

	std::vector<HANDLE> vecHandle;

	int nCount = 5;
	while(nCount--)
		vecHandle.push_back(GlobalSemMap()->Create(pszName, 1));

	nCount = 2;
	while(nCount--)
		vecHandle.push_back(GlobalSemMap()->Create(pszName, 2));

	EXPECT_STREQ(pszName, GlobalSemMap()->m_pSemDataArr[0].szName);
	EXPECT_STREQ("", GlobalSemMap()->m_pSemDataArr[1].szName);
	EXPECT_EQ(-2, GlobalSemMap()->m_pRefCountArr[-1].nNextIndex);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-1].nPrevIndex);
	EXPECT_EQ(5, GlobalSemMap()->m_pRefCountArr[-1].nValue);
	EXPECT_EQ(0, GlobalSemMap()->m_pRefCountArr[-2].nNextIndex);
	EXPECT_EQ(-1, GlobalSemMap()->m_pRefCountArr[-2].nPrevIndex);
	EXPECT_EQ(2, GlobalSemMap()->m_pRefCountArr[-2].nValue);

	size_t i;
	for(i=0; i<vecHandle.size(); i++)
		GlobalSemMap()->Destroy(vecHandle[i]);

	EXPECT_TRUE(GlobalSemMap()->IsEmpty());
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, GlobalSem_2ThreadTest)
{
	const size_t tThreadCount = 2;
	HANDLE hThread[tThreadCount];

	size_t i;
	for(i=0; i<tThreadCount; i++)
		hThread[i] = CreateThread(ThreadFunc, NULL);

	for(i=0; i<tThreadCount; i++)
		JoinThread(hThread[i]);

	EXPECT_TRUE(GlobalSemMap()->IsEmpty());
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, GlobalSemStressTest)
{
	const size_t tThreadCount = 5;
	HANDLE hThread[tThreadCount];

	size_t i;
	for(i=0; i<tThreadCount; i++)
		hThread[i] = CreateThread(ThreadFunc, NULL);

	for(i=0; i<tThreadCount; i++)
		JoinThread(hThread[i]);

	EXPECT_TRUE(GlobalSemMap()->IsEmpty());
}
#endif
