#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
TEST(UnicodeTest, ASCIITruncateTest)
{
	BYTE szSrc[4] = { 0x31, 0xB0, 0xA1, 0 };	// 1GA

	size_t tRead = 0;
	WORD szDest[10]= { 0, };
	EXPECT_EQ(1, ASCII_TO_UTF16((LPCSTR)szSrc, 2, szDest, 9, &tRead));
	EXPECT_STREQ(L"1", (LPCWSTR)szDest);
	EXPECT_EQ(1, tRead);
}

//////////////////////////////////////////////////////////////////////////
TEST(UnicodeTest, ASCIITruncateTest_3byte)
{
	BYTE szSrc[6] = { 0x31, 0xB0, 0xA1, 0xB0, 0xA1, 0 };	// 1GAGA

	size_t tRead = 0;
	WORD szDest[10]= { 0, };
	EXPECT_EQ(2, ASCII_TO_UTF16((LPCSTR)szSrc, 4, szDest, 9, &tRead));

	WORD szExpectedDest[3] = { L'1', 0xAC00, 0 };
	EXPECT_EQ(0, memcmp(szDest, szExpectedDest, 5));
	EXPECT_EQ(3, tRead);
}

//////////////////////////////////////////////////////////////////////////
void ReadBytes(FILE* pFile, std::vector<BYTE>& outBytes)
{
	const int nBuffSize = 1024;
	int nTotalReadSize = 0;
	while(1)
	{
		outBytes.resize(nTotalReadSize + nBuffSize);
		int nCurRead = (int)fread(&outBytes[nTotalReadSize], 1, nBuffSize, pFile);
		if( nCurRead <= 0 )
		{
			outBytes.resize(nTotalReadSize);
			break;
		}

		if( nCurRead < nBuffSize )
		{
			outBytes.resize(nTotalReadSize + nCurRead);
			break;
		}

		nTotalReadSize += nCurRead;
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(UnicodeTest, WCSConvert)
{
	std::vector<BYTE> vecUTF8Bytes;
	{
		FILE* pFile = fopen("../../Build/Test/Unicode/UTF8Sample.txt", "rb");
		ASSERT_NE(pFile, (FILE*)NULL);
		ASSERT_EQ(ReadBOM(pFile), BOM_UTF8);
		ReadBytes(pFile, vecUTF8Bytes);
		fclose(pFile);
		ASSERT_GT(vecUTF8Bytes.size(), 0UL);
	}

	std::vector<BYTE> vecUTF16Bytes;
	{
		FILE* pFile = fopen("../../Build/Test/Unicode/UTF16Sample.txt", "rb");
		ASSERT_NE(pFile, (FILE*)NULL);
		ASSERT_EQ(ReadBOM(pFile), BOM_UTF16);
		ReadBytes(pFile, vecUTF16Bytes);
		fclose(pFile);
		ASSERT_GT(vecUTF16Bytes.size(), 0UL);
	}

	std::vector<BYTE> vecUTF32Bytes;
	{
		FILE* pFile = fopen("../../Build/Test/Unicode/UTF32Sample.txt", "rb");
		ASSERT_NE(pFile, (FILE*)NULL);
		ASSERT_EQ(ReadBOM(pFile), BOM_UTF32);
		ReadBytes(pFile, vecUTF32Bytes);
		fclose(pFile);
		ASSERT_GT(vecUTF32Bytes.size(), 0UL);
	}

	std::vector<BYTE> vecASCIIBytes;
	{
		FILE* pFile = fopen("../../Build/Test/Unicode/ASCIISample.txt", "rb");
		ASSERT_NE(pFile, (FILE*)NULL);
		ASSERT_EQ(ReadBOM(pFile), BOM_UNDEFINED);
		ReadBytes(pFile, vecASCIIBytes);
		fclose(pFile);
		ASSERT_GT(vecASCIIBytes.size(), 0UL);
	}

	{
		std::wstring strWCSFromUTF8  = WCSFromUTF8((const char*)&vecUTF8Bytes[0]);
		std::wstring strWCSFromUTF16 = WCSFromUTF16((const WORD*)&vecUTF16Bytes[0], vecUTF16Bytes.size()/2);
		std::wstring strWCSFromUTF32 = WCSFromUTF32((const DWORD*)&vecUTF32Bytes[0], vecUTF32Bytes.size()/4);
		std::wstring strWCSFromASCII = WCSFromASCII((const char*)&vecASCIIBytes[0]);

		EXPECT_EQ(strWCSFromUTF8,  strWCSFromUTF16);
		EXPECT_EQ(strWCSFromUTF8,  strWCSFromUTF32);
		EXPECT_EQ(strWCSFromUTF8,  strWCSFromASCII);

		EXPECT_EQ(strWCSFromUTF16, strWCSFromUTF8 );
		EXPECT_EQ(strWCSFromUTF16, strWCSFromUTF32);
		EXPECT_EQ(strWCSFromUTF16, strWCSFromASCII);

		EXPECT_EQ(strWCSFromUTF32, strWCSFromUTF8 );
		EXPECT_EQ(strWCSFromUTF32, strWCSFromUTF16);
		EXPECT_EQ(strWCSFromUTF32, strWCSFromASCII);

		EXPECT_EQ(strWCSFromASCII, strWCSFromUTF8 );
		EXPECT_EQ(strWCSFromASCII, strWCSFromUTF16);
		EXPECT_EQ(strWCSFromASCII, strWCSFromUTF32);
	}

	{
		std::string strMBSFromUTF8  = MBSFromUTF8((const char*)&vecUTF8Bytes[0]);
		std::string strMBSFromUTF16 = MBSFromUTF16((const WORD*)&vecUTF16Bytes[0], vecUTF16Bytes.size()/2);
		std::string strMBSFromUTF32 = MBSFromUTF32((const DWORD*)&vecUTF32Bytes[0], vecUTF32Bytes.size()/4);
		std::string strMBSFromASCII = MBSFromASCII((const char*)&vecASCIIBytes[0]);

		EXPECT_EQ(strMBSFromUTF8,  strMBSFromUTF16);
		EXPECT_EQ(strMBSFromUTF8,  strMBSFromUTF32);
		EXPECT_EQ(strMBSFromUTF8,  strMBSFromASCII);

		EXPECT_EQ(strMBSFromUTF16, strMBSFromUTF8 );
		EXPECT_EQ(strMBSFromUTF16, strMBSFromUTF32);
		EXPECT_EQ(strMBSFromUTF16, strMBSFromASCII);

		EXPECT_EQ(strMBSFromUTF32, strMBSFromUTF8 );
		EXPECT_EQ(strMBSFromUTF32, strMBSFromUTF16);
		EXPECT_EQ(strMBSFromUTF32, strMBSFromASCII);

		EXPECT_EQ(strMBSFromASCII, strMBSFromUTF8 );
		EXPECT_EQ(strMBSFromASCII, strMBSFromUTF16);
		EXPECT_EQ(strMBSFromASCII, strMBSFromUTF32);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(UnicodeTest, ASCII_UTF8_TEST)
{
	std::vector<BYTE> vecUTF8Bytes;
	{
		FILE* pFile = fopen("../../Build/Test/Unicode/UTF8Sample.txt", "rb");
		ASSERT_NE(pFile, (FILE*)NULL);
		ASSERT_EQ(ReadBOM(pFile), BOM_UTF8);
		ReadBytes(pFile, vecUTF8Bytes);
		fclose(pFile);
		ASSERT_GT(vecUTF8Bytes.size(), 0UL);
	}

	std::vector<BYTE> vecASCIIBytes;
	{
		FILE* pFile = fopen("../../Build/Test/Unicode/ASCIISample.txt", "rb");
		ASSERT_NE(pFile, (FILE*)NULL);
		ASSERT_EQ(ReadBOM(pFile), BOM_UNDEFINED);
		ReadBytes(pFile, vecASCIIBytes);
		fclose(pFile);
		ASSERT_GT(vecASCIIBytes.size(), 0UL);
	}

	{
		std::string strASCIIFromUTF8 = ASCIIFromUTF8((const char*)&vecUTF8Bytes[0]);
		std::string strUTF8FromASCII = UTF8FromASCII((const char*)&vecASCIIBytes[0]);
		EXPECT_STREQ((const char*)&vecUTF8Bytes[0],  strUTF8FromASCII.c_str());
		EXPECT_STREQ((const char*)&vecASCIIBytes[0],  strASCIIFromUTF8.c_str());
	}
}
