#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, GetSystemDirectoryTest)
{
	std::tstring strSystemDirectory = GetSystemDirectory();
	std::tstring strTempPath = GetTempPath();

	EXPECT_FALSE(strSystemDirectory.empty());
	EXPECT_FALSE(strTempPath.empty());

	tprintf(TEXT("%s\n"), strSystemDirectory.c_str());
	tprintf(TEXT("%s\n"), strTempPath.c_str());
}

//////////////////////////////////////////////////////////////////////////
static inline void ListFiles(std::tstring strPath)
{
	ST_FILE_FINDDATA stFindData;
	HANDLE hFind = FindFirstFile(strPath.c_str(), &stFindData);
	if( NULL == hFind )
		return;

	do
	{
		ST_SYSTEMTIME stCreateTime = SystemTimeFrom(stFindData.uCreationTime);
		tprintf(TEXT("%c-%c-%c\t%lu\t%s\t%04u-%02u-%02u %02u:%02u:%02u\n")
			, stFindData.bIsDirectory? 'o' : 'x'
			, stFindData.bIsFile? 'o' : 'x'
			, stFindData.bIsLink? 'o' : 'x'
			, stFindData.uFileSize
			, stFindData.strFileName.c_str()
			, stCreateTime.wYear, stCreateTime.wMonth, stCreateTime.wDay
			, stCreateTime.wHour, stCreateTime.wMinute, stCreateTime.wSecond
			);
	}	while(FindNextFile(hFind, &stFindData));

	FindClose(hFind);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, FileSystemListingTest)
{
	ListFiles(TEXT("./*"));
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateSymbolicLink_FileTest)
{
	std::tstring strNewFile = TEXT("somefile.txt");
	EXPECT_EQ(EC_SUCCESS, WriteFileContents(strNewFile, TEXT("test")));
	EXPECT_TRUE(IsFileExist(strNewFile.c_str()));

	std::tstring strNewLink = strNewFile + TEXT(".lnk");
	EXPECT_TRUE(CreateSymbolicLink(strNewLink.c_str(), strNewFile.c_str()));
	EXPECT_TRUE(IsFileExist(strNewLink.c_str()));

	DeleteFile(strNewFile.c_str());
	DeleteFile(strNewLink.c_str());
}

#ifdef _MSC_VER		// linux/unix system does not allow link to directory
//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CreateSymbolicLink_DirectoryTest)
{
	std::tstring strNewDirectory = GetCurrentDirectory() + TEXT("/new_dir");
	EXPECT_TRUE(CreateDirectory(strNewDirectory.c_str()));
	EXPECT_TRUE(IsDirectory(strNewDirectory.c_str()));

	std::tstring strNewLink = strNewDirectory + TEXT(".lnk");
	EXPECT_TRUE(CreateSymbolicLink(strNewLink.c_str(), strNewDirectory.c_str()));
	Log_Info("CreateSymbolicLink, %d", GetLastError());
	EXPECT_TRUE(IsFileExist(strNewLink.c_str()));

	std::tstring strNewFile = TEXT("somefile.txt");
	std::tstring strNewFile_Orginal = strNewDirectory + TEXT("/") + strNewFile;
	std::tstring strNewFile_Link = strNewLink + TEXT("/") + strNewFile;

	EXPECT_EQ(EC_SUCCESS, WriteFileContents(strNewFile_Orginal, TEXT("test")));

	EXPECT_TRUE(IsFileExist(strNewFile_Orginal.c_str()));
	EXPECT_TRUE(IsFileExist(strNewFile_Link.c_str()));

	DeleteFile(strNewFile_Orginal.c_str());
	RemoveDirectory(strNewLink.c_str());
	RemoveDirectory(strNewDirectory.c_str());
}
#endif

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, FindFirstFileTest)
{
	std::tstring strTempDirectory = GetCurrentDirectory() + TEXT("/temp_dir");
	EXPECT_TRUE(CreateDirectory(strTempDirectory.c_str()));

	std::tstring strSomeFile = strTempDirectory + TEXT("/") + TEXT("somefile.txt");
	EXPECT_EQ(EC_SUCCESS, WriteFileContents(strSomeFile, TEXT("test")));

	ST_FILE_FINDDATA stFileData;
	HANDLE hFind = FindFirstFile((strTempDirectory + TEXT("/*")).c_str(), &stFileData);
	EXPECT_NE((HANDLE)NULL, hFind);
	if( hFind )
	{
		bool bFound = false;
		do 
		{
			if( stFileData.bIsDirectory )
				continue;

			if( stFileData.strFileName == strSomeFile )
				bFound = true;

			EXPECT_NE(0UL, stFileData.uFileSize			);
			EXPECT_NE(0UL, stFileData.uCreationTime	);
			EXPECT_NE(0UL, stFileData.uLastAccessTime	);
			EXPECT_NE(0UL, stFileData.uLastWriteTime	);
		}	while(FindNextFile(hFind, &stFileData));

		FindClose(hFind);
	}
	
	RemoveDirectoryRecursively(strTempDirectory.c_str());
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, GetCurrentFileNameTest)
{
	std::tstring strCurrentFile = GetFileName();
	EXPECT_FALSE(strCurrentFile.empty());

	std::tstring strCurrentFileWithoutExt = ExtractFileNameWithoutExt(strCurrentFile);
#ifdef UNICODE
	EXPECT_EQ(TEXT("TestW"), Right(strCurrentFileWithoutExt, 5));
#else
	EXPECT_EQ(TEXT("TestA"), Right(strCurrentFileWithoutExt, 5));
#endif
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, DirectoryCreationDeletionTest)
{
	std::tstring strCurrentPath = GetCurrentDirectory();
	EXPECT_FALSE(strCurrentPath.empty());

	std::tstring strTestPath = strCurrentPath + TEXT("/test/");
	EXPECT_TRUE(CreateDirectory(strTestPath.c_str()));

	EXPECT_TRUE(SetCurrentDirectory(strTestPath.c_str()));

	std::tstring strChangedDirectory = GetCurrentDirectory();
	EXPECT_NE(strCurrentPath, strChangedDirectory);

	SetCurrentDirectory(strCurrentPath.c_str());
	EXPECT_TRUE(RemoveDirectory(strTestPath.c_str()));
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, PathFileExistsTest)
{
	std::tstring strFilename = TEXT("test.txt");
	DeleteFile(strFilename.c_str());

	ASSERT_FALSE(PathFileExists(strFilename.c_str()));

	{
		FILE* pFile = fopenT(strFilename.c_str(), TEXT("w"));
		ASSERT_NE((FILE*)NULL, pFile);
		fputs("test", pFile);
		fclose(pFile);
	}

	ASSERT_TRUE(PathFileExists(strFilename.c_str()));
	DeleteFile(strFilename.c_str());
	ASSERT_FALSE(PathFileExists(strFilename.c_str()));
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, IsFileExistTest)
{
	std::tstring strFilename = TEXT("test.txt");
	DeleteFile(strFilename.c_str());

	ASSERT_FALSE(IsFileExist(strFilename.c_str()));

	{
		FILE* pFile = fopenT(strFilename.c_str(), TEXT("w"));
		fputs("test", pFile);
		fclose(pFile);
	}

	ASSERT_TRUE(IsFileExist(strFilename.c_str()));
	DeleteFile(strFilename.c_str());
	ASSERT_FALSE(IsFileExist(strFilename.c_str()));
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, MoveFileTest)
{
	std::tstring strFilename = TEXT("test.txt");
	std::tstring strFilename2 = TEXT("test2.txt");
	DeleteFile(strFilename.c_str());
	DeleteFile(strFilename2.c_str());

	{
		FILE* pFile = fopenT(strFilename.c_str(), TEXT("w"));
		fputs("test", pFile);
		fclose(pFile);
	}

	ASSERT_TRUE(IsFileExist(strFilename.c_str()));
	ASSERT_FALSE(IsFileExist(strFilename2.c_str()));
	MoveFile(strFilename.c_str(), strFilename2.c_str());
	ASSERT_FALSE(IsFileExist(strFilename.c_str()));
	ASSERT_TRUE(IsFileExist(strFilename2.c_str()));
	DeleteFile(strFilename2.c_str());
}

//////////////////////////////////////////////////////////////////////////
std::string __test_internal_ReadFileContents(std::tstring strFilename)
{
	FILE* pFile = fopenT(strFilename.c_str(), TEXT("rb"));
	if( NULL == pFile )
		return "";

	const int nBuffLen = 512;
	char szBuff[nBuffLen+1];

	std::string strRet;
	while(true)
	{
		int nRead = (int)fread(szBuff, 1, nBuffLen, pFile);
		if( nRead < 0 )
			break;

		szBuff[nRead] = 0;

		strRet += szBuff;
		if( nRead < nBuffLen )
			break;
	}
	fclose(pFile);

	return strRet;
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CopyFileTest_Normal)
{
	std::tstring strFilename = TEXT("test.txt");
	std::tstring strFilename2 = TEXT("test2.txt");
	DeleteFile(strFilename.c_str());
	DeleteFile(strFilename2.c_str());

	{
		FILE* pFile = fopenT(strFilename.c_str(), TEXT("w"));
		do
		{
			fputs("test", pFile);
		}	while(rand() % 100);
		fclose(pFile);
	}

	ASSERT_TRUE(IsFileExist(strFilename.c_str()));
	ASSERT_FALSE(IsFileExist(strFilename2.c_str()));
	EXPECT_TRUE(CopyFile(strFilename.c_str(), strFilename2.c_str(), TRUE));
	ASSERT_TRUE(IsFileExist(strFilename.c_str()));
	ASSERT_TRUE(IsFileExist(strFilename2.c_str()));

	std::string strFile1Contents = __test_internal_ReadFileContents(strFilename);
	std::string strFile2Contents = __test_internal_ReadFileContents(strFilename2);
	EXPECT_FALSE(strFile1Contents.empty());
	EXPECT_EQ(strFile1Contents, strFile2Contents);

	DeleteFile(strFilename.c_str());
	DeleteFile(strFilename2.c_str());
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CopyFileTest_NonExistFile)
{
	std::tstring strFilename = TEXT("test.txt");
	std::tstring strFilename2 = TEXT("test2.txt");
	DeleteFile(strFilename.c_str());
	DeleteFile(strFilename2.c_str());

	ASSERT_FALSE(IsFileExist(strFilename.c_str()));
	ASSERT_FALSE(IsFileExist(strFilename2.c_str()));
	EXPECT_FALSE(CopyFile(strFilename.c_str(), strFilename2.c_str(), TRUE));
	ASSERT_FALSE(IsFileExist(strFilename.c_str()));
	ASSERT_FALSE(IsFileExist(strFilename2.c_str()));
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CopyFileTest_SameFile)
{
	std::tstring strFilename = TEXT("test.txt");
	DeleteFile(strFilename.c_str());

	{
		FILE* pFile = fopenT(strFilename.c_str(), TEXT("w"));
		do
		{
			fputs("test", pFile);
		}	while(rand() % 100);
		fclose(pFile);
	}

	ASSERT_TRUE(IsFileExist(strFilename.c_str()));
	EXPECT_FALSE(CopyFile(strFilename.c_str(), strFilename.c_str(), TRUE));
	ASSERT_TRUE(IsFileExist(strFilename.c_str()));
	DeleteFile(strFilename.c_str());
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CopyFileTest_AlreadyExistFile1)
{
	std::tstring strFilename = TEXT("test.txt");
	std::tstring strFilename2 = TEXT("test2.txt");
	DeleteFile(strFilename.c_str());
	DeleteFile(strFilename2.c_str());

	{
		FILE* pFile = fopenT(strFilename.c_str(), TEXT("w"));
		do
		{
			fputs("test", pFile);
		}	while(rand() % 100);
		fclose(pFile);
	}
	{
		FILE* pFile = fopenT(strFilename2.c_str(), TEXT("w"));
		do
		{
			fputs("test123132123", pFile);
		}	while(rand() % 100);
		fclose(pFile);
	}

	ASSERT_TRUE(IsFileExist(strFilename.c_str()));
	ASSERT_TRUE(IsFileExist(strFilename2.c_str()));
	EXPECT_FALSE(CopyFile(strFilename.c_str(), strFilename2.c_str(), TRUE));
	ASSERT_TRUE(IsFileExist(strFilename.c_str()));
	ASSERT_TRUE(IsFileExist(strFilename2.c_str()));

	std::string strFile1Contents = __test_internal_ReadFileContents(strFilename);
	std::string strFile2Contents = __test_internal_ReadFileContents(strFilename2);
	EXPECT_FALSE(strFile1Contents.empty());
	EXPECT_NE(strFile1Contents, strFile2Contents);

	DeleteFile(strFilename.c_str());
	DeleteFile(strFilename2.c_str());
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, CopyFileTest_AlreadyExistFile2)
{
	std::tstring strFilename = TEXT("test.txt");
	std::tstring strFilename2 = TEXT("test2.txt");
	DeleteFile(strFilename.c_str());
	DeleteFile(strFilename2.c_str());

	{
		FILE* pFile = fopenT(strFilename.c_str(), TEXT("w"));
		do
		{
			fputs("test", pFile);
		}	while(rand() % 100);
		fclose(pFile);
	}
	{
		FILE* pFile = fopenT(strFilename2.c_str(), TEXT("w"));
		do
		{
			fputs("test", pFile);
		}	while(rand() % 100);
		fclose(pFile);
	}

	ASSERT_TRUE(IsFileExist(strFilename.c_str()));
	ASSERT_TRUE(IsFileExist(strFilename2.c_str()));
	EXPECT_TRUE(CopyFile(strFilename.c_str(), strFilename2.c_str(), FALSE));
	ASSERT_TRUE(IsFileExist(strFilename.c_str()));
	ASSERT_TRUE(IsFileExist(strFilename2.c_str()));

	std::string strFile1Contents = __test_internal_ReadFileContents(strFilename);
	std::string strFile2Contents = __test_internal_ReadFileContents(strFilename2);
	EXPECT_FALSE(strFile1Contents.empty());
	EXPECT_EQ(strFile1Contents, strFile2Contents);

	DeleteFile(strFilename.c_str());
	DeleteFile(strFilename2.c_str());
}
