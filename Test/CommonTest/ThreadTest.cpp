#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
static std::tstring g_strSuccessMessage = TEXT("Member function thread message.");
static int g_nExitCode = 123;

//////////////////////////////////////////////////////////////////////////
class CMemberFuncThreadTest
{
public:
	std::tstring strMessage;
	CMemberFuncThreadTest(void)	{}

	int ThreadFunc(void* pContext)
	{
		strMessage = g_strSuccessMessage;
		return g_nExitCode;
	}
};

//////////////////////////////////////////////////////////////////////////
TEST(MemberFuncThreadTest, FunctionCallingTest)
{
	CMemberFuncThreadTest Test;
	HANDLE hThread = CreateThread(&Test, &CMemberFuncThreadTest::ThreadFunc, NULL);
	ASSERT_NE((HANDLE)NULL, hThread);

	int nExitCode = 0;
	EXPECT_EQ(EC_SUCCESS, JoinThread(hThread, &nExitCode));
	EXPECT_EQ(g_strSuccessMessage, Test.strMessage);
}

//////////////////////////////////////////////////////////////////////////
TEST(MemberFuncThreadTest, FunctionRetCodeTest)
{
	CMemberFuncThreadTest Test;
	HANDLE hThread = CreateThread(&Test, &CMemberFuncThreadTest::ThreadFunc, NULL);
	ASSERT_NE((HANDLE)NULL, hThread);

	int nExitCode = 0;
	EXPECT_EQ(EC_SUCCESS, JoinThread(hThread, &nExitCode));
	EXPECT_EQ(g_nExitCode, nExitCode);
}