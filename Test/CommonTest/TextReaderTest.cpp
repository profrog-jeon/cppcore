#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, TextReaderTest_UTF8_16)
{
	std::tstring strContextA;
	{
		CTextFileReader TextFileReader("../../Build/Test/Unicode/UTF8Sample.txt");
		EXPECT_EQ(EC_SUCCESS, TextFileReader.ReadLine(strContextA));
		EXPECT_FALSE(strContextA.empty());
	}

	std::tstring strContextB;
	{
		CTextFileReader TextFileReader("../../Build/Test/Unicode/UTF16Sample.txt");
		EXPECT_EQ(EC_SUCCESS, TextFileReader.ReadLine(strContextB));
		EXPECT_FALSE(strContextB.empty());
	}

	EXPECT_EQ(Trim(strContextA), Trim(strContextB));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, TextReaderTest_UTF8_32)
{
	std::tstring strContextA;
	{
		CTextFileReader TextFileReader("../../Build/Test/Unicode/UTF8Sample.txt");
		EXPECT_EQ(EC_SUCCESS, TextFileReader.ReadLine(strContextA));
		EXPECT_FALSE(strContextA.empty());
	}

	std::tstring strContextB;
	{
		CTextFileReader TextFileReader("../../Build/Test/Unicode/UTF32Sample.txt");
		EXPECT_EQ(EC_SUCCESS, TextFileReader.ReadLine(strContextB));
		EXPECT_FALSE(strContextB.empty());
	}

	EXPECT_EQ(Trim(strContextA), Trim(strContextB));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, TextReaderTest_UTF16_32)
{
	std::tstring strContextA;
	{
		CTextFileReader TextFileReader("../../Build/Test/Unicode/UTF16Sample.txt");
		EXPECT_EQ(EC_SUCCESS, TextFileReader.ReadLine(strContextA));
		EXPECT_FALSE(strContextA.empty());
	}

	std::tstring strContextB;
	{
		CTextFileReader TextFileReader("../../Build/Test/Unicode/UTF32Sample.txt");
		EXPECT_EQ(EC_SUCCESS, TextFileReader.ReadLine(strContextB));
		EXPECT_FALSE(strContextB.empty());
	}

	EXPECT_EQ(Trim(strContextA), Trim(strContextB));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, TextReaderTest_ASCII_UTF8)
{
	std::tstring strContextA;
	{
		CTextFileReader TextFileReader("../../Build/Test/Unicode/ASCIISample.txt");
		EXPECT_EQ(EC_SUCCESS, TextFileReader.ReadLine(strContextA));
		EXPECT_FALSE(strContextA.empty());
	}

	std::tstring strContextB;
	{
		CTextFileReader TextFileReader("../../Build/Test/Unicode/UTF8Sample.txt");
		EXPECT_EQ(EC_SUCCESS, TextFileReader.ReadLine(strContextB));
		EXPECT_FALSE(strContextB.empty());
	}

	EXPECT_EQ(Trim(strContextA), Trim(strContextB));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, TextReaderTest_ASCII_UTF16)
{
	std::tstring strContextA;
	{
		CTextFileReader TextFileReader("../../Build/Test/Unicode/ASCIISample.txt");
		EXPECT_EQ(EC_SUCCESS, TextFileReader.ReadLine(strContextA));
		EXPECT_FALSE(strContextA.empty());
	}

	std::tstring strContextB;
	{
		CTextFileReader TextFileReader("../../Build/Test/Unicode/UTF16Sample.txt");
		EXPECT_EQ(EC_SUCCESS, TextFileReader.ReadLine(strContextB));
		EXPECT_FALSE(strContextB.empty());
	}

	EXPECT_EQ(Trim(strContextA), Trim(strContextB));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, TextReaderTest_ASCII_UTF32)
{
	std::tstring strContextA;
	{
		CTextFileReader TextFileReader("../../Build/Test/Unicode/ASCIISample.txt");
		EXPECT_EQ(EC_SUCCESS, TextFileReader.ReadLine(strContextA));
		EXPECT_FALSE(strContextA.empty());
	}

	std::tstring strContextB;
	{
		CTextFileReader TextFileReader("../../Build/Test/Unicode/UTF32Sample.txt");
		EXPECT_EQ(EC_SUCCESS, TextFileReader.ReadLine(strContextB));
		EXPECT_FALSE(strContextB.empty());
	}

	EXPECT_EQ(Trim(strContextA), Trim(strContextB));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, TextReaderTest_UTF16_BE)
{
	std::tstring strContextA;
	{
		CTextFileReader TextFileReader("../../Build/Test/Unicode/UTF16BESample.txt");
		EXPECT_EQ(EC_SUCCESS, TextFileReader.ReadLine(strContextA));
		EXPECT_FALSE(strContextA.empty());
	}

	std::tstring strContextB;
	{
		CTextFileReader TextFileReader("../../Build/Test/Unicode/UTF16Sample.txt");
		EXPECT_EQ(EC_SUCCESS, TextFileReader.ReadLine(strContextB));
		EXPECT_FALSE(strContextB.empty());
	}

	EXPECT_EQ(Trim(strContextA), Trim(strContextB));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, TextReaderTest_UTF32_BE)
{
	std::tstring strContextA;
	{
		CTextFileReader TextFileReader("../../Build/Test/Unicode/UTF32BESample.txt");
		EXPECT_EQ(EC_SUCCESS, TextFileReader.ReadLine(strContextA));
		EXPECT_FALSE(strContextA.empty());
	}

	std::tstring strContextB;
	{
		CTextFileReader TextFileReader("../../Build/Test/Unicode/UTF32Sample.txt");
		EXPECT_EQ(EC_SUCCESS, TextFileReader.ReadLine(strContextB));
		EXPECT_FALSE(strContextB.empty());
	}

	EXPECT_EQ(Trim(strContextA), Trim(strContextB));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, ReadFileContentsATest)
{
	std::string strContextA;
	EXPECT_EQ(EC_SUCCESS, ReadFileContentsA(TEXT("../../Build/Test/Unicode/ASCIISample.txt"), strContextA));
	strContextA = Trim(strContextA);
	ASSERT_FALSE(strContextA.empty());

	std::tstring strTempFileName = TEXT("temporate.txt");
	{
		CTextFileWriter writer(strTempFileName, BOM_UTF32_BE);
		writer.WriteLine(strContextA);
	}

	std::string strContextB;
	EXPECT_EQ(EC_SUCCESS, ReadFileContentsA(strTempFileName, strContextB));

	EXPECT_EQ(Trim(strContextA), Trim(strContextB));
	DeleteFile(strTempFileName.c_str());
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, ReadFileContentsWTest)
{
	std::wstring strContextA;
	EXPECT_EQ(EC_SUCCESS, ReadFileContentsW(TEXT("../../Build/Test/Unicode/ASCIISample.txt"), strContextA));
	strContextA = Trim(strContextA);
	ASSERT_FALSE(strContextA.empty());

	std::tstring strTempFileName = TEXT("temporate.txt");
	{
		CTextFileWriter writer(strTempFileName, BOM_UTF32_BE);
		writer.WriteLine(strContextA);
	}

	std::wstring strContextB;
	EXPECT_EQ(EC_SUCCESS, ReadFileContentsW(strTempFileName, strContextB));

	EXPECT_EQ(Trim(strContextA), Trim(strContextB));
	DeleteFile(strTempFileName.c_str());
}


//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, ReadFileContentsTest_ASCII_UTF32BE)
{
	std::tstring strContextA;
	EXPECT_EQ(EC_SUCCESS, ReadFileContents(TEXT("../../Build/Test/Unicode/ASCIISample.txt"), strContextA));
	strContextA = Trim(strContextA);
	ASSERT_FALSE(strContextA.empty());

	std::tstring strTempFileName = TEXT("temporate.txt");
	{
		CTextFileWriter writer(strTempFileName, BOM_UTF32_BE);
		writer.WriteLine(strContextA);
	}

	std::tstring strContextB;
	EXPECT_EQ(EC_SUCCESS, ReadFileContents(strTempFileName, strContextB));

	EXPECT_EQ(Trim(strContextA), Trim(strContextB));
	DeleteFile(strTempFileName.c_str());
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, ReadFileContentsTest_NonExistFile)
{
	std::tstring strContextA;
	EXPECT_NE(EC_SUCCESS, ReadFileContents(TEXT("NonExistFile"), strContextA));
}
