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

//////////////////////////////////////////////////////////////////////////
void HandleType_HMACTest_Worker(E_HASH_TYPE nHashType, std::string strKey, std::string strMessage, size_t tTokenSize, std::vector<BYTE>& outKeyStream)
{
	HANDLE hHMac = InitHMAC(nHashType, strKey);

	for (size_t i = 0; i < strMessage.length(); i += tTokenSize)
	{
		const size_t tRemainedSize = strMessage.length() - i;
		const size_t tTrySize = std::min(tRemainedSize, tTokenSize);
		UpdateHMAC(hHMac, (LPCBYTE)strMessage.c_str() + i, tTrySize);
	}

	FinalHMAC(hHMac, outKeyStream);
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, HandleType_HMACTest_MD5)
{
	std::string strKey = "key";
	std::string strMessage = "The quick brown fox jumps over the lazy dog";

	const std::vector<BYTE> ExpectedKeyStream = {
		0x80, 0x07, 0x07, 0x13, 0x46, 0x3e, 0x77, 0x49, 0xb9, 0x0c, 0x2d, 0xc2, 0x49, 0x11, 0xe2, 0x75
	};

	for (size_t i = 1; i < 30; i++)
	{
		std::vector<BYTE> KeyStream;
		HandleType_HMACTest_Worker(HASH_TYPE_MD5, strKey, strMessage, i, KeyStream);
		ASSERT_EQ(KeyStream.size(), ExpectedKeyStream.size());
		EXPECT_EQ(0, memcmp(KeyStream.data(), ExpectedKeyStream.data(), KeyStream.size()));
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, HandleType_HMACTest_SHA1)
{
	std::string strKey = "key";
	std::string strMessage = "The quick brown fox jumps over the lazy dog";

	const std::vector<BYTE> ExpectedKeyStream = {
		0xde, 0x7c, 0x9b, 0x85, 0xb8, 0xb7, 0x8a, 0xa6, 0xbc, 0x8a, 0x7a, 0x36, 0xf7, 0x0a, 0x90, 0x70, 0x1c, 0x9d, 0xb4, 0xd9
	};

	for (size_t i = 1; i < 30; i++)
	{
		std::vector<BYTE> KeyStream;
		HandleType_HMACTest_Worker(HASH_TYPE_SHA1, strKey, strMessage, i, KeyStream);
		ASSERT_EQ(KeyStream.size(), ExpectedKeyStream.size());
		EXPECT_EQ(0, memcmp(KeyStream.data(), ExpectedKeyStream.data(), KeyStream.size()));
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, HandleType_HMACTest_SHA256)
{
	std::string strKey = "key";
	std::string strMessage = "The quick brown fox jumps over the lazy dog";

	const std::vector<BYTE> ExpectedKeyStream = {
		0xf7, 0xbc, 0x83, 0xf4, 0x30, 0x53, 0x84, 0x24, 0xb1, 0x32, 0x98, 0xe6, 0xaa, 0x6f, 0xb1, 0x43, 0xef, 0x4d, 0x59, 0xa1, 0x49, 0x46, 0x17, 0x59, 0x97, 0x47, 0x9d, 0xbc, 0x2d, 0x1a, 0x3c, 0xd8
	};

	for (size_t i = 1; i < 30; i++)
	{
		std::vector<BYTE> KeyStream;
		HandleType_HMACTest_Worker(HASH_TYPE_SHA256, strKey, strMessage, i, KeyStream);
		ASSERT_EQ(KeyStream.size(), ExpectedKeyStream.size());
		EXPECT_EQ(0, memcmp(KeyStream.data(), ExpectedKeyStream.data(), KeyStream.size()));
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, HMACTest_MD5)
{
	std::string strKey = "key";
	std::string strMessage = "The quick brown fox jumps over the lazy dog";

	std::vector<BYTE> KeyStream;
	BuildHMAC(HASH_TYPE_MD5, strKey.c_str(), strMessage.c_str(), KeyStream);

	const std::vector<BYTE> ExpectedKeyStream = {
		0x80, 0x07, 0x07, 0x13, 0x46, 0x3e, 0x77, 0x49, 0xb9, 0x0c, 0x2d, 0xc2, 0x49, 0x11, 0xe2, 0x75
	};
	ASSERT_EQ(KeyStream.size(), ExpectedKeyStream.size());
	EXPECT_EQ(0, memcmp(KeyStream.data(), ExpectedKeyStream.data(), KeyStream.size()));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, HMACTest_SHA1)
{
	std::string strKey = "key";
	std::string strMessage = "The quick brown fox jumps over the lazy dog";

	std::vector<BYTE> KeyStream;
	BuildHMAC(HASH_TYPE_SHA1, strKey.c_str(), strMessage.c_str(), KeyStream);

	const std::vector<BYTE> ExpectedKeyStream = {
		0xde, 0x7c, 0x9b, 0x85, 0xb8, 0xb7, 0x8a, 0xa6, 0xbc, 0x8a, 0x7a, 0x36, 0xf7, 0x0a, 0x90, 0x70, 0x1c, 0x9d, 0xb4, 0xd9
	};
	ASSERT_EQ(KeyStream.size(), ExpectedKeyStream.size());
	EXPECT_EQ(0, memcmp(KeyStream.data(), ExpectedKeyStream.data(), KeyStream.size()));
}

//////////////////////////////////////////////////////////////////////////
TEST(CommonTest, HMACTest_SHA256)
{
	std::string strKey = "key";
	std::string strMessage = "The quick brown fox jumps over the lazy dog";

	std::vector<BYTE> KeyStream;
	BuildHMAC(HASH_TYPE_SHA256, strKey.c_str(), strMessage.c_str(), KeyStream);

	const std::vector<BYTE> ExpectedKeyStream = {
		0xf7, 0xbc, 0x83, 0xf4, 0x30, 0x53, 0x84, 0x24, 0xb1, 0x32, 0x98, 0xe6, 0xaa, 0x6f, 0xb1, 0x43, 0xef, 0x4d, 0x59, 0xa1, 0x49, 0x46, 0x17, 0x59, 0x97, 0x47, 0x9d, 0xbc, 0x2d, 0x1a, 0x3c, 0xd8
	};
	ASSERT_EQ(KeyStream.size(), ExpectedKeyStream.size());
	EXPECT_EQ(0, memcmp(KeyStream.data(), ExpectedKeyStream.data(), KeyStream.size()));
}
