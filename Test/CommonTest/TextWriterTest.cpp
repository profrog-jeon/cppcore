#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, WriteFileContentsTest_Plain)
{
	std::tstring strContextA;
	ASSERT_EQ(EC_SUCCESS, ReadFileContents(TEXT("../../Build/Test/Unicode/ASCIISample.txt"), strContextA));
	strContextA = Trim(strContextA);
	ASSERT_FALSE(strContextA.empty());

	std::tstring strTempFileName = TEXT("temporate.txt");
	EXPECT_EQ(EC_SUCCESS, WriteFileContents(strTempFileName, strContextA));

	std::tstring strContextB;
	EXPECT_EQ(EC_SUCCESS, ReadFileContents(strTempFileName, strContextB));

	EXPECT_EQ(Trim(strContextA), Trim(strContextB));
	DeleteFile(strTempFileName.c_str());
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, WriteFileContentsTest_Binary)
{
	std::vector<BYTE> vecData;
	{
		BYTE btData[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
		vecData.resize(sizeof(btData));
		memcpy(&vecData[0], btData, sizeof(btData));
	}

	std::tstring strFileName = TEXT("temp.bin");
	DeleteFile(strFileName.c_str());
	ASSERT_EQ(EC_SUCCESS, WriteFileContents(strFileName, vecData));

	std::vector<BYTE> vecRestored;
	EXPECT_EQ(EC_SUCCESS, ReadFileContents(strFileName, vecRestored));
	ASSERT_EQ(vecData.size(), vecRestored.size());
	EXPECT_EQ(0, memcmp(&vecData[0], &vecRestored[0], vecData.size()));
	DeleteFile(strFileName.c_str());
}

//////////////////////////////////////////////////////////////////////////
inline void WriteFileContentsTest_Worker(E_SYM_CIPHER_TYPE nType, E_SYM_CIPHER_MODE nMode)
{
	std::tstring strContextA;
	ASSERT_EQ(EC_SUCCESS, ReadFileContents(TEXT("../../Build/Test/Unicode/ASCIISample.txt"), strContextA));
	strContextA = Trim(strContextA);
	ASSERT_FALSE(strContextA.empty());

	std::string strTestKey = "test";
	std::tstring strTempFileName = TEXT("temporate.txt");
	EXPECT_EQ(EC_SUCCESS, WriteFileContents(strTempFileName, strContextA, nType, nMode, strTestKey));

	std::tstring strContextB;
	EXPECT_EQ(EC_SUCCESS, ReadFileContents(strTempFileName.c_str(), strContextB, strTestKey));

	EXPECT_EQ(Trim(strContextA), Trim(strContextB));
	DeleteFile(strTempFileName.c_str());
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, WriteFileContentsTest_AES128_ECB	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_AES128, SYM_CIPHER_MODE_ECB	);	}
TEST(CommonTest, WriteFileContentsTest_AES128_CBC	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_AES128, SYM_CIPHER_MODE_CBC	);	}
TEST(CommonTest, WriteFileContentsTest_AES128_PCBC	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_AES128, SYM_CIPHER_MODE_PCBC	);	}
TEST(CommonTest, WriteFileContentsTest_AES128_CFB	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_AES128, SYM_CIPHER_MODE_CFB	);	}
TEST(CommonTest, WriteFileContentsTest_AES128_OFB	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_AES128, SYM_CIPHER_MODE_OFB	);	}
TEST(CommonTest, WriteFileContentsTest_AES128_CTR	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_AES128, SYM_CIPHER_MODE_CTR	);	}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, WriteFileContentsTest_AES192_ECB	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_AES192, SYM_CIPHER_MODE_ECB	);	}
TEST(CommonTest, WriteFileContentsTest_AES192_CBC	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_AES192, SYM_CIPHER_MODE_CBC	);	}
TEST(CommonTest, WriteFileContentsTest_AES192_PCBC	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_AES192, SYM_CIPHER_MODE_PCBC	);	}
TEST(CommonTest, WriteFileContentsTest_AES192_CFB	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_AES192, SYM_CIPHER_MODE_CFB	);	}
TEST(CommonTest, WriteFileContentsTest_AES192_OFB	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_AES192, SYM_CIPHER_MODE_OFB	);	}
TEST(CommonTest, WriteFileContentsTest_AES192_CTR	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_AES192, SYM_CIPHER_MODE_CTR	);	}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, WriteFileContentsTest_AES256_ECB	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_AES256, SYM_CIPHER_MODE_ECB	);	}
TEST(CommonTest, WriteFileContentsTest_AES256_CBC	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_AES256, SYM_CIPHER_MODE_CBC	);	}
TEST(CommonTest, WriteFileContentsTest_AES256_PCBC	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_AES256, SYM_CIPHER_MODE_PCBC	);	}
TEST(CommonTest, WriteFileContentsTest_AES256_CFB	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_AES256, SYM_CIPHER_MODE_CFB	);	}
TEST(CommonTest, WriteFileContentsTest_AES256_OFB	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_AES256, SYM_CIPHER_MODE_OFB	);	}
TEST(CommonTest, WriteFileContentsTest_AES256_CTR	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_AES256, SYM_CIPHER_MODE_CTR	);	}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, WriteFileContentsTest_SEED128_ECB	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_SEED128, SYM_CIPHER_MODE_ECB	);	}
TEST(CommonTest, WriteFileContentsTest_SEED128_CBC	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_SEED128, SYM_CIPHER_MODE_CBC	);	}
TEST(CommonTest, WriteFileContentsTest_SEED128_PCBC	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_SEED128, SYM_CIPHER_MODE_PCBC	);	}
TEST(CommonTest, WriteFileContentsTest_SEED128_CFB	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_SEED128, SYM_CIPHER_MODE_CFB	);	}
TEST(CommonTest, WriteFileContentsTest_SEED128_OFB	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_SEED128, SYM_CIPHER_MODE_OFB	);	}
TEST(CommonTest, WriteFileContentsTest_SEED128_CTR	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_SEED128, SYM_CIPHER_MODE_CTR	);	}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, WriteFileContentsTest_SEED256_ECB	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_SEED256, SYM_CIPHER_MODE_ECB	);	}
TEST(CommonTest, WriteFileContentsTest_SEED256_CBC	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_SEED256, SYM_CIPHER_MODE_CBC	);	}
TEST(CommonTest, WriteFileContentsTest_SEED256_PCBC	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_SEED256, SYM_CIPHER_MODE_PCBC	);	}
TEST(CommonTest, WriteFileContentsTest_SEED256_CFB	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_SEED256, SYM_CIPHER_MODE_CFB	);	}
TEST(CommonTest, WriteFileContentsTest_SEED256_OFB	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_SEED256, SYM_CIPHER_MODE_OFB	);	}
TEST(CommonTest, WriteFileContentsTest_SEED256_CTR	)	{	WriteFileContentsTest_Worker(SYM_CIPHER_TYPE_SEED256, SYM_CIPHER_MODE_CTR	);	}

//////////////////////////////////////////////////////////////////////////
bool IsSameFile(std::tstring strFileA, std::tstring strFileB)
{
	FILE* pFileA = fopenT(strFileA.c_str(), TEXT("rb"));
	FILE* pFileB = fopenT(strFileB.c_str(), TEXT("rb"));

	try
	{
		if( NULL == pFileA )
			throw exception_format(TEXT("fopenT %s has failed."), strFileA.c_str());

		if( NULL == pFileB )
			throw exception_format(TEXT("fopenT %s has failed."), strFileB.c_str());

		while(1)
		{
			BYTE btFileA, btFileB;
			bool bFileAReadSucceed = (1 != fread(&btFileA, sizeof(btFileA), 1, pFileA));
			bool bFileBReadSucceed = (1 != fread(&btFileB, sizeof(btFileB), 1, pFileB));

			if( bFileAReadSucceed != bFileBReadSucceed )
				throw exception_format(TEXT("File size is not matched."), strFileB.c_str());

			if( !bFileAReadSucceed && !bFileBReadSucceed )
				break;

			if( btFileA != btFileB )
				throw exception_format("Data is not matched, %u with %u", btFileA, btFileB);
		}
		fclose(pFileA);
		fclose(pFileB);
	}
	catch (std::exception& e)
	{
		if( pFileA )
			fclose(pFileA);
		if( pFileB )
			fclose(pFileB);
		Log_Error("%s", e.what());
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, TextWriterTest_ASCII)
{
	std::tstring strOriginalFile = TEXT("../../Build/Test/Unicode/ASCIISample.txt");
	std::tstring strTempFileName = TEXT("../../Build/Test/Unicode/ASCIItemp.txt");

	std::tstring strContext;
	EXPECT_EQ(EC_SUCCESS, ReadFileContents(strOriginalFile, strContext));

	{
		CTextFileWriter writer(strTempFileName.c_str(), BOM_UTF8);
		EXPECT_LT(0, writer.WriteLine(strContext));
	}

	EXPECT_TRUE(IsSameFile(strOriginalFile, strTempFileName));
	DeleteFile(strTempFileName.c_str());
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, TextWriterTest_UTF8)
{
	std::tstring strOriginalFile = TEXT("../../Build/Test/Unicode/UTF8Sample.txt");
	std::tstring strTempFileName = TEXT("../../Build/Test/Unicode/UTF8temp.txt");

	std::tstring strContext;
	EXPECT_EQ(EC_SUCCESS, ReadFileContents(strOriginalFile, strContext));

	{
		CTextFileWriter writer(strTempFileName.c_str(), BOM_UTF8);
		EXPECT_LT(0, writer.WriteLine(strContext));
	}

	EXPECT_TRUE(IsSameFile(strOriginalFile, strTempFileName));
	DeleteFile(strTempFileName.c_str());
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, TextWriterTest_UTF16)
{
	std::tstring strOriginalFile = TEXT("../../Build/Test/Unicode/UTF16Sample.txt");
	std::tstring strTempFileName = TEXT("../../Build/Test/Unicode/UTF16temp.txt");

	std::tstring strContext;
	EXPECT_EQ(EC_SUCCESS, ReadFileContents(strOriginalFile, strContext));

	{
		CTextFileWriter writer(strTempFileName.c_str(), BOM_UTF8);
		EXPECT_LT(0, writer.WriteLine(strContext));
	}

	EXPECT_TRUE(IsSameFile(strOriginalFile, strTempFileName));
	DeleteFile(strTempFileName.c_str());
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, TextWriterTest_UTF32)
{
	std::tstring strOriginalFile = TEXT("../../Build/Test/Unicode/UTF32Sample.txt");
	std::tstring strTempFileName = TEXT("../../Build/Test/Unicode/UTF32temp.txt");

	std::tstring strContext;
	EXPECT_EQ(EC_SUCCESS, ReadFileContents(strOriginalFile, strContext));

	{
		CTextFileWriter writer(strTempFileName.c_str(), BOM_UTF8);
		EXPECT_LT(0, writer.WriteLine(strContext));
	}

	EXPECT_TRUE(IsSameFile(strOriginalFile, strTempFileName));
	DeleteFile(strTempFileName.c_str());
}
