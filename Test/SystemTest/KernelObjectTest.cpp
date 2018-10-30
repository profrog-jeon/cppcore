#include "stdafx.h"
#include <vector>

//////////////////////////////////////////////////////////////////////////
static int Sleep_ThreadWorker(void* pContext)
{
	Sleep(1000 * 1000);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, WaitForThreadTest_TimeOut)
{
	HANDLE hThread = CreateThread(Sleep_ThreadWorker, NULL);
	ASSERT_NE((HANDLE)NULL, hThread);
	DWORD dwStartTick = GetTickCount();
	EXPECT_EQ(EC_TIMEOUT, WaitForThread(hThread, 500, NULL));
	DWORD dwElapsedTick = GetTickCount() - dwStartTick;
	EXPECT_GE(dwElapsedTick, 500U);
	EXPECT_LT(dwElapsedTick, 1000U);
}

//////////////////////////////////////////////////////////////////////////
static int Ret123_ThreadWorker(void* pContext)
{
	Sleep(1);
	return 123;
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, WaitForThreadTest_ThreadRet)
{
	HANDLE hThread = CreateThread(Ret123_ThreadWorker, NULL);
	ASSERT_NE((HANDLE)NULL, hThread);

	int nRet = 0;
	EXPECT_EQ(EC_SUCCESS, WaitForThread(hThread, 500, &nRet));
	EXPECT_EQ(nRet, 123);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, SetEvent_DelayedWaitTest)
{
	HANDLE hEvent = CreateEvent(false, false);
	ASSERT_NE((HANDLE)NULL, hEvent);

	SetEvent(hEvent);

	EXPECT_EQ(EC_SUCCESS, WaitForEvent(hEvent, 0));
	EXPECT_EQ(EC_TIMEOUT, WaitForEvent(hEvent, 0));
	CloseEvent(hEvent);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, SetEvent_DelayedWaitTest2)
{
	HANDLE hEvent = CreateEvent(false, false);
	ASSERT_NE((HANDLE)NULL, hEvent);

	SetEvent(hEvent);
	SetEvent(hEvent);

	EXPECT_EQ(EC_SUCCESS, WaitForEvent(hEvent, 0));
	EXPECT_EQ(EC_TIMEOUT, WaitForEvent(hEvent, 0));
	CloseEvent(hEvent);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, PulseEvent_DelayedWaitTest)
{
	HANDLE hEvent = CreateEvent(false, false);
	ASSERT_NE((HANDLE)NULL, hEvent);

	PulseEvent(hEvent);

	EXPECT_EQ(EC_TIMEOUT, WaitForEvent(hEvent, 0));
	EXPECT_EQ(EC_TIMEOUT, WaitForEvent(hEvent, 0));
	CloseEvent(hEvent);
}

//////////////////////////////////////////////////////////////////////////
static int WaitEventThread(void* pContext)
{
	HANDLE hEvent = (HANDLE)pContext;
	EWAIT nRet = WaitForEvent(hEvent, 1000);
	if( WAIT_OBJECT_0_ == nRet )
		return 0;
	return -1;
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, PulseEvent_DelayedWaitTest2)
{
	HANDLE hEvent = CreateEvent(false, false);
	ASSERT_NE((HANDLE)NULL, hEvent);

	HANDLE hThread = CreateThread(WaitEventThread, hEvent);
	if( hThread )
	{
		Sleep(50);
		PulseEvent(hEvent);

		int nRet = 0;
		WaitForThread(hThread, INFINITE, &nRet);
		CloseThreadHandle(hThread);
		EXPECT_EQ(0, nRet);
	}
	CloseEvent(hEvent);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateEventTest_auto_nosignaled)
{
	HANDLE hEvent = CreateEvent(false, false);
	ASSERT_NE((HANDLE)NULL, hEvent);

	EXPECT_EQ(EC_TIMEOUT, WaitForEvent(hEvent, 0));
	EXPECT_EQ(EC_TIMEOUT, WaitForEvent(hEvent, 0));
	CloseEvent(hEvent);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateEventTest_auto_signaled)
{
	HANDLE hEvent = CreateEvent(false, true);
	ASSERT_NE((HANDLE)NULL, hEvent);

	EXPECT_EQ(EC_SUCCESS, WaitForEvent(hEvent, 0));
	EXPECT_EQ(EC_TIMEOUT, WaitForEvent(hEvent, 0));
	CloseEvent(hEvent);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, ResetEventTest_auto_nosignaled_ret)
{
	HANDLE hEvent = CreateEvent(false, false);
	ASSERT_NE((HANDLE)NULL, hEvent);
	EXPECT_TRUE(ResetEvent(hEvent));
	EXPECT_TRUE(ResetEvent(hEvent));
	EXPECT_FALSE(ResetEvent(0));
	CloseEvent(hEvent);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, ResetEventTest_auto_signaled_ret)
{
	HANDLE hEvent = CreateEvent(false, true);
	ASSERT_NE((HANDLE)NULL, hEvent);
	EXPECT_TRUE(ResetEvent(hEvent));
	EXPECT_TRUE(ResetEvent(hEvent));
	EXPECT_FALSE(ResetEvent(0));
	CloseEvent(hEvent);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateEventTest_auto_multisignal)
{
	HANDLE hEvent = CreateEvent(false, false);
	ASSERT_NE((HANDLE)NULL, hEvent);

	EXPECT_TRUE(SetEvent(hEvent));
	EXPECT_TRUE(SetEvent(hEvent));
	EXPECT_TRUE(SetEvent(hEvent));
	EXPECT_FALSE(SetEvent(0));
	EXPECT_EQ(EC_SUCCESS, WaitForEvent(hEvent, 0));
	EXPECT_EQ(EC_TIMEOUT, WaitForEvent(hEvent, 0));
	EXPECT_EQ(EC_TIMEOUT, WaitForEvent(hEvent, 0));
	CloseEvent(hEvent);
}


//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateEventTest_manual_nosignaled)
{
	HANDLE hEvent = CreateEvent(true, false);
	ASSERT_NE((HANDLE)NULL, hEvent);

	EXPECT_EQ(EC_TIMEOUT, WaitForEvent(hEvent, 0));
	EXPECT_EQ(EC_TIMEOUT, WaitForEvent(hEvent, 0));
	SetEvent(hEvent);
	EXPECT_EQ(EC_SUCCESS, WaitForEvent(hEvent, 0));
	EXPECT_EQ(EC_SUCCESS, WaitForEvent(hEvent, 0));
	ResetEvent(hEvent);
	EXPECT_EQ(EC_TIMEOUT, WaitForEvent(hEvent, 0));
	EXPECT_EQ(EC_TIMEOUT, WaitForEvent(hEvent, 0));
	CloseEvent(hEvent);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateEventTest_manual_signaled)
{
	HANDLE hEvent = CreateEvent(true, true);
	ASSERT_NE((HANDLE)NULL, hEvent);

	EXPECT_EQ(EC_SUCCESS, WaitForEvent(hEvent, 0));
	EXPECT_EQ(EC_SUCCESS, WaitForEvent(hEvent, 0));
	ResetEvent(hEvent);
	EXPECT_EQ(EC_TIMEOUT, WaitForEvent(hEvent, 0));
	EXPECT_EQ(EC_TIMEOUT, WaitForEvent(hEvent, 0));
	CloseEvent(hEvent);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateEventTest_SingleSetEventTest)
{
	HANDLE hEvent = CreateEvent(false, false);
	ASSERT_NE((HANDLE)NULL, hEvent);

	EXPECT_EQ(EC_TIMEOUT, WaitForEvent(hEvent, 0));
	SetEvent(hEvent);
	EXPECT_EQ(EC_SUCCESS, WaitForEvent(hEvent, 0));
	EXPECT_EQ(EC_TIMEOUT, WaitForEvent(hEvent, 0));
	CloseEvent(hEvent);
}

//////////////////////////////////////////////////////////////////////////
static int WaitForEventThread(void* pContext)
{
	HANDLE hEvent = (HANDLE)pContext;
	return WaitForEvent(hEvent, 300);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateEventTest_SinglePulseEventTest)
{
	HANDLE hEvent = CreateEvent(true, false);
	ASSERT_NE((HANDLE)NULL, hEvent);

	EXPECT_EQ(EC_TIMEOUT, WaitForEvent(hEvent, 0));
	PulseEvent(hEvent);

	// If the call to PulseEvent occurs during the time when the thread has been removed from the wait state, 
	// the thread will not be released because 
	// PulseEvent releases only those threads that are waiting at the moment it is called
	EXPECT_EQ(EC_TIMEOUT, WaitForEvent(hEvent, 0));
	CloseEvent(hEvent);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateEventTest_MultiplePulseEventTest)
{
	HANDLE hEvent = CreateEvent(true, false);
	ASSERT_NE((HANDLE)NULL, hEvent);

	const int nThreadCount = 10;
	HANDLE hThread[nThreadCount] = { NULL, };
	int i;
	for(i=0; i<nThreadCount; i++)
		hThread[i] = CreateThread(WaitForEventThread, hEvent);

	Sleep(100);
	PulseEvent(hEvent);
	for(i=0; i<nThreadCount; i++)
	{
		int nExitCode = 0;
		JoinThread(hThread[i], &nExitCode);
		EXPECT_EQ(0, nExitCode);
	}

	for(i=0; i<nThreadCount; i++)
		hThread[i] = CreateThread(WaitForEventThread, hEvent);

	Sleep(100);
	for(i=0; i<nThreadCount; i++)
	{
		int nExitCode = 0;
		JoinThread(hThread[i], &nExitCode);
		EXPECT_NE(0, nExitCode);
	}

	CloseEvent(hEvent);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateEventTest_SinglePulseEvent_auto)
{
	HANDLE hEvent = CreateEvent(false, false);
	ASSERT_NE((HANDLE)NULL, hEvent);

	EXPECT_EQ(EC_TIMEOUT, WaitForEvent(hEvent, 0));
	PulseEvent(hEvent);

	// If the call to PulseEvent occurs during the time when the thread has been removed from the wait state, 
	// the thread will not be released because 
	// PulseEvent releases only those threads that are waiting at the moment it is called
	EXPECT_EQ(EC_TIMEOUT, WaitForEvent(hEvent, 0));
	CloseEvent(hEvent);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateEventTest_MultiplePulseEvent_auto)
{
	HANDLE hEvent = CreateEvent(false, false);
	ASSERT_NE((HANDLE)NULL, hEvent);

	const int nThreadCount = 10;
	HANDLE hThread[nThreadCount] = { NULL, };
	int i;
	for(i=0; i<nThreadCount; i++)
		hThread[i] = CreateThread(WaitForEventThread, hEvent);

	Sleep(100);
	PulseEvent(hEvent);

	// only 1 thread awaken
	int nZeroRetCount = 0;
	int nNonZeroRetCount = 0;
	for(i=0; i<nThreadCount; i++)
	{
		int nExitCode = 0;
		JoinThread(hThread[i], &nExitCode);
		if( nExitCode )
			nNonZeroRetCount ++;
		else
			nZeroRetCount ++;
	}
	EXPECT_EQ(1, nZeroRetCount);
	EXPECT_EQ(nThreadCount - 1, nNonZeroRetCount);

	for(i=0; i<nThreadCount; i++)
		hThread[i] = CreateThread(WaitForEventThread, hEvent);

	Sleep(100);
	for(i=0; i<nThreadCount; i++)
	{
		int nExitCode = 0;
		JoinThread(hThread[i], &nExitCode);
		EXPECT_NE(0, nExitCode);
	}

	CloseEvent(hEvent);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateEventTest_MultipleSetEventTest)
{
	HANDLE hEvent = CreateEvent(false, false);
	ASSERT_NE((HANDLE)NULL, hEvent);

	const int nThreadCount = 10;
	HANDLE hThread[nThreadCount] = { NULL, };
	int i;
	for(i=0; i<nThreadCount; i++)
		hThread[i] = CreateThread(WaitForEventThread, hEvent);

	Sleep(100);
	SetEvent(hEvent);

	int nZeroRetCount = 0;
	int nNonZeroRetCount = 0;
	for(i=0; i<nThreadCount; i++)
	{
		int nExitCode = 0;
		JoinThread(hThread[i], &nExitCode);
		if( nExitCode )
			nNonZeroRetCount ++;
		else
			nZeroRetCount ++;
	}

	EXPECT_EQ(1, nZeroRetCount);
	EXPECT_EQ(nThreadCount - 1, nNonZeroRetCount);
	CloseEvent(hEvent);
}

//////////////////////////////////////////////////////////////////////////
TEST(FileMappingTest, CreateFileMapping)
{
    std::tstring strFileName = TEXT("TempMappingFile.txt");

    std::tstring strContents = TEXT("Hello world!");
    const size_t tContentsLen = strContents.length() * sizeof(TCHAR);
    {
        HANDLE hFile = CreateFile(strFileName.c_str(), GENERIC_WRITE_, CREATE_ALWAYS_, 0);
        ASSERT_NE((HANDLE)NULL, hFile);

        EXPECT_TRUE(WriteFile(hFile, strContents.c_str(), tContentsLen, NULL));
        CloseFile(hFile);
    }

    {
        HANDLE hFile = CreateFile(strFileName.c_str(), GENERIC_READ_, OPEN_ALWAYS_, 0);
        ASSERT_NE((HANDLE)NULL, hFile);

        EXPECT_EQ(tContentsLen, GetFileSize(hFile));

        {
            HANDLE hFileMapping = CreateFileMapping(hFile, PAGE_READONLY_, FILE_MAP_READ_, tContentsLen);
            void* pFileMapping = MapViewOfFile(hFileMapping, 0, tContentsLen);
            EXPECT_NE((void*)NULL, pFileMapping);

            if( pFileMapping )
            {
                EXPECT_EQ(0, memcmp(pFileMapping, strContents.c_str(), tContentsLen));
                UnmapViewOfFile(hFileMapping, pFileMapping);
            }
            CloseFileMappingHandle(hFileMapping);
        }
        CloseFile(hFile);
    }

    DeleteFile(strFileName.c_str());
}

//////////////////////////////////////////////////////////////////////////
TEST(FileMappingTest, OpenFileMapping)
{
    std::tstring strFileName = TEXT("TempMappingFile.txt");

    std::tstring strContents = TEXT("Hello world!");
    const size_t tContentsLen = strContents.length() * sizeof(TCHAR);
    {
        HANDLE hFile = CreateFile(strFileName.c_str(), GENERIC_WRITE_, CREATE_ALWAYS_, 0);
        ASSERT_NE((HANDLE)NULL, hFile);

        EXPECT_TRUE(WriteFile(hFile, strContents.c_str(), tContentsLen, NULL));
        CloseFile(hFile);
    }

    {
        HANDLE hFile = CreateFile(strFileName.c_str(), GENERIC_READ_, OPEN_ALWAYS_, 0);
        ASSERT_NE((HANDLE)NULL, hFile);

        {
            size_t tMappingKey = 12012;
            HANDLE hFileMapping = CreateFileMapping(hFile, PAGE_READONLY_, FILE_MAP_READ_, tContentsLen, tMappingKey);
            {
                HANDLE hFileMapping2 = OpenFileMapping(FILE_MAP_READ_, tMappingKey);
                EXPECT_NE((void*)NULL, hFileMapping2);
                void* pFileMapping = MapViewOfFile(hFileMapping2, 0, tContentsLen);
                EXPECT_NE((void*)NULL, pFileMapping);

                if( pFileMapping )
                {
                    EXPECT_EQ(0, memcmp(pFileMapping, strContents.c_str(), tContentsLen));
                    UnmapViewOfFile(hFileMapping2, pFileMapping);
                }
                CloseFileMappingHandle(hFileMapping2);
            }
            CloseFileMappingHandle(hFileMapping);
        }
        CloseFile(hFile);
    }

    DeleteFile(strFileName.c_str());
}

//////////////////////////////////////////////////////////////////////////
TEST(SharedMemoryTest, CreateMemoryMapping)
{
    DWORD dwKey = 0x124;
    DWORD dwMemorySize = 1024;
    std::string strContents = "Hello memory mapping test!";

    HANDLE hFileMapping = CreateFileMapping(NULL, PAGE_EXECUTE_READWRITE_, FILE_MAP_WRITE_, dwMemorySize, dwKey);
    {
        ASSERT_NE((void*)NULL, hFileMapping);
        void* pFileMapping = MapViewOfFile(hFileMapping, 0, dwMemorySize);
        EXPECT_NE((void*)NULL, pFileMapping);

        if( pFileMapping )
        {
            memcpy(pFileMapping, strContents.c_str(), strContents.length());
            UnmapViewOfFile(hFileMapping, pFileMapping);
        }
    }

    {
        HANDLE hFileMapping2 = OpenFileMapping(FILE_MAP_READ_, dwKey);
        void* pFileMapping = MapViewOfFile(hFileMapping2, 0, dwMemorySize);
        EXPECT_NE((void*)NULL, pFileMapping);

        if( pFileMapping )
        {
            EXPECT_EQ(0, memcmp(pFileMapping, strContents.c_str(), strContents.length()));
            UnmapViewOfFile(hFileMapping, pFileMapping);
        }
        CloseFileMappingHandle(hFileMapping2);
    }

    CloseFileMappingHandle(hFileMapping);
}

//////////////////////////////////////////////////////////////////////////
TEST(SharedMemoryTest, OpenMemoryMapping)
{
	DWORD dwKey = 0x124;
	DWORD dwMemorySize = 1024;
	std::string strContents = "Hello memory mapping test!";

	HANDLE hFileMapping = OpenFileMapping(FILE_MAP_READ_, dwKey);
	EXPECT_EQ((HANDLE)NULL, hFileMapping);
	if( hFileMapping )
		CloseFileMappingHandle(hFileMapping);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, EnumProcessesTest)
{
	std::vector<ST_PROCESS_INFO> vecProcesses;	
	EXPECT_NE(0U, EnumProcesses(TEXT("*"), vecProcesses));

	std::tstring strModuleName = ExtractFileName(GetFileName());

	bool bThisProcIsExist = false;
	size_t i;
	for(i=0; i<vecProcesses.size(); i++)
	{
		if( vecProcesses[i].strName != strModuleName )
			continue;

		bThisProcIsExist = true;
		break;
	}

	EXPECT_TRUE(bThisProcIsExist);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, EnumProcessesTest_Pattern)
{
	std::tstring strModuleName = ExtractFileName(GetFileName());

	std::vector<ST_PROCESS_INFO> vecProcesses;	
	EXPECT_LE(1U, EnumProcesses(strModuleName, vecProcesses));
}

//////////////////////////////////////////////////////////////////////////
const int g_cThreadRet = 123;

//////////////////////////////////////////////////////////////////////////
struct ST_CRITICAL_SECTION_TEST_DATA
{
    HANDLE hCS;
    std::vector<int> vecResult;
};

//////////////////////////////////////////////////////////////////////////
static int CriticalSectionTestThreadFunc(void* pContext)
{
    try
    {
        ST_CRITICAL_SECTION_TEST_DATA* pTestData = (ST_CRITICAL_SECTION_TEST_DATA*)pContext;

        HANDLE    hCS    = pTestData->hCS;
        DWORD    dwRet    = EC_SUCCESS;

        EnterCriticalSection(hCS);

        pTestData->vecResult.push_back(5);
        pTestData->vecResult.push_back(6);
        pTestData->vecResult.push_back(7);
        pTestData->vecResult.push_back(8);

        LeaveCriticalSection(hCS);
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    return g_cThreadRet;
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CriticalSectionTest)
{
    ST_CRITICAL_SECTION_TEST_DATA    testData;

    HANDLE    hThread        = NULL;

    {
        HANDLE hCS1 = InitializeCriticalSection();
        HANDLE hCS2 = InitializeCriticalSection();
        HANDLE hCS3 = InitializeCriticalSection();
        HANDLE hCS4 = InitializeCriticalSection();
        HANDLE hCS5 = InitializeCriticalSection();
        HANDLE hCS6 = InitializeCriticalSection();
        HANDLE hCS7 = InitializeCriticalSection();
        HANDLE hCS8 = InitializeCriticalSection();
        HANDLE hCS9 = InitializeCriticalSection();
        HANDLE hCS10 = InitializeCriticalSection();

        EnterCriticalSection(hCS1);
        EnterCriticalSection(hCS2);
        EnterCriticalSection(hCS3);
        EnterCriticalSection(hCS4);
        EnterCriticalSection(hCS5);
        EnterCriticalSection(hCS6);
        EnterCriticalSection(hCS7);
        EnterCriticalSection(hCS8);
        EnterCriticalSection(hCS9);
        EnterCriticalSection(hCS10);

        LeaveCriticalSection(hCS10);
        LeaveCriticalSection(hCS9);
        LeaveCriticalSection(hCS8);
        LeaveCriticalSection(hCS7);
        LeaveCriticalSection(hCS6);
        LeaveCriticalSection(hCS5);
        LeaveCriticalSection(hCS4);
        LeaveCriticalSection(hCS3);
        LeaveCriticalSection(hCS2);
        LeaveCriticalSection(hCS1);

        DeleteCriticalSection(hCS1);
        DeleteCriticalSection(hCS2);
        DeleteCriticalSection(hCS3);
        DeleteCriticalSection(hCS4);
        DeleteCriticalSection(hCS5);
        DeleteCriticalSection(hCS6);
        DeleteCriticalSection(hCS7);
        DeleteCriticalSection(hCS8);
        DeleteCriticalSection(hCS9);
        DeleteCriticalSection(hCS10);
    }

    {
        testData.hCS = InitializeCriticalSection();
        ASSERT_NE((HANDLE)NULL, testData.hCS);
        EnterCriticalSection(testData.hCS);

        hThread = CreateThread(CriticalSectionTestThreadFunc, &testData);
        ASSERT_NE((HANDLE)NULL, hThread);

        testData.vecResult.push_back(1);        Sleep(100);
        testData.vecResult.push_back(2);        Sleep(100);
        testData.vecResult.push_back(3);        Sleep(100);
        testData.vecResult.push_back(4);        Sleep(100);

        LeaveCriticalSection(testData.hCS);

        int nRet = 0;
        ASSERT_EQ((DWORD)EC_SUCCESS, JoinThread(hThread, &nRet));
        EXPECT_EQ(nRet, g_cThreadRet);

        size_t i;
        for(i=1; i<testData.vecResult.size(); i++)
        {
            EXPECT_EQ(1, testData.vecResult[i] - testData.vecResult[i-1]);
        }
    }

    DeleteCriticalSection(testData.hCS);
    testData.hCS = NULL;
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CriticalSection_RecursiveLockTest)
{
    HANDLE hCS = InitializeCriticalSection();
    ASSERT_NE(hCS, (HANDLE)NULL);

    EnterCriticalSection(hCS);
    EnterCriticalSection(hCS);
    LeaveCriticalSection(hCS);
    LeaveCriticalSection(hCS);
    DeleteCriticalSection(hCS);
}

//////////////////////////////////////////////////////////////////////////
const E_THREAD_PRIORITY g_nPriority[] =
{
    THREAD_PRIORITY_BELOW_LOW    ,
    THREAD_PRIORITY_LOW            ,
    THREAD_PRIORITY_MIDDLE        ,
    THREAD_PRIORITY_HIGH        ,
    THREAD_PRIORITY_ABOVE_HIGH    ,
};
const size_t g_tPriorityCount = sizeof(g_nPriority) / sizeof(g_nPriority[0]);
int g_nFlagCount = 0;

//////////////////////////////////////////////////////////////////////////
int ThreadFunc_ThreadPriorityTest(void* pData)
{
    int* pnCount = ((int*)pData);

    while(InterlockedDecrement_(&g_nFlagCount) > 0)
    {
        InterlockedIncrement_(pnCount);
        Sleep(0);
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////
inline void CalcRank(const int nValues[g_tPriorityCount], int nRank[g_tPriorityCount])
{
    size_t i;
    for(i=0; i<g_tPriorityCount; i++)
    {
        size_t j;
        for(j=0; j<g_tPriorityCount; j++)
        {
            if( nValues[i] < nValues[j] )
                nRank[i]++;
        }
    }
}

//////////////////////////////////////////////////////////////////////////
int CalcPriorityDiffFactor(const int nCounts[g_tPriorityCount])
{
    int nRank[g_tPriorityCount] = { 0, };
    CalcRank(nCounts, nRank);

    size_t i;
    for(i=0; i<g_tPriorityCount; i++)
        printf("%d\t", nCounts[i]);
        //printf("%d:%d\t", nRank[i], nCounts[i]);
    printf("-> diff factor:");

    int nRet = 0;
    for(i=0; i<g_tPriorityCount; i++)
    {
        int nDiff = DIFF((int)(g_tPriorityCount - i - 1), nRank[i]);
        nRet += nDiff * nDiff;
    }
    return nRet;
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, DISABLED_ThreadPriorityTest)
{
    const size_t tSetCount = 2;
    const size_t tThreadCount = g_tPriorityCount * tSetCount;

    int nTestCount = 5;
    while(nTestCount--)
    {
        HANDLE hThread[tThreadCount] = { NULL, };
        int nCountArr[g_tPriorityCount] = { 0, };

#ifdef _MSC_VER
        g_nFlagCount = 100000;
#else
        g_nFlagCount = 1000000;
#endif

        size_t i;
        for(i=0; i<tThreadCount; i++)
        {
            size_t tIndex = i % g_tPriorityCount;
            hThread[i] = CreateThread(ThreadFunc_ThreadPriorityTest, &nCountArr[tIndex], g_nPriority[tIndex]);
        }

        for(i=0; i<tThreadCount; i++)
            JoinThread(hThread[i]);

        int nDiffFactor = 0;
        std::cout << (nDiffFactor = CalcPriorityDiffFactor(nCountArr)) << std::endl;
        EXPECT_GT(10, nDiffFactor);
    }
}

