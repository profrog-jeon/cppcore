#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
inline bool HashStringTestHelper(E_HASH_TYPE dwType, LPCBYTE pData, size_t tDataSize, std::string strFeedback)
{
	HANDLE hHash = InitHash(dwType);
	UpdateHash(hHash, pData, tDataSize);
    std::string strHash = FinalHash(hHash);
	std::string strResult = MakeLower(strHash);

	if( strResult != MakeLower(strFeedback) )
	{
		std::cout << GetHashTypeString(dwType) << " failure!!, result:" << strResult.c_str() << " is not matched " << strFeedback.c_str() << std::endl;
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
TEST(EncoderTest, HashTest_StringFinal)
{
	// MD5		: c3fcd3d76192e4007dfb496cca67e13b
	// SHA1		: 32d10c7b8cf96570ca04ce37f2a19d84240d3a89
	// SHA256	: 71c480df93d6ae2f1efad1447c66c9525e316218cf51fc8d9ed832f2daf18b73
	// CRC64	: a1aa8b21f979f059
	// HAS160	: 596185c9ab6703d0d0dbb98702bc0f5729cd1d3c
	char	szDummy[]	= "abcdefghijklmnopqrstuvwxyz";
	size_t	tDummyLen	= strlen(szDummy);

	EXPECT_TRUE(HashStringTestHelper(HASH_TYPE_MD5,		(LPCBYTE)szDummy, 			tDummyLen, 	"c3fcd3d76192e4007dfb496cca67e13b"));
	EXPECT_TRUE(HashStringTestHelper(HASH_TYPE_SHA1,		(LPCBYTE)szDummy, 			tDummyLen, 	"32d10c7b8cf96570ca04ce37f2a19d84240d3a89"));
	EXPECT_TRUE(HashStringTestHelper(HASH_TYPE_SHA256,	(LPCBYTE)szDummy, 			tDummyLen, 	"71c480df93d6ae2f1efad1447c66c9525e316218cf51fc8d9ed832f2daf18b73"));
	EXPECT_TRUE(HashStringTestHelper(HASH_TYPE_HAS160,	(LPCBYTE)szDummy, 			tDummyLen, 	"596185c9ab6703d0d0dbb98702bc0f5729cd1d3c"));
	EXPECT_TRUE(HashStringTestHelper(HASH_TYPE_HAS160,	(LPCBYTE)"",				0,			"307964ef34151d37c8047adec7ab50f4ff89762d"));
	EXPECT_TRUE(HashStringTestHelper(HASH_TYPE_HAS160,	(LPCBYTE)"a",				1,			"4872bcbc4cd0f0a9dc7c2f7045e5b43b6c830db8"));
	EXPECT_TRUE(HashStringTestHelper(HASH_TYPE_HAS160,	(LPCBYTE)"abc",				3,			"975e810488cf2a3d49838478124afce4b1c78804"));
	EXPECT_TRUE(HashStringTestHelper(HASH_TYPE_HAS160,	(LPCBYTE)"message digest",	14,			"2338dbc8638d31225f73086246ba529f96710bc6"));
}

//////////////////////////////////////////////////////////////////////////
inline bool HashVectorTestHelper(E_HASH_TYPE dwType, LPCBYTE pData, size_t tDataSize, std::string strExpect)
{
	std::vector<BYTE> vecOutput;
	{
		HANDLE hHash = InitHash(dwType);
		UpdateHash(hHash, pData, tDataSize);
		FinalHash(hHash, vecOutput);
	}
	
	std::vector<BYTE> vecExpect;
	vecExpect.resize(strExpect.length() / 2);
	HexFromString(&vecExpect[0], vecExpect.size(), strExpect);

	if( vecOutput.size() != vecExpect.size() )
	{
		std::cout << GetHashTypeString(dwType) << " failure!!, result:" << StringFromHex(&vecOutput[0], vecOutput.size()).c_str() << " is not matched " << strExpect.c_str() << std::endl;
		return false;
	}

	if( memcmp(&vecOutput[0], &vecExpect[0], vecOutput.size()) )
	{
		std::cout << GetHashTypeString(dwType) << " failure!!, result:" << StringFromHex(&vecOutput[0], vecOutput.size()).c_str() << " is not matched " << strExpect.c_str() << std::endl;
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
TEST(EncoderTest, HashTest_VectorFinal)
{
	// MD5		: c3fcd3d76192e4007dfb496cca67e13b
	// SHA1		: 32d10c7b8cf96570ca04ce37f2a19d84240d3a89
	// SHA256	: 71c480df93d6ae2f1efad1447c66c9525e316218cf51fc8d9ed832f2daf18b73
	// CRC64	: a1aa8b21f979f059
	// HAS160	: 596185c9ab6703d0d0dbb98702bc0f5729cd1d3c
	char	szDummy[]	= "abcdefghijklmnopqrstuvwxyz";
	size_t	tDummyLen	= strlen(szDummy);

	EXPECT_TRUE(HashVectorTestHelper(HASH_TYPE_MD5,		(LPCBYTE)szDummy, 			tDummyLen, 	"c3fcd3d76192e4007dfb496cca67e13b"));
	EXPECT_TRUE(HashVectorTestHelper(HASH_TYPE_SHA1,	(LPCBYTE)szDummy, 			tDummyLen, 	"32d10c7b8cf96570ca04ce37f2a19d84240d3a89"));
	EXPECT_TRUE(HashVectorTestHelper(HASH_TYPE_SHA256,	(LPCBYTE)szDummy, 			tDummyLen, 	"71c480df93d6ae2f1efad1447c66c9525e316218cf51fc8d9ed832f2daf18b73"));
	EXPECT_TRUE(HashVectorTestHelper(HASH_TYPE_HAS160,	(LPCBYTE)szDummy, 			tDummyLen, 	"596185c9ab6703d0d0dbb98702bc0f5729cd1d3c"));
	EXPECT_TRUE(HashVectorTestHelper(HASH_TYPE_HAS160,	(LPCBYTE)"",				0,			"307964ef34151d37c8047adec7ab50f4ff89762d"));
	EXPECT_TRUE(HashVectorTestHelper(HASH_TYPE_HAS160,	(LPCBYTE)"a",				1,			"4872bcbc4cd0f0a9dc7c2f7045e5b43b6c830db8"));
	EXPECT_TRUE(HashVectorTestHelper(HASH_TYPE_HAS160,	(LPCBYTE)"abc",				3,			"975e810488cf2a3d49838478124afce4b1c78804"));
	EXPECT_TRUE(HashVectorTestHelper(HASH_TYPE_HAS160,	(LPCBYTE)"message digest",	14,			"2338dbc8638d31225f73086246ba529f96710bc6"));
}
