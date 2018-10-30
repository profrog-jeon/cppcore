#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
HANDLE CreateFileTestWorker(LPCTSTR pszPath, DWORD dwDesiredAccess, E_FILE_DISPOSITION nDisposition)
{
	HANDLE hFile = CreateFile(pszPath, dwDesiredAccess, nDisposition, 0);
	if( hFile )
		CloseFile(hFile);

	DeleteFile(pszPath);
	return hFile;
}

//////////////////////////////////////////////////////////////////////////
TEST(FileTest, CreateFileTest)
{
	EXPECT_TRUE(NULL != CreateFileTestWorker(TEXT("some.test.file"), GENERIC_READ_|GENERIC_WRITE_, CREATE_NEW_			));
	EXPECT_TRUE(NULL != CreateFileTestWorker(TEXT("some.test.file"), GENERIC_READ_|GENERIC_WRITE_, CREATE_ALWAYS_		));
	EXPECT_TRUE(NULL == CreateFileTestWorker(TEXT("some.test.file"), GENERIC_READ_|GENERIC_WRITE_, OPEN_EXISTING_		));
	EXPECT_TRUE(NULL != CreateFileTestWorker(TEXT("some.test.file"), GENERIC_READ_|GENERIC_WRITE_, OPEN_ALWAYS_			));
	EXPECT_TRUE(NULL == CreateFileTestWorker(TEXT("some.test.file"), GENERIC_READ_|GENERIC_WRITE_, TRUNCATE_EXISTING_	));
}

//////////////////////////////////////////////////////////////////////////
HANDLE CreateExistFileTestWorker(LPCTSTR pszPath, DWORD dwDesiredAccess, E_FILE_DISPOSITION nDisposition)
{
	WriteFileContents(pszPath, TEXT("some text"));

	HANDLE hFile = CreateFile(pszPath, dwDesiredAccess, nDisposition, 0);
	if( hFile )
		CloseFile(hFile);

	DeleteFile(pszPath);
	return hFile;
}

//////////////////////////////////////////////////////////////////////////
TEST(FileTest, CreateExistFileTest)
{
	EXPECT_TRUE(NULL == CreateExistFileTestWorker(TEXT("some.test.file"), GENERIC_READ_|GENERIC_WRITE_, CREATE_NEW_			));
	EXPECT_TRUE(NULL != CreateExistFileTestWorker(TEXT("some.test.file"), GENERIC_READ_|GENERIC_WRITE_, CREATE_ALWAYS_		));
	EXPECT_TRUE(NULL != CreateExistFileTestWorker(TEXT("some.test.file"), GENERIC_READ_|GENERIC_WRITE_, OPEN_EXISTING_		));
	EXPECT_TRUE(NULL != CreateExistFileTestWorker(TEXT("some.test.file"), GENERIC_READ_|GENERIC_WRITE_, OPEN_ALWAYS_		));
	EXPECT_TRUE(NULL != CreateExistFileTestWorker(TEXT("some.test.file"), GENERIC_READ_|GENERIC_WRITE_, TRUNCATE_EXISTING_	));
}

//////////////////////////////////////////////////////////////////////////
int CreateExistFilePositionTestWorker(LPCTSTR pszPath, DWORD dwDesiredAccess, E_FILE_DISPOSITION nDisposition)
{
	WriteFileContents(pszPath, TEXT("some text"));

	int64_t nCurrentPos = -1;

	HANDLE hFile = CreateFile(pszPath, dwDesiredAccess, nDisposition, 0);
	if( hFile )
	{
		WriteFile(hFile, "", 1, NULL);
		nCurrentPos = SetFilePointer(hFile, -1, FILE_CURRENT_);
		CloseFile(hFile);
	}

	DeleteFile(pszPath);
	return (int)nCurrentPos;
}

//////////////////////////////////////////////////////////////////////////
TEST(FileTest, CreateExistFilePositionTest)
{
	EXPECT_EQ(-1, CreateExistFilePositionTestWorker(TEXT("some.test.file"), GENERIC_READ_, CREATE_ALWAYS_		));
	EXPECT_EQ(-1, CreateExistFilePositionTestWorker(TEXT("some.test.file"), GENERIC_READ_, OPEN_EXISTING_		));
	EXPECT_EQ(-1, CreateExistFilePositionTestWorker(TEXT("some.test.file"), GENERIC_READ_, OPEN_ALWAYS_			));
	EXPECT_EQ(-1, CreateExistFilePositionTestWorker(TEXT("some.test.file"), GENERIC_READ_, TRUNCATE_EXISTING_	));

	EXPECT_EQ(0, CreateExistFilePositionTestWorker(TEXT("some.test.file"), GENERIC_WRITE_, CREATE_ALWAYS_		));
	EXPECT_EQ(0, CreateExistFilePositionTestWorker(TEXT("some.test.file"), GENERIC_WRITE_, OPEN_EXISTING_		));
	EXPECT_EQ(0, CreateExistFilePositionTestWorker(TEXT("some.test.file"), GENERIC_WRITE_, OPEN_ALWAYS_			));
	EXPECT_EQ(0, CreateExistFilePositionTestWorker(TEXT("some.test.file"), GENERIC_WRITE_, TRUNCATE_EXISTING_	));

	EXPECT_EQ(0, CreateExistFilePositionTestWorker(TEXT("some.test.file"), FILE_APPEND_DATA_, CREATE_ALWAYS_	));
	EXPECT_LT(0, CreateExistFilePositionTestWorker(TEXT("some.test.file"), FILE_APPEND_DATA_, OPEN_EXISTING_	));
	EXPECT_LT(0, CreateExistFilePositionTestWorker(TEXT("some.test.file"), FILE_APPEND_DATA_, OPEN_ALWAYS_		));
	EXPECT_EQ(-1, CreateExistFilePositionTestWorker(TEXT("some.test.file"), FILE_APPEND_DATA_, TRUNCATE_EXISTING_));
}

//////////////////////////////////////////////////////////////////////////
std::string WriteFileTestWorker(LPCTSTR pszPath, DWORD dwDesiredAccess, E_FILE_DISPOSITION nDisposition, std::string strContents)
{
	DWORD dwWritten = 0;
	HANDLE hFile = CreateFile(pszPath, dwDesiredAccess, nDisposition, 0);
	if( hFile )
	{
		WriteFile(hFile, strContents.c_str(), strContents.length(), &dwWritten);
		CloseFile(hFile);
	}

	std::string strRet;
	ReadFileContents(MBSFromTCS(pszPath), strRet);
	DeleteFile(pszPath);

	if( dwWritten != strContents.length() )
		return "";

	return strRet;
}

//////////////////////////////////////////////////////////////////////////
TEST(FileTest, WriteFileTest)
{
	std::string strContents = "File writing test!!";

	EXPECT_NE(strContents, WriteFileTestWorker(TEXT("some.test.file"), GENERIC_READ_,		CREATE_ALWAYS_	, strContents));
	EXPECT_NE(strContents, WriteFileTestWorker(TEXT("some.test.file"), GENERIC_READ_,		CREATE_ALWAYS_	, strContents));
	EXPECT_NE(strContents, WriteFileTestWorker(TEXT("some.test.file"), GENERIC_READ_,		CREATE_ALWAYS_	, strContents));

	EXPECT_EQ(strContents, WriteFileTestWorker(TEXT("some.test.file"), GENERIC_WRITE_,		CREATE_ALWAYS_	, strContents));
	EXPECT_EQ(strContents, WriteFileTestWorker(TEXT("some.test.file"), GENERIC_WRITE_,		CREATE_ALWAYS_	, strContents));
	EXPECT_EQ(strContents, WriteFileTestWorker(TEXT("some.test.file"), GENERIC_WRITE_,		CREATE_ALWAYS_	, strContents));

	EXPECT_EQ(strContents, WriteFileTestWorker(TEXT("some.test.file"), FILE_APPEND_DATA_,	CREATE_ALWAYS_	, strContents));
	EXPECT_EQ(strContents, WriteFileTestWorker(TEXT("some.test.file"), FILE_APPEND_DATA_,	CREATE_ALWAYS_	, strContents));
	EXPECT_EQ(strContents, WriteFileTestWorker(TEXT("some.test.file"), FILE_APPEND_DATA_,	CREATE_ALWAYS_	, strContents));
}

//////////////////////////////////////////////////////////////////////////
std::string WriteExistFileTestWorker(LPCTSTR pszPath, DWORD dwDesiredAccess, E_FILE_DISPOSITION nDisposition, std::string strPrefix, std::string strContents)
{
	WriteFileContents(MBSFromTCS(pszPath), strPrefix);

	DWORD dwWritten = 0;
	HANDLE hFile = CreateFile(pszPath, dwDesiredAccess, nDisposition, 0);
	if( hFile )
	{
		WriteFile(hFile, strContents.c_str(), strContents.length(), &dwWritten);
		CloseFile(hFile);
	}

	std::string strRet;
	ReadFileContents(MBSFromTCS(pszPath), strRet);
	DeleteFile(pszPath);
	if( dwWritten != strContents.length() )
		return "";
	return strRet;
}

//////////////////////////////////////////////////////////////////////////
TEST(FileTest, WriteExistFileTest)
{
	std::string strPrefix = "Oops! ";
	std::string strContents = "File writing test!!";

	EXPECT_EQ(strPrefix + strContents, WriteExistFileTestWorker(TEXT("some.test.file"), FILE_APPEND_DATA_,	OPEN_EXISTING_, strPrefix, strContents));
	EXPECT_EQ(strPrefix + strContents, WriteExistFileTestWorker(TEXT("some.test.file"), FILE_APPEND_DATA_,	OPEN_EXISTING_, strPrefix, strContents));
	EXPECT_EQ(strPrefix + strContents, WriteExistFileTestWorker(TEXT("some.test.file"), FILE_APPEND_DATA_,	OPEN_EXISTING_, strPrefix, strContents));
}

//////////////////////////////////////////////////////////////////////////
std::string ReadFileTestWorker(LPCTSTR pszPath, DWORD dwDesiredAccess, E_FILE_DISPOSITION nDisposition, std::string strContents)
{
	WriteFileContents(MBSFromTCS(pszPath), strContents);

	char szBuff[1024] = { 0, };

	HANDLE hFile = CreateFile(pszPath, dwDesiredAccess, nDisposition, 0);
	if( hFile )
	{
		DWORD dwRead = 0;
		ReadFile(hFile, szBuff, 1024, &dwRead);
		CloseFile(hFile);

		szBuff[dwRead] = 0;
	}

	DeleteFile(pszPath);
	return &szBuff[3];
}

//////////////////////////////////////////////////////////////////////////
TEST(FileTest, ReadFileTest)
{
	std::string strContents = "File reading test!!";

	EXPECT_EQ(strContents, ReadFileTestWorker(TEXT("some.test.file"), GENERIC_READ_,	OPEN_EXISTING_, strContents));
	EXPECT_EQ(strContents, ReadFileTestWorker(TEXT("some.test.file"), GENERIC_READ_,	OPEN_EXISTING_, strContents));
	EXPECT_EQ(strContents, ReadFileTestWorker(TEXT("some.test.file"), GENERIC_READ_,	OPEN_EXISTING_, strContents));

	EXPECT_NE(strContents, ReadFileTestWorker(TEXT("some.test.file"), GENERIC_WRITE_,	OPEN_EXISTING_, strContents));
	EXPECT_NE(strContents, ReadFileTestWorker(TEXT("some.test.file"), GENERIC_WRITE_,	OPEN_EXISTING_, strContents));
	EXPECT_NE(strContents, ReadFileTestWorker(TEXT("some.test.file"), GENERIC_WRITE_,	OPEN_EXISTING_, strContents));

	EXPECT_NE(strContents, ReadFileTestWorker(TEXT("some.test.file"), FILE_APPEND_DATA_,	OPEN_EXISTING_, strContents));
	EXPECT_NE(strContents, ReadFileTestWorker(TEXT("some.test.file"), FILE_APPEND_DATA_,	OPEN_EXISTING_, strContents));
	EXPECT_NE(strContents, ReadFileTestWorker(TEXT("some.test.file"), FILE_APPEND_DATA_,	OPEN_EXISTING_, strContents));
}
