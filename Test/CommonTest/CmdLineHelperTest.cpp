#include "stdafx.h"

TEST(CmdLineHelperTest, args2CmdLine2args)
{
	std::vector<std::tstring> vecArgs;
	vecArgs.push_back(TEXT("1"));
	vecArgs.push_back(TEXT("2 3"));
	vecArgs.push_back(TEXT("4"));

	std::tstring strCmdLine = MakeCmdLine(vecArgs);
	std::vector<std::tstring> vecArgsRestored;
	ParseCmdLine(strCmdLine, vecArgsRestored);

	ASSERT_EQ(vecArgs.size(), vecArgsRestored.size());

	for (size_t i = 0; i < vecArgs.size(); i++)
	{
		EXPECT_EQ(vecArgs[i], vecArgsRestored[i]);
	}
}

TEST(CmdLineHelperTest, CmdLine2args2CmdLine)
{
	std::tstring strCmdLine = TEXT("1 \"2 3\" 4");
	
	std::vector<std::tstring> vecArgs;
	ParseCmdLine(strCmdLine, vecArgs);

	std::tstring strCmdLineRestored = MakeCmdLine(vecArgs);
	EXPECT_EQ(strCmdLine, strCmdLineRestored);
}

TEST(CmdLineHelperTest, QuotaionIncludeTest)
{
	std::tstring strCmdLine = TEXT("some.exe log-path:\"c:\\hello\\world\"");

	std::vector<std::tstring> vecArgs;
	ParseCmdLine(strCmdLine, vecArgs);

	std::tstring strCmdLineRestored = MakeCmdLine(vecArgs);
	EXPECT_EQ(strCmdLine, strCmdLineRestored);
}

TEST(CmdLineHelperTest, QuotaionIncludeTest2)
{
	std::tstring strCmdLine = TEXT("some.exe log-path:\"c:\\hello world\"");

	std::vector<std::tstring> vecArgs;
	ParseCmdLine(strCmdLine, vecArgs);

	std::tstring strCmdLineRestored = MakeCmdLine(vecArgs);
	EXPECT_EQ(strCmdLine, strCmdLineRestored);
}
