#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
TEST(UtilityString, ExtractDirectoryTest)
{
	EXPECT_EQ(TEXT("c:\\a/b/c/d/e\\f"), ExtractDirectory(TEXT("c:\\a/b/c/d/e\\f/test.txt")));
	EXPECT_EQ(TEXT(""), ExtractDirectory(TEXT("/test.txt")));
	EXPECT_EQ(TEXT(""), ExtractDirectory(TEXT("test.txt")));
}

//////////////////////////////////////////////////////////////////////////
TEST(UtilityString, ExtractFileNameTest)
{
	EXPECT_EQ(TEXT("test.txt"), ExtractFileName(TEXT("c:\\a/b/c/d/e\\f/test.txt")));
	EXPECT_EQ(TEXT("test.txt"), ExtractFileName(TEXT("/test.txt")));
	EXPECT_EQ(TEXT("test.txt"), ExtractFileName(TEXT("test.txt")));
}

//////////////////////////////////////////////////////////////////////////
TEST(UtilityString, ExtractFileExtTest)
{
	EXPECT_EQ(TEXT("txt"),	ExtractFileExt(TEXT("c:\\a/b/c/d/e\\f/test.abc.xyz.txt")));
	EXPECT_EQ(TEXT("txt"),	ExtractFileExt(TEXT("c:\\a/b/c/d/e\\f/test.abc.txt")));
	EXPECT_EQ(TEXT("txt"),	ExtractFileExt(TEXT("c:\\a/b/c/d/e\\f/test.txt")));
	EXPECT_EQ(TEXT("txt"),	ExtractFileExt(TEXT("c:\\a/b/c/d/e\\f.abc/test.txt")));
	EXPECT_EQ(TEXT("txt"),	ExtractFileExt(TEXT("c:\\a/b/c/d/e.abc\\f/test.txt")));
	EXPECT_EQ(TEXT(""),		ExtractFileExt(TEXT("c:\\a/b/c/d/e.abc\\f/test")));
	EXPECT_EQ(TEXT(""),		ExtractFileExt(TEXT("c:\\a/b/c/d/e\\f.abc/test")));
	EXPECT_EQ(TEXT(""),		ExtractFileExt(TEXT("c:\\a/b/c/d/e\\f/test")));
	EXPECT_EQ(TEXT(""), 	ExtractFileExt(TEXT("c:\\a/b/c/d/e.abc\\")));
	EXPECT_EQ(TEXT(""), 	ExtractFileExt(TEXT("c:\\a/b/c/d/")));
	EXPECT_EQ(TEXT(""), 	ExtractFileExt(TEXT("c:\\a/b/c")));
	EXPECT_EQ(TEXT(""), 	ExtractFileExt(TEXT("c:\\a/b")));
	EXPECT_EQ(TEXT(""), 	ExtractFileExt(TEXT("c:\\")));
	EXPECT_EQ(TEXT(""), 	ExtractFileExt(TEXT("c:")));
	EXPECT_EQ(TEXT(""), 	ExtractFileExt(TEXT("/")));
	EXPECT_EQ(TEXT(""), 	ExtractFileExt(TEXT("\\")));
}

//////////////////////////////////////////////////////////////////////////
TEST(UtilityString, ExtractFileNameWithoutExtTest)
{
	EXPECT_EQ(TEXT("test.abc.xyz"),	ExtractFileNameWithoutExt(TEXT("c:\\a/b/c/d/e\\f/test.abc.xyz.txt")));
	EXPECT_EQ(TEXT("test.abc"),	ExtractFileNameWithoutExt(TEXT("c:\\a/b/c/d/e\\f/test.abc.txt")));
	EXPECT_EQ(TEXT("test"),	ExtractFileNameWithoutExt(TEXT("c:\\a/b/c/d/e\\f/test.txt")));
	EXPECT_EQ(TEXT("test"),	ExtractFileNameWithoutExt(TEXT("c:\\a/b/c/d/e\\f.abc/test.txt")));
	EXPECT_EQ(TEXT("test"),	ExtractFileNameWithoutExt(TEXT("c:\\a/b/c/d/e.abc\\f/test.txt")));
	EXPECT_EQ(TEXT("test"),		ExtractFileNameWithoutExt(TEXT("c:\\a/b/c/d/e.abc\\f/test")));
	EXPECT_EQ(TEXT("test"),		ExtractFileNameWithoutExt(TEXT("c:\\a/b/c/d/e\\f.abc/test")));
	EXPECT_EQ(TEXT("test"),		ExtractFileNameWithoutExt(TEXT("c:\\a/b/c/d/e\\f/test")));
	EXPECT_EQ(TEXT(""), 	ExtractFileNameWithoutExt(TEXT("c:\\a/b/c/d/e.abc\\")));
	EXPECT_EQ(TEXT(""), 	ExtractFileNameWithoutExt(TEXT("c:\\a/b/c/d/")));
	EXPECT_EQ(TEXT("c"), 	ExtractFileNameWithoutExt(TEXT("c:\\a/b/c")));
	EXPECT_EQ(TEXT("b"), 	ExtractFileNameWithoutExt(TEXT("c:\\a/b")));
	EXPECT_EQ(TEXT(""), 	ExtractFileNameWithoutExt(TEXT("c:\\")));
	EXPECT_EQ(TEXT(""), 	ExtractFileNameWithoutExt(TEXT("c:")));
	EXPECT_EQ(TEXT(""), 	ExtractFileNameWithoutExt(TEXT("/")));
	EXPECT_EQ(TEXT(""), 	ExtractFileNameWithoutExt(TEXT("\\")));
}

//////////////////////////////////////////////////////////////////////////
TEST(UtilityString, MakeHexDumpStrTest)
{
	std::vector<BYTE> vecHexDump;
	vecHexDump.resize(256);
	for (size_t i = 0; i < 256; i++)
	{
		if (g_bDisplayableAscii[i])
			vecHexDump[i] = BYTE(i);
		else
			vecHexDump[i] = '.';
	}

	LPCTSTR pszTestFile = TEXT("../../Build/Test/Ascii/HexDump.txt");
	//WriteFileContents(pszTestFile, vecHexDump.data(), vecHexDump.size());

	std::vector<BYTE> vecExpectData;
	ReadFileContents(pszTestFile, vecExpectData);
	ASSERT_EQ(vecHexDump.size(), 256);
	ASSERT_EQ(vecExpectData.size(), 256);
	EXPECT_EQ(0, memcmp(vecHexDump.data(), vecExpectData.data(), 256));
}