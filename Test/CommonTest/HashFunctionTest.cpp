#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, BuildStringHashTest)
{
	EXPECT_EQ(TEXT("900150983CD24FB0D6963F7D28E17F72"), BuildStringHash(HASH_TYPE_MD5, TEXT("abc")));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, BuildStringHashTest_Data)
{
	BYTE btData[] = {	0x90, 0x01, 0x50, 0x98, 0x3C, 0xD2, 0x4F, 0xB0, 0xD6, 0x96, 0x3F, 0x7D, 0x28, 0xE1, 0x7F, 0x72	};
	std::vector<BYTE> vecResult;
	BuildStringHash(HASH_TYPE_MD5, TEXT("abc"), vecResult);
	EXPECT_EQ(0lu, memcmp(btData, &vecResult[0], vecResult.size()));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, BuildFileHashTest)
{
	std::vector<std::tstring> vecFiles;
	GrepFiles(TEXT("../../Build/Test/Encoder"), TEXT("*.md5"), vecFiles);

	size_t i;
	for(i=0; i<vecFiles.size(); i++)
	{
		std::tstring strHash = MakeLower(BuildFileHash(HASH_TYPE_MD5, vecFiles[i]));
		EXPECT_EQ(ExtractFileNameWithoutExt(vecFiles[i]), strHash);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, BuildFileHashTest_NonExistedFile)
{
	EXPECT_EQ(TEXT(""), BuildFileHash(HASH_TYPE_MD5, TEXT("non-existed-file")));
}
