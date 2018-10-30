#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, EncodingLogTest)
{
	std::tstring strLogFilePath = TEXT("./Log/temp.txt");
	DeleteFile(strLogFilePath.c_str());

	InitLog(strLogFilePath, true);
	Log_Info("TEST1");
	Log_Info(L"TEST2");
	Log_Info(TEXT("TEST3"));

	std::tstring strEncContents;
	ReadFileContents(strLogFilePath, strEncContents);

	std::tstring strContents;

	int nTotalLen = (int)strEncContents.size();
	int nPos = 0;
	while(nPos >= 0)
	{
		std::tstring strToken = Tokenize(strEncContents, nPos);
		Trim(strToken);
		strContents += TCSFromMBS(DefaultDecodeString(strToken));
	}

	EXPECT_NE(std::tstring::npos, strContents.find(TEXT("TEST1")));
	EXPECT_NE(std::tstring::npos, strContents.find(TEXT("TEST2")));
	EXPECT_NE(std::tstring::npos, strContents.find(TEXT("TEST3")));
	EXPECT_TRUE(DeleteFile(strLogFilePath.c_str()));

	int nError = GetLastError();
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CrossCodeLogTest)
{
	std::tstring strLogFilePath = TEXT("./Log/temp.txt");
	DeleteFile(strLogFilePath.c_str());

	InitLog(strLogFilePath);
	Log_Info("TEST1");
	Log_Info(L"TEST2");
	Log_Info(TEXT("TEST3"));

	std::tstring strContents;
	ReadFileContents(strLogFilePath, strContents);

	EXPECT_NE(std::tstring::npos, strContents.find(TEXT("TEST1")));
	EXPECT_NE(std::tstring::npos, strContents.find(TEXT("TEST2")));
	EXPECT_NE(std::tstring::npos, strContents.find(TEXT("TEST3")));
	EXPECT_TRUE(DeleteFile(strLogFilePath.c_str()));

	int nError = GetLastError();
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, LogRotation_FileSizeTest)
{
	const DWORD dwFileCount = 5;

	ST_LOG_INIT_PARAM stParam;
	stParam.strLogPath = TEXT("./Log/Output.txt");
	stParam.dwOutputFlag = LOG_OUTPUT_FILE;
	stParam.dwMaxFileSize = 10000;
	stParam.dwMaxFileCount = dwFileCount;

	InitLog(stParam);

	size_t i;
	for(i=0; i<1000; i++)
	{
		Log_Info(TEXT("TEST"));
	}

	std::vector<std::tstring> vecFiles;
	GrepFiles(TEXT("./Log/"), TEXT("Output_*.txt"), vecFiles);
	system("ls Log");

	EXPECT_LT(0U, vecFiles.size());

	for(i=0; i<vecFiles.size(); i++)
	{
		FILE* pFile = fopenT(vecFiles[i].c_str(), TEXT("rb"));
		EXPECT_NE((FILE*)NULL, pFile);
		if( pFile )
		{
			fseek(pFile, 0, SEEK_END);
			size_t tFileSize = ftell(pFile);
			fclose(pFile);

			EXPECT_LT(stParam.dwMaxFileSize, tFileSize);
		}
		EXPECT_TRUE(DeleteFile(vecFiles[i].c_str()));
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, LogRotation_FileCountTest)
{
	const DWORD dwFileCount = 5;

	ST_LOG_INIT_PARAM stParam;
	stParam.strLogPath = TEXT("./Log/Output.txt");
	stParam.dwOutputFlag = LOG_OUTPUT_FILE;
	stParam.dwMaxFileSize = 10000;
	stParam.dwMaxFileCount = dwFileCount;

	InitLog(stParam);

	size_t i;
	for(i=0; i<1000; i++)
	{
		Log_Info(TEXT("TEST"));
	}

	std::vector<std::tstring> vecFiles;
	GrepFiles(TEXT("./Log/"), TEXT("Output*.txt"), vecFiles);

	EXPECT_LE(dwFileCount-1, vecFiles.size());

	for(i=0; i<vecFiles.size(); i++)
	{
		EXPECT_TRUE(DeleteFile(vecFiles[i].c_str()));
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, LogTest)
{
	InitLog(TEXT("./Log/Output.txt"));
	Log_Info(TEXT("TEST1"));
	Log_Info(TEXT("TEST2"));
	Log_Info(TEXT("TEST3"));
	Log_Info(TEXT("TEST4"));
	EXPECT_TRUE(DeleteFile(TEXT("Log/Output.txt")));
}
