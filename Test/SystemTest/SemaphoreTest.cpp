#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
TEST(UnnamedSemaphoreTest, DoubleClose)
{
    HANDLE hSemaphore = CreateSemaphore(3);
    ASSERT_NE((HANDLE)NULL, hSemaphore);
    CloseSemaphore(hSemaphore);
    CloseSemaphore(hSemaphore);
}

//////////////////////////////////////////////////////////////////////////
TEST(UnnamedSemaphoreTest, Wait)
{
    HANDLE hSemaphore = CreateSemaphore(3);
    EXPECT_NE((HANDLE)NULL, hSemaphore);

    DWORD dwStartTick = GetTickCount();
    EXPECT_EQ(EC_SUCCESS, WaitForSemaphore(hSemaphore, 2000));
    EXPECT_EQ(EC_SUCCESS, WaitForSemaphore(hSemaphore, 2000));
    EXPECT_EQ(EC_SUCCESS, WaitForSemaphore(hSemaphore, 2000));
    DWORD dwElapsedTick = GetTickCount() - dwStartTick;
    EXPECT_LT(dwElapsedTick, (DWORD)100);
    CloseSemaphore(hSemaphore);
}

//////////////////////////////////////////////////////////////////////////
TEST(UnnamedSemaphoreTest, WaitTimeOut)
{
    HANDLE hSemaphore = CreateSemaphore(0);
    EXPECT_NE((HANDLE)NULL, hSemaphore);

    DWORD dwStartTick = GetTickCount();
    EXPECT_EQ(EC_TIMEOUT, WaitForSemaphore(hSemaphore, 200));
    DWORD dwElapsedTick = GetTickCount() - dwStartTick;
    EXPECT_LT(DIFF(dwElapsedTick, (DWORD)200), (DWORD)50);
    CloseSemaphore(hSemaphore);
}

//////////////////////////////////////////////////////////////////////////
TEST(UnnamedSemaphoreTest, WaitSemaphoreAndRelease)
{
    HANDLE hSemaphore = CreateSemaphore(1);
    EXPECT_NE((HANDLE)NULL, hSemaphore);

    EXPECT_EQ(EC_SUCCESS, WaitForSemaphore(hSemaphore, 20));
    EXPECT_EQ(EC_TIMEOUT, WaitForSemaphore(hSemaphore, 20));
    ReleaseSemaphore(hSemaphore);
    ReleaseSemaphore(hSemaphore);
    EXPECT_EQ(EC_SUCCESS, WaitForSemaphore(hSemaphore, 20));
    EXPECT_EQ(EC_SUCCESS, WaitForSemaphore(hSemaphore, 20));
    EXPECT_EQ(EC_TIMEOUT, WaitForSemaphore(hSemaphore, 20));
    CloseSemaphore(hSemaphore);
}

//////////////////////////////////////////////////////////////////////////
TEST(UnnamedSemaphoreTest, WaitSemaphoreAndMultiRelease)
{
    HANDLE hSemaphore = CreateSemaphore(1);
    EXPECT_NE((HANDLE)NULL, hSemaphore);

    ReleaseSemaphore(hSemaphore, 3);
    EXPECT_EQ(EC_SUCCESS, WaitForSemaphore(hSemaphore, 20));
    EXPECT_EQ(EC_SUCCESS, WaitForSemaphore(hSemaphore, 20));
    EXPECT_EQ(EC_SUCCESS, WaitForSemaphore(hSemaphore, 20));
    EXPECT_EQ(EC_SUCCESS, WaitForSemaphore(hSemaphore, 20));
    EXPECT_EQ(EC_TIMEOUT, WaitForSemaphore(hSemaphore, 20));
    CloseSemaphore(hSemaphore);
}

//////////////////////////////////////////////////////////////////////////
TEST(NamedSemaphoreTest, WaitSemaphoreTimeoutPrecisen)
{
    HANDLE hSemaphore = CreateSemaphore(1, TEXT("test"));
    EXPECT_NE((HANDLE)NULL, hSemaphore);

    HANDLE hSemaphore2 = OpenSemaphore(TEXT("test"));
    EXPECT_NE((HANDLE)NULL, hSemaphore2);

    DWORD dwStartTick = GetTickCount();
    EXPECT_EQ(EC_SUCCESS, WaitForSemaphore(hSemaphore2, 2000));
    DWORD dwElapsedTick = GetTickCount() - dwStartTick;
    EXPECT_LT(dwElapsedTick, (DWORD)100);
    CloseSemaphore(hSemaphore2);
    CloseSemaphore(hSemaphore);
}

//////////////////////////////////////////////////////////////////////////
TEST(NamedSemaphoreTest, WaitSemaphoreAfterCloseOpen_linux_failure)
{
    HANDLE hSemaphore = CreateSemaphore(1, TEXT("test"));
    EXPECT_NE((HANDLE)NULL, hSemaphore);
    {
        HANDLE hSemaphore2 = OpenSemaphore(TEXT("test"));
        EXPECT_NE((HANDLE)NULL, hSemaphore2);
        CloseSemaphore(hSemaphore2);
    }
    HANDLE hSemaphore2 = OpenSemaphore(TEXT("test"));
    EXPECT_NE((HANDLE)NULL, hSemaphore2);
    DWORD dwStartTick = GetTickCount();
    EXPECT_EQ(EC_SUCCESS, WaitForSemaphore(hSemaphore2, 2000));
    DWORD dwElapsedTick = GetTickCount() - dwStartTick;
    EXPECT_LT(dwElapsedTick, (DWORD)100);
    CloseSemaphore(hSemaphore2);
    CloseSemaphore(hSemaphore);
}

//////////////////////////////////////////////////////////////////////////
TEST(NamedSemaphoreTest, WaitSemaphoreAndRelease)
{
    HANDLE hSemaphore = CreateSemaphore(1, TEXT("test"));
    EXPECT_NE((HANDLE)NULL, hSemaphore);

    HANDLE hSemaphore2 = OpenSemaphore(TEXT("test"));
    EXPECT_NE((HANDLE)NULL, hSemaphore2);

    EXPECT_EQ(EC_SUCCESS, WaitForSemaphore(hSemaphore2, 20));
    EXPECT_EQ(EC_TIMEOUT, WaitForSemaphore(hSemaphore2, 20));
    ReleaseSemaphore(hSemaphore);
    ReleaseSemaphore(hSemaphore2);
    EXPECT_EQ(EC_SUCCESS, WaitForSemaphore(hSemaphore2, 20));
    EXPECT_EQ(EC_SUCCESS, WaitForSemaphore(hSemaphore2, 20));
    EXPECT_EQ(EC_TIMEOUT, WaitForSemaphore(hSemaphore2, 20));
    CloseSemaphore(hSemaphore);
    CloseSemaphore(hSemaphore2);
}

//////////////////////////////////////////////////////////////////////////
TEST(NamedSemaphoreTest, WaitSemaphoreAndMultiRelease)
{
    HANDLE hSemaphore = CreateSemaphore(1, TEXT("test"));
    EXPECT_NE((HANDLE)NULL, hSemaphore);
    
    HANDLE hSemaphore2 = OpenSemaphore(TEXT("test"));
    EXPECT_NE((HANDLE)NULL, hSemaphore2);
    
    ReleaseSemaphore(hSemaphore, 3);
    EXPECT_EQ(EC_SUCCESS, WaitForSemaphore(hSemaphore2, 20));
    EXPECT_EQ(EC_SUCCESS, WaitForSemaphore(hSemaphore2, 20));
    EXPECT_EQ(EC_SUCCESS, WaitForSemaphore(hSemaphore2, 20));
    EXPECT_EQ(EC_SUCCESS, WaitForSemaphore(hSemaphore2, 20));
    EXPECT_EQ(EC_TIMEOUT, WaitForSemaphore(hSemaphore2, 20));
    
    CloseSemaphore(hSemaphore);
    CloseSemaphore(hSemaphore2);
}

//////////////////////////////////////////////////////////////////////////
TEST(NamedSemaphoreTest, OpenFirst)
{
    ASSERT_EQ((HANDLE)NULL, OpenSemaphore(TEXT("test")));
}

//////////////////////////////////////////////////////////////////////////
TEST(NamedSemaphoreTest, TwiceCreate)
{
    HANDLE hSemaphore, hSemaphore2;
    ASSERT_NE((HANDLE)NULL, hSemaphore = CreateSemaphore(1, TEXT("test")));
    EXPECT_NE((HANDLE)NULL, hSemaphore2 = CreateSemaphore(1, TEXT("test")));
    CloseSemaphore(hSemaphore);
    CloseSemaphore(hSemaphore2);
}

//////////////////////////////////////////////////////////////////////////
TEST(NamedSemaphoreTest, TwiceCreateIgnored)
{
    HANDLE hSemaphore = CreateSemaphore(1, TEXT("test"));
    ASSERT_NE((HANDLE)NULL, hSemaphore);

    HANDLE hSemaphore2 = CreateSemaphore(10, TEXT("test"));
    ASSERT_NE((HANDLE)NULL, hSemaphore);

    {
        DWORD dwStartTick = GetTickCount();
        EXPECT_EQ(EC_SUCCESS, WaitForSemaphore(hSemaphore2, 200));
        DWORD dwElapsedTick = GetTickCount() - dwStartTick;
        EXPECT_LT(dwElapsedTick, 50U);
    }

    {
        DWORD dwStartTick = GetTickCount();
        EXPECT_EQ(EC_TIMEOUT, WaitForSemaphore(hSemaphore2, 200));
        DWORD dwElapsedTick = GetTickCount() - dwStartTick;
        EXPECT_LT(DIFF(dwElapsedTick, (DWORD)200U), 50U);
    }

    CloseSemaphore(hSemaphore);
    CloseSemaphore(hSemaphore2);
}

//////////////////////////////////////////////////////////////////////////
TEST(NamedSemaphoreTest, TwiceCreate_SecondSucceed)
{
    HANDLE hSemaphore = CreateSemaphore(1, TEXT("test"));
    ASSERT_NE((HANDLE)NULL, hSemaphore);

    HANDLE hSemaphore2 = CreateSemaphore(10, TEXT("test"));
    ASSERT_NE((HANDLE)NULL, hSemaphore);

    {
        DWORD dwStartTick = GetTickCount();
        EXPECT_EQ(EC_SUCCESS, WaitForSemaphore(hSemaphore2, 200));
        DWORD dwElapsedTick = GetTickCount() - dwStartTick;
        EXPECT_LT(dwElapsedTick, 100U);
    }

    {
        DWORD dwStartTick = GetTickCount();
        EXPECT_EQ(EC_TIMEOUT, WaitForSemaphore(hSemaphore2, 200));
        DWORD dwElapsedTick = GetTickCount() - dwStartTick;
        EXPECT_LT(DIFF(dwElapsedTick, (DWORD)200U), 100U);
    }

    CloseSemaphore(hSemaphore);
    CloseSemaphore(hSemaphore2);
}

//////////////////////////////////////////////////////////////////////////
TEST(NamedSemaphoreTest, SingleOpenClose_InterProcessSemaphore)
{
    std::tstring strName = TEXT("semaphore_test");
    HANDLE hSemaphore = CreateSemaphore(0, strName.c_str());
    ASSERT_NE((HANDLE)NULL, hSemaphore);

    std::tstring strModuleDir = ExtractDirectory(GetFileName());
    std::vector<std::tstring> vecArgs;
    vecArgs.push_back(TEXT("semaphore_open_close"));
    vecArgs.push_back(strName);
    HANDLE hProcess = CreateProcess(TEXT("DummyProcess"), strModuleDir.c_str(), vecArgs);

    int nExitCode = 0;
    EXPECT_EQ(EC_SUCCESS, WaitForProcess(hProcess, INFINITE, &nExitCode));
    EXPECT_EQ(0, nExitCode);
    CloseSemaphore(hSemaphore);
}

//////////////////////////////////////////////////////////////////////////
TEST(NamedSemaphoreTest, MultipleOpenClose_InterProcessSemaphore)
{
	std::tstring strName = TEXT("semaphore_test");
	HANDLE hSemaphore = CreateSemaphore(0, strName.c_str());
	ASSERT_NE((HANDLE)NULL, hSemaphore);

	std::tstring strModuleDir = ExtractDirectory(GetFileName());
	std::vector<std::tstring> vecArgs;
	vecArgs.push_back(TEXT("semaphore_create_close"));
	vecArgs.push_back(strName);

	int i;
	for(i=0; i<5; i++)
	{
		HANDLE hProcess = CreateProcess(TEXT("DummyProcess"), strModuleDir.c_str(), vecArgs);
		int nExitCode = 0;
		EXPECT_EQ(EC_SUCCESS, WaitForProcess(hProcess, INFINITE, &nExitCode));
		EXPECT_EQ(0, nExitCode);
		Sleep(10);
	}

	CloseSemaphore(hSemaphore);
}

//////////////////////////////////////////////////////////////////////////
TEST(NamedSemaphoreTest, CreateSemaphoreExitAutoClear_linux_failure)
{
    std::tstring strName = TEXT("semaphore_test");
    {
        std::tstring strModuleDir = ExtractDirectory(GetFileName());
        std::vector<std::tstring> vecArgs;
        vecArgs.push_back(TEXT("semaphore_create_noclose"));
        vecArgs.push_back(strName);
        HANDLE hProcess = CreateProcess(TEXT("DummyProcess"), strModuleDir.c_str(), vecArgs);

        int nExitCode = 0;
        EXPECT_EQ(EC_SUCCESS, WaitForProcess(hProcess, INFINITE, &nExitCode));
        EXPECT_EQ(0, nExitCode);
    }

    {    // check still exist the semaphore
        HANDLE hSemaphore = OpenSemaphore(strName.c_str());
        EXPECT_EQ((HANDLE)NULL, hSemaphore);
        if( hSemaphore )
            CloseSemaphore(hSemaphore);
    }
}

//////////////////////////////////////////////////////////////////////////
TEST(NamedSemaphoreTest, CreateSemaphoreExitAutoClear_linux_failure2)
{
    std::tstring strName = TEXT("semaphore_test");
    HANDLE hGlobalSemaphore = NULL;
    {
        std::tstring strModuleDir = ExtractDirectory(GetFileName());
        std::vector<std::tstring> vecArgs;
        vecArgs.push_back(TEXT("semaphore_create_noclose"));
        vecArgs.push_back(strName);
        HANDLE hProcess = CreateProcess(TEXT("DummyProcess"), strModuleDir.c_str(), vecArgs);

        int nExitCode = 0;
        EXPECT_EQ(EC_SUCCESS, WaitForProcess(hProcess, INFINITE, &nExitCode));
        EXPECT_EQ(0, nExitCode);

        hGlobalSemaphore = CreateSemaphore(1, strName.c_str());
    }

    {
        HANDLE hSemaphore = CreateSemaphore(1, strName.c_str());
        CloseSemaphore(hSemaphore);
    }

    {    // check still exist the semaphore
        HANDLE hSemaphore = OpenSemaphore(strName.c_str());
        EXPECT_NE((HANDLE)NULL, hSemaphore);
        if( hSemaphore )
            CloseSemaphore(hSemaphore);
    }
    CloseSemaphore(hGlobalSemaphore);
}

//////////////////////////////////////////////////////////////////////////
// linux succeed
TEST(NamedSemaphoreTest, CreateSemaphoreExitAutoClear)
{
    std::tstring strName = TEXT("semaphore_test");
    {
        std::tstring strModuleDir = ExtractDirectory(GetFileName());
        std::vector<std::tstring> vecArgs;
        vecArgs.push_back(TEXT("semaphore_create_noclose"));
        vecArgs.push_back(strName);
        HANDLE hProcess = CreateProcess(TEXT("DummyProcess"), strModuleDir.c_str(), vecArgs);

        int nExitCode = 0;
        EXPECT_EQ(EC_SUCCESS, WaitForProcess(hProcess, INFINITE, &nExitCode));
        EXPECT_EQ(0, nExitCode);
    }

    {
        HANDLE hSemaphore = CreateSemaphore(1, strName.c_str());
        CloseSemaphore(hSemaphore);
    }

    {    // check still exist the semaphore
        HANDLE hSemaphore = OpenSemaphore(strName.c_str());
        EXPECT_EQ((HANDLE)NULL, hSemaphore);
        if( hSemaphore )
            CloseSemaphore(hSemaphore);
    }
}

//////////////////////////////////////////////////////////////////////////
TEST(NamedSemaphoreTest, OpenSemaphoreExitAutoClear)
{
    std::tstring strName = TEXT("semaphore_test");
    {
        HANDLE hSemaphore = CreateSemaphore(0, strName.c_str());
        ASSERT_NE((HANDLE)NULL, hSemaphore);

        std::tstring strModuleDir = ExtractDirectory(GetFileName());
        std::vector<std::tstring> vecArgs;
        vecArgs.push_back(TEXT("semaphore_open_noclose"));
        vecArgs.push_back(strName);
        HANDLE hProcess = CreateProcess(TEXT("DummyProcess"), strModuleDir.c_str(), vecArgs);

        int nExitCode = 0;
        EXPECT_EQ(EC_SUCCESS, WaitForProcess(hProcess, INFINITE, &nExitCode));
        EXPECT_EQ(0, nExitCode);
        CloseSemaphore(hSemaphore);
    }

    {    // check still exist the semaphore
        HANDLE hSemaphore = OpenSemaphore(strName.c_str());
        EXPECT_EQ((HANDLE)NULL, hSemaphore);
        if( hSemaphore )
            CloseSemaphore(hSemaphore);
    }
}

//////////////////////////////////////////////////////////////////////////
TEST(NamedSemaphoreTest, InterProcessReleaseSemaphore)
{
    std::tstring strName = TEXT("semaphore_test");

    HANDLE hSemaphore = CreateSemaphore(0, strName.c_str());
    ASSERT_NE((HANDLE)NULL, hSemaphore);
    EXPECT_EQ(EC_TIMEOUT, WaitForSemaphore(hSemaphore, 100));

    std::tstring strModuleDir = ExtractDirectory(GetFileName());
    std::vector<std::tstring> vecArgs;
    vecArgs.push_back(TEXT("semaphore_open_release_close"));
    vecArgs.push_back(strName);
    HANDLE hProcess = CreateProcess(TEXT("DummyProcess"), strModuleDir.c_str(), vecArgs);

    int nExitCode = 0;
    EXPECT_EQ(EC_SUCCESS, WaitForProcess(hProcess, INFINITE, &nExitCode));
    EXPECT_EQ(0, nExitCode);

    EXPECT_EQ(EC_SUCCESS, WaitForSemaphore(hSemaphore, 100));
    CloseSemaphore(hSemaphore);
}
