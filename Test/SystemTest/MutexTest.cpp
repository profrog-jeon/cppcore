#include "stdafx.h"
#include <vector>

LPCTSTR g_pszMutexKey = TEXT("1943");

//////////////////////////////////////////////////////////////////////////
int MutexAcquiredThread(void* pContext)
{
	HANDLE hMutex = (HANDLE)pContext;
	if( NULL == hMutex )
		return -1;

	ECODE nRet = WaitForMutex(hMutex, 200);
	if( EC_SUCCESS != nRet )
		return -2;

	Sleep(300);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, AnotherThread_ReleaseMutexTest)
{
	HANDLE hMutex = CreateMutex(false, g_pszMutexKey);
	ASSERT_NE((HANDLE)NULL, hMutex);

	HANDLE hThread = CreateThread(MutexAcquiredThread, hMutex);
	EXPECT_NE((HANDLE)NULL, hThread);
	if( NULL == hThread )
	{
		CloseMutex(hMutex);
		return;
	}

	Sleep(100);
	EXPECT_EQ(EC_TIMEOUT, WaitForMutex(hMutex, 10));
	EXPECT_FALSE(ReleaseMutex(hMutex));
	EXPECT_EQ(EC_TIMEOUT, WaitForMutex(hMutex, 10));

	int nExitCode = 0;
	JoinThread(hThread, &nExitCode);
	CloseMutex(hMutex);
	EXPECT_EQ(0, nExitCode);
}

//////////////////////////////////////////////////////////////////////////
int MutexWaitingThread(void* pContext)
{
	HANDLE hMutex = (HANDLE)pContext;
	if( NULL == hMutex )
		return -1;

	ECODE nRet = WaitForMutex(hMutex, 200);
	if( EC_TIMEOUT != nRet )
		return -2;

	nRet = WaitForMutex(hMutex, 200);
	if( WAIT_FAILED_ == nRet )
		return 0;

	return nRet;
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, MutexClose_AbandonedTest)
{
	HANDLE hMutex = CreateMutex(true, g_pszMutexKey);
	ASSERT_NE((HANDLE)NULL, hMutex);

	HANDLE hThread = CreateThread(MutexWaitingThread, hMutex);
	EXPECT_NE((HANDLE)NULL, hThread);
	if( NULL == hThread )
	{
		CloseMutex(hMutex);
		return;
	}

	Sleep(100);
	CloseMutex(hMutex);
	int nExitRet = 0;
	EXPECT_EQ(EC_SUCCESS, WaitForThread(hThread, 1000, &nExitRet));
	EXPECT_EQ(0, nExitRet);
	TerminateThread(hThread, 0);
	CloseThreadHandle(hThread);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, Interprocess_Mutex_OwnerTest)
{
	std::vector<std::tstring> vecArgs;
	vecArgs.push_back(TEXT("mutex_owner_create"));
	vecArgs.push_back(g_pszMutexKey);

	HANDLE hProcess;
	ASSERT_NE((HANDLE)NULL, hProcess = CreateProcess(TEXT("DummyProcess"), NULL, vecArgs, NULL, NULL));
	{
		Sleep(100);
		HANDLE hMutex = CreateMutex(true, g_pszMutexKey);
		ECODE nLastError;
		EXPECT_EQ(EC_ALREADY_EXISTS, nLastError = GetLastError());
		EXPECT_NE((HANDLE)NULL, hMutex);
		if( hMutex )
			CloseMutex(hMutex);
	}
	TerminateProcess(hProcess);
	WaitForProcess(hProcess, INFINITE, NULL);
	CloseProcessHandle(hProcess);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, UnnamedMutexCreateTest)
{
	HANDLE hHandle = NULL;
	EXPECT_NE((HANDLE)NULL, hHandle = CreateMutex(false, NULL));
	if( hHandle )
		CloseMutex(hHandle);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, MutexTest_RecursiveLock)
{
	HANDLE hMutex = CreateMutex(false, g_pszMutexKey);
	ASSERT_NE((HANDLE)NULL, hMutex);
	EXPECT_EQ(EC_SUCCESS, WaitForMutex(hMutex, 1000));
	EXPECT_EQ(EC_SUCCESS, WaitForMutex(hMutex, 1000));
	EXPECT_EQ(EC_SUCCESS, WaitForMutex(hMutex, 1000));

	ReleaseMutex(hMutex);
	ReleaseMutex(hMutex);
	ReleaseMutex(hMutex);
	CloseMutex(hMutex);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateAndOpen_MutexTest)
{
	{
		SCOPED_TRACE("Already created mutex should be nowhere");
		HANDLE hMutex;
		EXPECT_EQ((HANDLE)NULL, hMutex = OpenMutex(g_pszMutexKey));
		if( hMutex )
			CloseMutex(hMutex);
	}

	HANDLE hMutex = CreateMutex(false, g_pszMutexKey);
	{
		SCOPED_TRACE("Creating first mutex should be succeed");
		ASSERT_NE((HANDLE)NULL, hMutex);
	}	

	CloseMutex(hMutex);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateAndOpenTryWait_MutexTest)
{
	HANDLE hMutex = CreateMutex(false, g_pszMutexKey);
	ASSERT_NE((HANDLE)NULL, hMutex);

	EXPECT_EQ(EC_SUCCESS, WaitForMutex(hMutex, 0));
	ReleaseMutex(hMutex);

	std::vector<std::tstring> vecArgs;
	vecArgs.push_back(TEXT("mutex_trywait_and_release"));
	vecArgs.push_back(g_pszMutexKey);
	
	HANDLE hProcess;
	EXPECT_NE((HANDLE)NULL, hProcess = CreateProcess(TEXT("DummyProcess"), NULL, vecArgs, NULL, NULL));

	int nExitCode = 0xFFFFFFFF;
	EXPECT_EQ(EC_SUCCESS, WaitForProcess(hProcess, INFINITE, &nExitCode));
	EXPECT_EQ(0, nExitCode);
	CloseMutex(hMutex);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateAndOpenTryWait_AlreadAcquired_MutexTest)
{
	HANDLE hMutex = CreateMutex(false, g_pszMutexKey);
	ASSERT_NE((HANDLE)NULL, hMutex);

	{
		SCOPED_TRACE("First TryWait should be succeed");
		EXPECT_EQ(EC_SUCCESS, WaitForMutex(hMutex, 0));
	}

	std::vector<std::tstring> vecArgs;
	vecArgs.push_back(TEXT("mutex_trywait_and_release"));
	vecArgs.push_back(g_pszMutexKey);

	HANDLE hProcess;
	EXPECT_NE((HANDLE)NULL, hProcess = CreateProcess(TEXT("DummyProcess"), NULL, vecArgs, NULL, NULL));

	int nExitCode = 0xFFFFFFFF;
	EXPECT_EQ(EC_SUCCESS, WaitForProcess(hProcess, INFINITE, &nExitCode));
	EXPECT_EQ(-2, nExitCode);

	CloseMutex(hMutex);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, SeriesOfSelfTryWait_MutexTest)
{
	HANDLE hMutex = CreateMutex(false, g_pszMutexKey);
	ASSERT_NE((HANDLE)NULL, hMutex);

	std::vector<std::tstring> vecArgs;
	vecArgs.push_back(TEXT("mutex_trywait_and_release"));
	vecArgs.push_back(g_pszMutexKey);

	{
		HANDLE hProcess;
		EXPECT_NE((HANDLE)NULL, hProcess = CreateProcess(TEXT("DummyProcess"), NULL, vecArgs, NULL, NULL));
		int nExitCode = 0xFFFFFFFF;
		EXPECT_EQ(EC_SUCCESS, WaitForProcess(hProcess, INFINITE, &nExitCode));
		EXPECT_EQ(0, nExitCode);
	}
	
	EXPECT_EQ(EC_SUCCESS, WaitForMutex(hMutex, 0));

	{
		HANDLE hProcess;
		EXPECT_NE((HANDLE)NULL, hProcess = CreateProcess(TEXT("DummyProcess"), NULL, vecArgs, NULL, NULL));
		int nExitCode = 0xFFFFFFFF;
		EXPECT_EQ(EC_SUCCESS, WaitForProcess(hProcess, INFINITE, &nExitCode));
		EXPECT_EQ(-2, nExitCode);
		EXPECT_NE((HANDLE)NULL, hProcess = CreateProcess(TEXT("DummyProcess"), NULL, vecArgs, NULL, NULL));
		EXPECT_EQ(EC_SUCCESS, WaitForProcess(hProcess, INFINITE, &nExitCode));
		EXPECT_EQ(-2, nExitCode);
	}
	
	ReleaseMutex(hMutex);

	{
		HANDLE hProcess;
		EXPECT_NE((HANDLE)NULL, hProcess = CreateProcess(TEXT("DummyProcess"), NULL, vecArgs, NULL, NULL));
		int nExitCode = 0xFFFFFFFF;
		EXPECT_EQ(EC_SUCCESS, WaitForProcess(hProcess, INFINITE, &nExitCode));
		EXPECT_EQ(0, nExitCode);
		EXPECT_NE((HANDLE)NULL, hProcess = CreateProcess(TEXT("DummyProcess"), NULL, vecArgs, NULL, NULL));
		EXPECT_EQ(EC_SUCCESS, WaitForProcess(hProcess, INFINITE, &nExitCode));
		EXPECT_EQ(0, nExitCode);
	}

	CloseMutex(hMutex);
}

//////////////////////////////////////////////////////////////////////////
// windows kernel object failure, 
// MSDN: If your multithreaded application must repeatedly create, open, and
//       close a named mutex object, a race condition can occur. In this situation, 
//       it is better to use CreateMutex instead of OpenMutex, because CreateMutex
//       opens a mutex if it exists and creates it if it does not.
TEST(SystemTest, AbandonedLock_MutexTest)
{
	HANDLE hMutex = CreateMutex(false, g_pszMutexKey);
	ASSERT_NE((HANDLE)NULL, hMutex);
	

	std::vector<std::tstring> vecArgs;
	vecArgs.push_back(TEXT("mutex_trywait"));
	vecArgs.push_back(g_pszMutexKey);

	{
		HANDLE hProcess;
		EXPECT_NE((HANDLE)NULL, hProcess = CreateProcess(TEXT("DummyProcess"), NULL, vecArgs, NULL, NULL));
		if( hProcess )
		{
			int nExitCode = 0;
			EXPECT_EQ(EC_SUCCESS, WaitForProcess(hProcess, INFINITE, &nExitCode));
			EXPECT_EQ(0, nExitCode);
		}
	}

	{
		SCOPED_TRACE("Mutex lock should be remained on last lock aquired process which has already terminated.");
		HANDLE hProcess;
		EXPECT_NE((HANDLE)NULL, hProcess = CreateProcess(TEXT("DummyProcess"), NULL, vecArgs, NULL, NULL));
		if( hProcess )
		{
			int nExitCode = 0;
			EXPECT_EQ(EC_SUCCESS, WaitForProcess(hProcess, INFINITE, &nExitCode));
			EXPECT_EQ(-2, nExitCode);
		}
	}

	EXPECT_EQ(EC_ABANDONED, WaitForMutex(hMutex, 0));
	EXPECT_TRUE(ReleaseMutex(hMutex));
	EXPECT_EQ(EC_SUCCESS, WaitForMutex(hMutex, 0));
	CloseMutex(hMutex);
}

//////////////////////////////////////////////////////////////////////////
void TryRecreatedMutexTestThreadHelper(void)
{
	HANDLE hMutex = OpenMutex(g_pszMutexKey);
	ASSERT_NE((HANDLE)NULL, hMutex);
	EXPECT_EQ(EC_SUCCESS, WaitForMutex(hMutex, 0));
	printf("mutex locked\n");
	Sleep(500);
	printf("mutex unlocked\n");
	ReleaseMutex(hMutex);
	CloseMutex(hMutex);
}

//////////////////////////////////////////////////////////////////////////
int TryRecreatedMutexTestThread(void* pContext)
{	TryRecreatedMutexTestThreadHelper();
	return 0;
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, TryReopenedMutexTest)
{
	HANDLE hThread = NULL;
	{
		SCOPED_TRACE("First created mutex operation failure");
		HANDLE hMutex = CreateMutex(false, g_pszMutexKey);
		ASSERT_NE((HANDLE)NULL, hMutex);
		EXPECT_NE((HANDLE)NULL, hThread = CreateThread(TryRecreatedMutexTestThread, NULL));
		Sleep(100);
		EXPECT_EQ(EC_TIMEOUT, WaitForMutex(hMutex, 0));
		CloseMutex(hMutex);
	}

	{
		SCOPED_TRACE("Recreated mutex operation failure");
		HANDLE hMutex = OpenMutex(g_pszMutexKey);
		ASSERT_NE((HANDLE)NULL, hMutex);
		EXPECT_EQ(EC_TIMEOUT, WaitForMutex(hMutex, 0));
		ReleaseMutex(hMutex);
		CloseMutex(hMutex);
	}

	JoinThread(hThread);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, TryRecreatedMutexTest)
{
	HANDLE hThread = NULL;
	{
		SCOPED_TRACE("First created mutex operation failure");
		HANDLE hMutex = CreateMutex(false, g_pszMutexKey);
		ASSERT_NE((HANDLE)NULL, hMutex);
		EXPECT_NE((HANDLE)NULL, hThread = CreateThread(TryRecreatedMutexTestThread, NULL));
		Sleep(100);
		EXPECT_EQ(EC_TIMEOUT, WaitForMutex(hMutex, 0));
		CloseMutex(hMutex);
	}

	{
		SCOPED_TRACE("Recreated mutex operation failure");
		HANDLE hMutex = CreateMutex(false, g_pszMutexKey);
		ASSERT_NE((HANDLE)NULL, hMutex);
		EXPECT_EQ(EC_TIMEOUT, WaitForMutex(hMutex, 0));
		ReleaseMutex(hMutex);
		CloseMutex(hMutex);
	}

	JoinThread(hThread);
}

//////////////////////////////////////////////////////////////////////////
struct ST_MUTEX_TEST_DATA1
{
	std::vector<int> vecResult;
};

//////////////////////////////////////////////////////////////////////////
static int MutexTestThreadFunc(void* pContext)
{
	ST_MUTEX_TEST_DATA1* pTestData = (ST_MUTEX_TEST_DATA1*)pContext;
	HANDLE hMutex	= NULL;
	try
	{
		hMutex	= OpenMutex(g_pszMutexKey);
		if( NULL == hMutex )
			throw exception_format("OpenMutex failure");

		printf("thread try get mutex...");
		if( EC_SUCCESS != WaitForMutex(hMutex, 1000) )
			throw exception_format("WaitForMutex failure, %u", GetLastError());
		printf("done\n");

		pTestData->vecResult.push_back(5);
		pTestData->vecResult.push_back(6);
		pTestData->vecResult.push_back(7);
		pTestData->vecResult.push_back(8);

		if( !ReleaseMutex(hMutex) )
			throw exception_format("ReleaseMutex failure, %u", GetLastError());
	}
	catch(std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	if( hMutex )
		CloseMutex(hMutex);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, Wait_MutexTest)
{
	ST_MUTEX_TEST_DATA1	testData;
	HANDLE hMutex = CreateMutex(false, g_pszMutexKey);
	ASSERT_NE((HANDLE)NULL, hMutex);
	int nTestCount = 5;
	while(nTestCount--)
	{
		EXPECT_EQ(EC_SUCCESS, WaitForMutex(hMutex, 1000));
		testData.vecResult.clear();
		HANDLE hThread = CreateThread(MutexTestThreadFunc, &testData);
		ASSERT_NE((HANDLE)NULL, hThread);
		testData.vecResult.push_back(1);Sleep(10);
		testData.vecResult.push_back(2);Sleep(10);
		testData.vecResult.push_back(3);Sleep(10);
		testData.vecResult.push_back(4);Sleep(10);
		EXPECT_TRUE(ReleaseMutex(hMutex));

		JoinThread(hThread);

		size_t i;
		for(i=1; i<testData.vecResult.size(); i++)
		{
			std::string strErrMsg;
			size_t j;
			for(j=0; j<testData.vecResult.size(); j++)
				strErrMsg += Format("%d ", testData.vecResult[j]);
			strErrMsg += "\n";
			SCOPED_TRACE(strErrMsg.c_str());
			ASSERT_EQ(1, testData.vecResult[i] - testData.vecResult[i-1]);
		}
	}
	CloseMutex(hMutex);
}

