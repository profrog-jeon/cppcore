#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, BuildUniqFileName_NULLTest)
{
	EXPECT_NE(TEXT(""), BuildUniqFileName(TEXT("")));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, ColosseumLanguageData_ReadingTest)
{
	std::tstring strContents;
	ReadFileContents(TEXT("../../Build/Test/Encoder/Colosseum.db"), strContents, "Colosseum.db");
 	EXPECT_EQ(TEXT("Code,ShortMsg_en,ShortMsg_ko,LongMsg_en,LongMsg_ko"), strContents);
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, MakeAbsPathFromModuleDir_AbnormalTest)
{
	MakeAbsPathByModulePath("");
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, StripQuotationTest)
{
	EXPECT_EQ(TEXT(""), StripQuotation(TEXT(""), 1));
	EXPECT_EQ(TEXT("a b c"), StripQuotation(TEXT("a b c"), 10));
	EXPECT_EQ(TEXT("a b c"), StripQuotation(TEXT("\'a b c\'"), 10));

	EXPECT_EQ(TEXT("\'a b c\'"), StripQuotation(TEXT("\'a b c\'"), 5));
	EXPECT_EQ(TEXT(" \'a b c\'"), StripQuotation(TEXT(" \'a b c\'"), 5));

	EXPECT_EQ(TEXT("a b c"), StripQuotation(TEXT("\'a b c\'"), 15));
	EXPECT_EQ(TEXT(" a b c"), StripQuotation(TEXT(" \'a b c\'"), 15));

	EXPECT_EQ(TEXT("a\'b\'c"), StripQuotation(TEXT("\'a\'b\'c\'"), 15));
	EXPECT_EQ(TEXT("a\'bc"), StripQuotation(TEXT("\'a\'bc\'"), 15));
	EXPECT_EQ(TEXT("a\'bc\'"), StripQuotation(TEXT("a\'bc\'"), 15));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, StripQuotationTest2)
{
	EXPECT_EQ(TEXT(""), StripQuotation(TEXT(""), 1));
	EXPECT_EQ(TEXT("a b c"), StripQuotation(TEXT("a b c"), 10));
	EXPECT_EQ(TEXT("a b c"), StripQuotation(TEXT("\"a b c\""), 10));
	
	EXPECT_EQ(TEXT("\"a b c\""), StripQuotation(TEXT("\"a b c\""), 5));
	EXPECT_EQ(TEXT(" \"a b c\""), StripQuotation(TEXT(" \"a b c\""), 5));

	EXPECT_EQ(TEXT("a b c"), StripQuotation(TEXT("\"a b c\""), 15));
	EXPECT_EQ(TEXT(" a b c"), StripQuotation(TEXT(" \"a b c\""), 15));

	EXPECT_EQ(TEXT("a\"b\"c"), StripQuotation(TEXT("\"a\"b\"c\""), 15));
	EXPECT_EQ(TEXT("a\"bc"), StripQuotation(TEXT("\"a\"bc\""), 15));
	EXPECT_EQ(TEXT("a\"bc\""), StripQuotation(TEXT("a\"bc\""), 15));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, WrapQuotationTest)
{
	EXPECT_EQ(TEXT("\"\""), WrapQuotation(TEXT(""), 1));
	EXPECT_EQ(TEXT("\"a b c\""), WrapQuotation(TEXT("a b c"), 15));
	EXPECT_EQ(TEXT("\"a b c\""), WrapQuotation(TEXT("\"a b c\""), 15));

	EXPECT_EQ(TEXT("\"a\"b c\"\""), WrapQuotation(TEXT("a\"b c\""), 15));
	EXPECT_EQ(TEXT(" \"a b c\""), WrapQuotation(TEXT(" a b c"), 15));

	EXPECT_EQ(TEXT("\"a b c\""), WrapQuotation(TEXT("\"a b c\""), 15));
	EXPECT_EQ(TEXT(" \"a b c\""), WrapQuotation(TEXT(" \"a b c\""), 15));

	EXPECT_EQ(TEXT("\"a\"b\"c\""), WrapQuotation(TEXT("\"a\"b\"c\""), 15));
	EXPECT_EQ(TEXT("\"a\"bc\""), WrapQuotation(TEXT("\"a\"bc\""), 15));
	EXPECT_EQ(TEXT("\"a\"bc\"\""), WrapQuotation(TEXT("a\"bc\""), 15));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, MakeFormalPathFromModuleTest)
{
	EXPECT_EQ(TEXT("some/test/path"), MakeFormalPath(TEXT("some///test/path")));
	EXPECT_EQ(TEXT("/some/test/path"), MakeFormalPath(TEXT("/some///test/path")));
	EXPECT_EQ(TEXT("/some/test/path/"), MakeFormalPath(TEXT("/some///test/path/")));
	EXPECT_EQ(TEXT("/some/test/path/"), MakeFormalPath(TEXT("/some/./test/has/../path/")));
	EXPECT_EQ(TEXT("/some/test/path/"), MakeFormalPath(TEXT("/some/test/has/../dup/../path/")));
	EXPECT_EQ(TEXT("/some/test/path/"), MakeFormalPath(TEXT("/some/test/has/dup/../../path/")));
	EXPECT_EQ(TEXT("/some/test/path/"), MakeFormalPath(TEXT("/some/test/./has/./dup/../../path/")));
	EXPECT_EQ(TEXT("/"), MakeFormalPath(TEXT("/some///test/path/../../../../")));
	EXPECT_EQ(TEXT("/"), MakeFormalPath(TEXT("/some/..//test/../path/../")));
	EXPECT_EQ(TEXT("/"), MakeFormalPath(TEXT("/../")));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, MakeAbsolutePathTest)
{
	std::tstring strModulePath = ExtractDirectory(GetFileName());
	std::tstring strTestPath = strModulePath + TEXT("/some/path/");
	MakeFormalPath(strTestPath);

	EXPECT_EQ(strTestPath, MakeAbsolutePath(strModulePath.c_str(), TEXT("./some/path/")));
	EXPECT_EQ(TEXT("/some/path/"), MakeAbsolutePath(strModulePath.c_str(), TEXT("/some/path/")));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, MakeAbsPathFromModuleTest)
{
	std::tstring strTestPath = ExtractDirectory(GetFileName()) + TEXT("/some/path/");
	MakeFormalPath(strTestPath);
	EXPECT_EQ(strTestPath, MakeAbsPathByModulePath(TEXT("./some/path/")));
	EXPECT_EQ(TEXT("/some/path/"), MakeAbsPathByModulePath(TEXT("/some/path/")));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, MakeAbsPathFromExePathTest)
{
	std::tstring strTestPath = GetCurrentDirectory() + TEXT("/some/path/");
	MakeFormalPath(strTestPath);
	EXPECT_EQ(strTestPath, MakeAbsPathByCurPath(TEXT("./some/path/")));
	EXPECT_EQ(TEXT("/some/path/"), MakeAbsPathByCurPath(TEXT("/some/path/")));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, MakeAbsPathFromModulePathTest_ChildDir)
{
	std::tstring strModuleDirectory = ExtractDirectory(GetFileName());
	std::tstring strTestPath = ExtractDirectory(strModuleDirectory) + TEXT("/some/path/");
	MakeFormalPath(strTestPath);
	EXPECT_EQ(strTestPath, MakeAbsPathByModulePath(TEXT("../some/path/")));

	std::tstring strErrorLog = Format(TEXT("ModulePath:%s\n ModuleDir:%s\n TestDir:%s\n"), GetFileName().c_str(), strModuleDirectory.c_str(), strTestPath.c_str());
	SCOPED_TRACE(strErrorLog.c_str());
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, MakeAbsPathFromExePathTest_ChildDir)
{
	std::tstring strModuleDirectory = GetCurrentDirectory();
	std::tstring strDirectory = ExtractDirectory(strModuleDirectory);
	std::tstring strTestPath = strDirectory + TEXT("/some/path/");
	MakeFormalPath(strTestPath);
	EXPECT_EQ(strTestPath, MakeAbsPathByCurPath(TEXT("../some/path/")));
}

//////////////////////////////////////////////////////////////////////////
bool TouchFile(std::tstring strFilePath)
{
	FILE* pFile = fopenT(strFilePath.c_str(), TEXT("wb"));
	if( NULL == pFile )
		return false;

	fwrite(&pFile, sizeof(pFile), 1, pFile);
	fclose(pFile);
	return true;
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, GrepHierarchyFile)
{
	std::tstring strCurrentPath = GetCurrentDirectory();
	std::tstring strTestPath = strCurrentPath + TEXT("/test");
	std::tstring strTestSubPath = strTestPath + TEXT("/a/b/c");
	CreateDirectoryRecursively(strTestSubPath);

	std::tstring strTestFile = strTestSubPath + TEXT("/sample.slcli");
	TouchFile(strTestFile);
	ASSERT_TRUE(PathFileExists(strTestFile.c_str()));

	std::vector<std::tstring> vecFiles;
	GrepFilesRecursively(strTestPath, TEXT("*.slcli"), vecFiles);
	ASSERT_FALSE(vecFiles.empty());

	EXPECT_EQ(strTestFile, vecFiles[0]);
	DeleteFile(strTestFile.c_str());
	RemoveDirectoryRecursively(strTestPath);
}

//////////////////////////////////////////////////////////////////////////
bool __internal_InvalidPathTraverseCallback(void* pContext, const ST_FILETRAVERSE_INFO& stInfo)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, EnumFileTest_InvalidPath)
{
	std::tstring strSearchDir = GetCurrentDirectory() + TEXT("/../aasdf");
	core::EnumFile(strSearchDir.c_str(), TEXT("*.*"), __internal_InvalidPathTraverseCallback, NULL);
}

//////////////////////////////////////////////////////////////////////////
bool __internal_TraverseDirectoryTestCallback(void* pContext, const ST_FILETRAVERSE_INFO& stInfo)
{
	EXPECT_TRUE(stInfo.bDirectory);

	std::vector<std::tstring>& vecDirectory = *(std::vector<std::tstring>*)pContext;
	vecDirectory.push_back(stInfo.pszFilePath);
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool __internal_EnumFileRecursiveTestCallback(void* pContext, const ST_FILETRAVERSE_INFO& stInfo)
{
	EXPECT_FALSE(stInfo.bDirectory);

	std::vector<std::tstring>& vecFileDirectory = *(std::vector<std::tstring>*)pContext;
	vecFileDirectory.push_back(stInfo.pszFilePath);
	return true;
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, IsDirectory_NonExistDirTest)
{
	EXPECT_FALSE(IsDirectory(TEXT("non-exist-path")));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, IsDirectory_FileTest)
{
	EXPECT_FALSE(IsDirectory(GetFileName().c_str()));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, IsDirectory_ExistDirTest)
{
	LPCTSTR pszTempDir = TEXT("TempDirForIsDirectoryTest");
	EXPECT_TRUE(CreateDirectory(pszTempDir));
	EXPECT_TRUE(IsDirectory(pszTempDir));
	RemoveDirectory(pszTempDir);
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, TraverseDirectoryTest)
{
	std::tstring strCurrentDir = GetCurrentDirectory();
	std::tstring strTempDirRoot = strCurrentDir + TEXT("/temp");
	std::tstring strTempDir = strTempDirRoot + TEXT("/1/2/3/4");
	ASSERT_TRUE(CreateDirectoryRecursively(strTempDir));

	std::tstring strTempFile = strTempDir + TEXT("/test.dat");
	TouchFile(strTempFile);

	std::vector<std::tstring> vecDirectory;
	core::TraverseDirectoryRecursively(strTempDirRoot.c_str(), TEXT("*"), __internal_TraverseDirectoryTestCallback, &vecDirectory);
	EXPECT_EQ(5U, vecDirectory.size());

	std::vector<std::tstring> vecFile;
	core::EnumFileRecursively(strTempDirRoot.c_str(), TEXT("*"), __internal_EnumFileRecursiveTestCallback, &vecFile);
	EXPECT_EQ(1U, vecFile.size());

	DeleteFile(strTempFile.c_str());
	RemoveDirectoryRecursively(strTempDirRoot);
}


//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, DirectoryCreationDeletionTest)
{
	std::tstring strCurrentPath = GetCurrentDirectory();
	EXPECT_FALSE(strCurrentPath.empty());

	std::tstring strTestPath = strCurrentPath + TEXT("/test/");
	EXPECT_TRUE(CreateDirectory(strTestPath.c_str()));

	std::tstring strTestPath2 = strTestPath + TEXT("/1/2");
	EXPECT_TRUE(CreateDirectoryRecursively(strTestPath2));

	strTestPath2 = strTestPath + TEXT("/a/b/c/d/e/f");
	EXPECT_TRUE(CreateDirectoryRecursively(strTestPath2));

	EXPECT_TRUE(SetCurrentDirectory(strTestPath2.c_str()));

	std::tstring strChangedDirectory = GetCurrentDirectory();
	EXPECT_NE(strCurrentPath ,strChangedDirectory);

	SetCurrentDirectory(strCurrentPath.c_str());
	EXPECT_TRUE(RemoveDirectoryRecursively(strTestPath));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, DirectoryCreationDeletionWithFileTest)
{
	EXPECT_TRUE(CreateDirectoryRecursively(TEXT("test_dir/a/b/c/d/e/f")));
	{
		FILE *fp = fopenT(TEXT("test_dir/a/b/c/d/e/f/test.txt"), TEXT("w"));
		fputs("test", fp);
		fclose(fp);
	}
	{
		FILE *fp = fopenT(TEXT("test_dir/a/b/c/test.txt"), TEXT("w"));
		fputs("test", fp);
		fclose(fp);
	}
	{
		FILE* pFile = fopenT(TEXT("test_dir/a/b/test1.txt"), TEXT("w"));
		fputs("test", pFile);
		fclose(pFile);
	}
	{
		FILE* pFile = fopenT(TEXT("test_dir/a/b/test2.txt"), TEXT("w"));
		fputs("test", pFile);
		fclose(pFile);
	}

	EXPECT_FALSE(RemoveDirectory(TEXT("test_dir/a/b")));
	EXPECT_TRUE(RemoveDirectoryRecursively(TEXT("test_dir/a/b/c/d/e/f")));
	EXPECT_TRUE(RemoveDirectoryRecursively(TEXT("test_dir")));
	EXPECT_FALSE(IsFileExist(TEXT("test_dir")));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, GrepFileTest)
{
	std::tstring strRootPath = TEXT(".");
	std::vector<std::tstring> vecFiles;
	ASSERT_EQ(EC_SUCCESS, GrepFiles(strRootPath, TEXT("*"), vecFiles));
	EXPECT_NE(0LU, vecFiles.size());

	size_t i;
	for(i=0; i<vecFiles.size(); i++)
	{
		std::tstring strDirectory = ExtractDirectory(vecFiles[i]);
		EXPECT_EQ(strRootPath, strDirectory);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, GrepFileRecursiveTest)
{
	std::tstring strRootPath = TEXT("../Test");
	std::vector<std::tstring> vecFiles;
	ASSERT_EQ(EC_SUCCESS, GrepFilesRecursively(strRootPath, TEXT("*"), vecFiles));
	EXPECT_NE(0LU, vecFiles.size());

	std::tstring strExternalPath;
	size_t i;
	for(i=0; i<vecFiles.size(); i++)
	{
		std::tstring strDirectory = ExtractDirectory(vecFiles[i]);
		if( strRootPath == strDirectory )
			continue;

		strExternalPath = strDirectory;
		break;
	}

	EXPECT_NE(TEXT(""), strExternalPath);
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, GrepDirectoryTest)
{
	std::tstring strRootPath = TEXT("../Test");
	std::vector<std::tstring> vecDirectory;
	ASSERT_EQ(EC_SUCCESS, GrepDirectories(strRootPath, TEXT("*"), vecDirectory));
	EXPECT_NE(0LU, vecDirectory.size());

	size_t i;
	for(i=0; i<vecDirectory.size(); i++)
	{
		std::tstring strDirectory = ExtractDirectory(vecDirectory[i]);
		EXPECT_EQ(strRootPath, strDirectory);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, GrepDirectoryTest_Pattern)
{
	std::tstring strRootPath = TEXT("../Test");
	std::vector<std::tstring> vecDirectory;
	ASSERT_EQ(EC_SUCCESS, GrepDirectories(strRootPath, TEXT("E*"), vecDirectory));
	EXPECT_NE(0LU, vecDirectory.size());

	size_t i;
	for(i=0; i<vecDirectory.size(); i++)
	{
		std::tstring strName = ExtractFileName(vecDirectory[i]);
		EXPECT_EQ(TEXT("E"), strName.substr(0, 1));
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, GrepDirectoryRecursiveTest)
{
	std::tstring strRootPath = TEXT("../Test");
	std::vector<std::tstring> vecFiles;
	ASSERT_EQ(EC_SUCCESS, GrepDirectoriesRecursively(strRootPath, TEXT("*"), vecFiles));
	EXPECT_NE(0LU, vecFiles.size());

	std::tstring strExternalPath;
	size_t i;
	for(i=0; i<vecFiles.size(); i++)
	{
		std::tstring strDirectory = ExtractDirectory(vecFiles[i]);
		if( strRootPath == strDirectory )
			continue;

		strExternalPath = strDirectory;
		break;
	}

	EXPECT_NE(TEXT(""), strExternalPath);
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, GrepDirectoryRecursiveTest_Pattern)
{
	std::tstring strRootPath = TEXT("../Test");
	std::vector<std::tstring> vecFiles;
	ASSERT_EQ(EC_SUCCESS, GrepDirectoriesRecursively(strRootPath, TEXT("E*"), vecFiles));
	EXPECT_NE(0LU, vecFiles.size());

	size_t i;
	for(i=0; i<vecFiles.size(); i++)
	{
		if( strRootPath == vecFiles[i] )
			continue;

		std::tstring strName = ExtractFileName(vecFiles[i]);
		EXPECT_EQ(TEXT("E"), strName.substr(0, 1));
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, CopyDirectoryTest)
{
	CreateDirectory(TEXT("temp"));
	CreateDirectory(TEXT("temp/a"));
	CreateDirectory(TEXT("temp/a/a1"));
	CreateDirectory(TEXT("temp/a/a2"));
	CreateDirectory(TEXT("temp/a/a2/aa1"));
	TouchFile(TEXT("temp/a/1.txt"));
	TouchFile(TEXT("temp/a/a1/1.txt"));
	TouchFile(TEXT("temp/a/a2/1.txt"));
	TouchFile(TEXT("temp/a/a2/aa1/1.txt"));
	CreateDirectory(TEXT("temp/b"));
	CreateDirectory(TEXT("temp/c"));

	std::vector<std::tstring> vecFiles;
	GrepFilesRecursively(TEXT("temp"), TEXT("*"), vecFiles);
	EXPECT_FALSE(vecFiles.empty());

	CopyDirectory(TEXT("temp"), TEXT("temp2"));

	std::vector<std::tstring> vecFiles2;
	GrepFilesRecursively(TEXT("temp2"), TEXT("*"), vecFiles2);
	EXPECT_FALSE(vecFiles2.empty());

	RemoveDirectoryRecursively(TEXT("temp"));
	RemoveDirectoryRecursively(TEXT("temp2"));

	ASSERT_EQ(vecFiles.size(), vecFiles2.size());

	size_t i;
	for(i=0; i<vecFiles.size(); i++)
	{
		Replace(vecFiles2[i], TEXT("temp2/"), TEXT("temp/"));
		EXPECT_EQ(vecFiles[i], vecFiles2[i]);
	}
}