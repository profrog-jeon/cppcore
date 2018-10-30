#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateProcess_StdPipeTest)
{
	std::vector<std::tstring> vecArgs;
	vecArgs.push_back(TEXT("output_test"));
	vecArgs.push_back(TEXT("text!!"));

	ST_STARTUPINFO stInfo;
	HANDLE hProcess = CreateProcess(TEXT("DummyProcess"), NULL, vecArgs, &stInfo);
	ASSERT_NE((HANDLE)NULL, hProcess);
	ASSERT_NE((HANDLE)NULL, stInfo.hStdInput);
	ASSERT_NE((HANDLE)NULL, stInfo.hStdOutput);
	ASSERT_NE((HANDLE)NULL, stInfo.hStdError);

	const size_t tBuffSize = 1024;
	char szBuff[tBuffSize];
	DWORD dwRead = 0;
	ReadFile(stInfo.hStdOutput, szBuff, tBuffSize, &dwRead);
	CloseProcessHandle(stInfo.hStdInput);
	CloseProcessHandle(stInfo.hStdOutput);
	//CloseProcessHandle(stInfo.hStdError);
	CloseProcessHandle(hProcess);

	szBuff[dwRead] = 0;
	EXPECT_EQ(vecArgs[1], TCSFromMBS(szBuff));
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, ShellExecuteTest_Dir)
{
	std::string strResult;
#ifdef _MSC_VER
	EXPECT_EQ(0, ShellExecuteByPipe("dir", strResult));
#else
    EXPECT_EQ(0, ShellExecuteByPipe("ls", strResult));
#endif
	EXPECT_FALSE(strResult.empty());
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, ShellExecuteTest_NonExistCommand)
{
	std::string strResult;
#ifdef _MSC_VER
	EXPECT_EQ(1, ShellExecuteByPipe("non-exist-command", strResult));
#else
	EXPECT_EQ(127, ShellExecuteByPipe("non-exist-command", strResult));
#endif
	EXPECT_TRUE(strResult.empty());
}

#ifdef _MSC_VER
#define CURRENT_DIR
#else
#define CURRENT_DIR		"./"
#endif

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, ShellExecuteTest_NoReturn)
{
	std::string strResult;
	EXPECT_EQ(0, ShellExecuteByPipe(CURRENT_DIR "DummyProcess output_test 123", strResult));
	EXPECT_EQ("123", strResult);
}


//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, ShellExecuteTest_LongLine)
{
	std::string strContext = "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";
	std::string strResult;
	EXPECT_EQ(EC_SUCCESS, ShellExecuteByPipe(Format(CURRENT_DIR "DummyProcess output_test %s", strContext.c_str()).c_str(), strResult));
	EXPECT_EQ(strContext, strResult);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateProcessTest_WaitTimeoutTest)
{
	std::vector<std::tstring> vecArgs;
	vecArgs.push_back(TEXT("hold"));

	HANDLE hProcess;
	ASSERT_NE((HANDLE)NULL, hProcess = CreateProcess(TEXT("DummyProcess"), NULL, vecArgs));

	DWORD dwTimeOut = 1000;
	DWORD dwStartTick = GetTickCount();
	EXPECT_EQ(EC_TIMEOUT, WaitForProcess(hProcess, dwTimeOut, NULL));

	DWORD dwElapsedTick = GetTickCount() - dwStartTick;
	EXPECT_LE(dwTimeOut, dwElapsedTick);
	EXPECT_GE(dwTimeOut+500, dwElapsedTick);

	TerminateProcess(hProcess);
	WaitForProcess(hProcess, INFINITE, NULL);
	CloseProcessHandle(hProcess);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateProcessTest_NonBlockTest)
{
    std::vector<std::tstring> vecArgs;
    vecArgs.push_back(TEXT("hold"));

    HANDLE hProcess;
    DWORD dwStartTick = GetTickCount();
    ASSERT_NE((HANDLE)NULL, hProcess = CreateProcess(TEXT("DummyProcess"), NULL, vecArgs));
    EXPECT_GT(1000U, GetTickCount() - dwStartTick);

    TerminateProcess(hProcess);
	WaitForProcess(hProcess, INFINITE, NULL);
	CloseProcessHandle(hProcess);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateProcessTest_ExitCodeTest)
{
    std::vector<std::tstring> vecArgs;
    vecArgs.push_back(TEXT("exitcode"));
    vecArgs.push_back(TEXT("123"));

    HANDLE hProcess;
    EXPECT_NE((HANDLE)NULL, hProcess = CreateProcess(TEXT("DummyProcess"), NULL, vecArgs));

    int nExitCode = 0;
    EXPECT_EQ(EC_SUCCESS, WaitForProcess(hProcess, INFINITE, &nExitCode));
    EXPECT_EQ(123, nExitCode);
	CloseProcessHandle(hProcess);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateProcessTest_Failure)
{
    EXPECT_EQ((HANDLE)NULL, CreateProcess(TEXT("NotExistProcess"), NULL));
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateProcessTest_DirectoryTest)
{
    std::vector<std::tstring> vecArgs;
    vecArgs.push_back(TEXT("filecheck"));
    vecArgs.push_back(TEXT("somefile"));

    CreateDirectory(TEXT("temp_dir"));
    WriteFileContents(TEXT("temp_dir/somefile"), TEXT("some text in here"));

	std::tstring strExePath = GetCurrentDirectory() + TEXT("/DummyProcess");
#ifdef _MSC_VER
	strExePath += TEXT(".exe");
#endif

    HANDLE hProcess;
    EXPECT_NE((HANDLE)NULL, hProcess = CreateProcess(strExePath.c_str(), TEXT("temp_dir"), vecArgs));

    int nExitCode = 0xFFFFFFFF;
    ::WaitForProcess(hProcess, INFINITE, &nExitCode);
    EXPECT_EQ(0, nExitCode);
	CloseProcessHandle(hProcess);

    DeleteFile(TEXT("temp_dir/somefile"));
    RemoveDirectory(TEXT("temp_dir"));
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateProcessTest_TerminateProcess)
{
    std::vector<std::tstring> vecArgs;
    vecArgs.push_back(TEXT("hold"));

    HANDLE hProcess;
    EXPECT_NE((HANDLE)NULL, hProcess = CreateProcess(TEXT("DummyProcess"), NULL, vecArgs));
    Sleep(100);

    {
        std::vector<ST_PROCESS_INFO> vecProc;
        EXPECT_EQ(1U, EnumProcesses(TEXT("DummyProcess*"), vecProc));

        size_t i;
        for(i=0; i<vecProc.size(); i++)
        {
            tprintf(TEXT("=========> %s(%lu)\n"), vecProc[i].strName.c_str(), vecProc[i].tPID);
        }
    }

    TerminateProcess(hProcess);
	WaitForProcess(hProcess, INFINITE, NULL);
	CloseProcessHandle(hProcess);

    {
        std::vector<ST_PROCESS_INFO> vecProc;
        EXPECT_EQ(0U, EnumProcesses(TEXT("DummyProcess*"), vecProc));
        
        size_t i;
        for(i=0; i<vecProc.size(); i++)
        {
            tprintf(TEXT("=========> %s(%lu)\n"), vecProc[i].strName.c_str(), vecProc[i].tPID);
        }
    }
}

//////////////////////////////////////////////////////////////////////////
void TestExitCode(int nTryExitCode, int nExpectExitCode)
{
	std::tstring strModuleDir = ExtractDirectory(GetFileName());

	std::vector<std::tstring> vecArgs;
	vecArgs.push_back(TEXT("exitcode"));
	vecArgs.push_back(StringFrom(nTryExitCode));

	HANDLE hProcess = CreateProcess(TEXT("DummyProcess"), strModuleDir.c_str(), vecArgs);

	int nExitCode = 0;
	EXPECT_EQ(EC_SUCCESS, WaitForProcess(hProcess, INFINITE, &nExitCode));
	EXPECT_EQ(nExpectExitCode, nExitCode);
	CloseProcessHandle(hProcess);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateProcessTest_ExceptionalExitCodeTest)
{
	TestExitCode(127, 127);
	TestExitCode(128, -128);
	TestExitCode(255, -1);
	TestExitCode(-1, -1);
	TestExitCode(256, 0);
	TestExitCode(0x0000FF00, 0);
	TestExitCode(0x00FF0000, 0);
	TestExitCode(0x7FFFFFFE, -2);
	TestExitCode(0x7FFFFFFF, -1);
	//TestExitCode(0x80000000, 0);	// do-not use exit-code over 0x80000000
	//TestExitCode(0xc0000005, 5);	// do-not use exit-code over 0x80000000
	//TestExitCode(0xFF000000, 0);	// do-not use exit-code over 0x80000000
}
