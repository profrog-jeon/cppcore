#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
std::string GenerateRandomString(size_t tLength)
{
	std::string strRet;
	strRet.resize(tLength);
	char* pDest = (char*)strRet.c_str();

	size_t i;
	for(i=0; i<tLength; i++)
		pDest[i] = '0' + rand() % ('z' - '0');
	return strRet;
}

//////////////////////////////////////////////////////////////////////////
std::string QueryConstantString(size_t tLength)
{
	std::string strRet;
	strRet.resize(tLength);
	char* pDest = (char*)strRet.c_str();

	size_t i;
	for(i=0; i<tLength; i++)
	{
		static size_t tIndex = 0;
		pDest[i] = '0' + (tIndex++) % ('z' - '0');
	}
	return strRet;
}

//////////////////////////////////////////////////////////////////////////
void STRING_SYM_CIPHER_TEST(E_SYM_CIPHER_TYPE nType, E_SYM_CIPHER_MODE nMode)
{
	size_t i;
	for(i=1; i<31; i++)
	{
		std::string strPlain = QueryConstantString(i);
		std::tstring strEncrypted;
		{
			ST_SYM_CIPHER_KEY stKey;
			EXPECT_EQ(EC_SUCCESS, GenerateCipherKey(nType, nMode, "test", stKey));

			strEncrypted = EncodeString(stKey, strPlain); 
		}

		std::string strRestored;
		{
			ST_SYM_CIPHER_KEY stKey;
			EXPECT_EQ(EC_SUCCESS, GenerateCipherKey(nType, nMode, "test", stKey));

			strRestored = DecodeString(stKey, strEncrypted); 
		}
		EXPECT_NE(strPlain, UTF8FromTCS(strEncrypted));
		EXPECT_EQ(strPlain, strRestored);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(EncoderTest, StringCipherTest_SEED128_ECB		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED128, SYM_CIPHER_MODE_ECB	);	}
TEST(EncoderTest, StringCipherTest_SEED128_CBC		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED128, SYM_CIPHER_MODE_CBC	);	}
TEST(EncoderTest, StringCipherTest_SEED128_PCBC		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED128, SYM_CIPHER_MODE_PCBC	);	}
TEST(EncoderTest, StringCipherTest_SEED128_CFB		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED128, SYM_CIPHER_MODE_CFB	);	}
TEST(EncoderTest, StringCipherTest_SEED128_OFB		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED128, SYM_CIPHER_MODE_OFB	);	}
TEST(EncoderTest, StringCipherTest_SEED128_CTR		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED128, SYM_CIPHER_MODE_CTR	);	}

//////////////////////////////////////////////////////////////////////////
TEST(EncoderTest, StringCipherTest_SEED256_ECB		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED256, SYM_CIPHER_MODE_ECB	);	}
TEST(EncoderTest, StringCipherTest_SEED256_CBC		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED256, SYM_CIPHER_MODE_CBC	);	}
TEST(EncoderTest, StringCipherTest_SEED256_PCBC		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED256, SYM_CIPHER_MODE_PCBC	);	}
TEST(EncoderTest, StringCipherTest_SEED256_CFB		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED256, SYM_CIPHER_MODE_CFB	);	}
TEST(EncoderTest, StringCipherTest_SEED256_OFB		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED256, SYM_CIPHER_MODE_OFB	);	}
TEST(EncoderTest, StringCipherTest_SEED256_CTR		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED256, SYM_CIPHER_MODE_CTR	);	}

//////////////////////////////////////////////////////////////////////////
TEST(EncoderTest, StringCipherTest_AES128_ECB		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES128, SYM_CIPHER_MODE_ECB	);	}
TEST(EncoderTest, StringCipherTest_AES128_CBC		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES128, SYM_CIPHER_MODE_CBC	);	}
TEST(EncoderTest, StringCipherTest_AES128_PCBC		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES128, SYM_CIPHER_MODE_PCBC);	}
TEST(EncoderTest, StringCipherTest_AES128_CFB		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES128, SYM_CIPHER_MODE_CFB	);	}
TEST(EncoderTest, StringCipherTest_AES128_OFB		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES128, SYM_CIPHER_MODE_OFB	);	}
TEST(EncoderTest, StringCipherTest_AES128_CTR		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES128, SYM_CIPHER_MODE_CTR	);	}

//////////////////////////////////////////////////////////////////////////
TEST(EncoderTest, StringCipherTest_AES192_ECB		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES192, SYM_CIPHER_MODE_ECB	);	}
TEST(EncoderTest, StringCipherTest_AES192_CBC		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES192, SYM_CIPHER_MODE_CBC	);	}
TEST(EncoderTest, StringCipherTest_AES192_PCBC		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES192, SYM_CIPHER_MODE_PCBC);	}
TEST(EncoderTest, StringCipherTest_AES192_CFB		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES192, SYM_CIPHER_MODE_CFB	);	}
TEST(EncoderTest, StringCipherTest_AES192_OFB		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES192, SYM_CIPHER_MODE_OFB	);	}
TEST(EncoderTest, StringCipherTest_AES192_CTR		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES192, SYM_CIPHER_MODE_CTR	);	}

//////////////////////////////////////////////////////////////////////////
TEST(EncoderTest, StringCipherTest_AES256_ECB		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES256, SYM_CIPHER_MODE_ECB	);	}
TEST(EncoderTest, StringCipherTest_AES256_CBC		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES256, SYM_CIPHER_MODE_CBC	);	}
TEST(EncoderTest, StringCipherTest_AES256_PCBC		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES256, SYM_CIPHER_MODE_PCBC);	}
TEST(EncoderTest, StringCipherTest_AES256_CFB		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES256, SYM_CIPHER_MODE_CFB	);	}
TEST(EncoderTest, StringCipherTest_AES256_OFB		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES256, SYM_CIPHER_MODE_OFB	);	}
TEST(EncoderTest, StringCipherTest_AES256_CTR		)	{	STRING_SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES256, SYM_CIPHER_MODE_CTR	);	}

//////////////////////////////////////////////////////////////////////////
inline void DumpToFile(std::string strFileName, const std::vector<BYTE>& vecData)
{
	if( vecData.empty() )
		return;

	FILE* pFile = fopen(strFileName.c_str(), "a+b");
	if( NULL == pFile )
		return;

	fwrite(&vecData[0], vecData.size(), 1, pFile);
	fclose(pFile);
}

//////////////////////////////////////////////////////////////////////////
inline void ReadFromFile(std::string strFileName, std::vector<BYTE>& vecData, size_t tLength)
{
	static std::string g_LastFileName;
	static int nReadPos = 0;

	if( g_LastFileName != strFileName )
	{
		g_LastFileName = strFileName;
		nReadPos = 0;
	}

	FILE* pFile = fopen(strFileName.c_str(), "r+b");
	if( NULL == pFile )
		return;

	::fseek(pFile, nReadPos, SEEK_SET);
	nReadPos += tLength;

	vecData.resize(tLength);
	fread(&vecData[0], 1, tLength, pFile);
	fclose(pFile);
}

//////////////////////////////////////////////////////////////////////////
void SYM_CIPHER_TEST(E_SYM_CIPHER_TYPE nType, E_SYM_CIPHER_MODE nMode)
{
	const size_t tIVSize = 1024;
	BYTE btIV[tIVSize] = { 0, };
	BYTE btKey[tIVSize] = { 0, };

	size_t i;
	for(i=0; i<tIVSize; i++)
	{
		btIV[i] = rand() & 0xFF;
		btKey[i] = rand() & 0xFF;
	}

	{
		ST_SYM_CIPHER_KEY stCipherKey;
		ASSERT_EQ(EC_SUCCESS, GenerateCipherKey(nType, nMode, btKey, btIV, stCipherKey));

		size_t ctTestStringLength[] =
		{
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,
			28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,
			51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,
			75,76,77,78,79,80,90,100,500,1000,2000,4001,6006,8009,10000,40001//,1024*1024*10
		};

		const size_t ctTestStringLengthCount = sizeof(ctTestStringLength) / sizeof(ctTestStringLength[0]);

		for(i=0; i<ctTestStringLengthCount; i++)
		{
			size_t tLength = ctTestStringLength[i];
			size_t tEncodeSize = ((tLength / stCipherKey.dwBlockSize) + 1) * stCipherKey.dwBlockSize;

			std::string strOriginalText = QueryConstantString(tLength);
			strOriginalText.resize(tEncodeSize);
			
			std::vector<BYTE> vecEncodedText;
			vecEncodedText.resize(tEncodeSize);
			{
				HANDLE hCipher = InitSymCipher(stCipherKey, CIPHER_METHOD_ENCRYPT);
				ASSERT_NE((HANDLE)NULL, hCipher);

				size_t j;
				for(j=0; j<tEncodeSize; j+=stCipherKey.dwBlockSize)
				{
					LPCBYTE pSrcData = (LPCBYTE)strOriginalText.c_str() + j;
					EXPECT_EQ(EC_SUCCESS, UpdateSymCipher(hCipher, pSrcData, 1, &vecEncodedText[j]));
				}
				EXPECT_EQ(EC_SUCCESS, FinalSymCipher(hCipher));
			}

			std::string strComparePath = Format("../../Build/Test/Encoder/CipherData/%s_%s.enc", GetCipherTypeStringA(nType), GetSymEncModeStringA(nMode));

			//if( 0 )
			//{
			//	DumpToFile(strComparePath, vecEncodedText);
			//}
			//else
			//{
			//	std::vector<BYTE> vecCompareData;
			//	ReadFromFile(strComparePath, vecCompareData, tEncodeSize);

			//	ASSERT_EQ(vecCompareData.size(), vecEncodedText.size());
			//	EXPECT_EQ(0, memcmp(&vecCompareData[0], &vecEncodedText[0], vecCompareData.size()));

			//	//printf("%s\n", StringFromHexA(&vecCompareData[0], vecCompareData.size()).c_str());
			//}

			std::string strRestoredText;
			strRestoredText.resize(vecEncodedText.size());

			{
				HANDLE hCipher = InitSymCipher(stCipherKey, CIPHER_METHOD_DECRYPT);
				ASSERT_NE((HANDLE)NULL, hCipher);

				size_t j;
				for(j=0; j<vecEncodedText.size(); j+=stCipherKey.dwBlockSize)
				{
					LPBYTE pDestData = (LPBYTE)strRestoredText.c_str() + j;
					EXPECT_EQ(EC_SUCCESS, UpdateSymCipher(hCipher, &vecEncodedText[j], 1, pDestData));
				}
				EXPECT_EQ(EC_SUCCESS, FinalSymCipher(hCipher));
			}

			strRestoredText.resize(strOriginalText.size());
			EXPECT_EQ(strOriginalText, strRestoredText);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(EncoderTest, CipherTest_SEED128_ECB	)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED128, SYM_CIPHER_MODE_ECB	);	}
TEST(EncoderTest, CipherTest_SEED128_CBC	)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED128, SYM_CIPHER_MODE_CBC	);	}
TEST(EncoderTest, CipherTest_SEED128_PCBC	)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED128, SYM_CIPHER_MODE_PCBC	);	}
TEST(EncoderTest, CipherTest_SEED128_CFB	)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED128, SYM_CIPHER_MODE_CFB	);	}
TEST(EncoderTest, CipherTest_SEED128_OFB	)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED128, SYM_CIPHER_MODE_OFB	);	}
TEST(EncoderTest, CipherTest_SEED128_CTR	)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED128, SYM_CIPHER_MODE_CTR	);	}

//////////////////////////////////////////////////////////////////////////
TEST(EncoderTest, CipherTest_SEED256_ECB	)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED256, SYM_CIPHER_MODE_ECB	);	}
TEST(EncoderTest, CipherTest_SEED256_CBC	)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED256, SYM_CIPHER_MODE_CBC	);	}
TEST(EncoderTest, CipherTest_SEED256_PCBC	)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED256, SYM_CIPHER_MODE_PCBC	);	}
TEST(EncoderTest, CipherTest_SEED256_CFB	)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED256, SYM_CIPHER_MODE_CFB	);	}
TEST(EncoderTest, CipherTest_SEED256_OFB	)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED256, SYM_CIPHER_MODE_OFB	);	}
TEST(EncoderTest, CipherTest_SEED256_CTR	)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_SEED256, SYM_CIPHER_MODE_CTR	);	}

//////////////////////////////////////////////////////////////////////////
TEST(EncoderTest, CipherTest_AES128_ECB		)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES128, SYM_CIPHER_MODE_ECB	);	}
TEST(EncoderTest, CipherTest_AES128_CBC		)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES128, SYM_CIPHER_MODE_CBC	);	}
TEST(EncoderTest, CipherTest_AES128_PCBC	)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES128, SYM_CIPHER_MODE_PCBC);	}
TEST(EncoderTest, CipherTest_AES128_CFB		)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES128, SYM_CIPHER_MODE_CFB	);	}
TEST(EncoderTest, CipherTest_AES128_OFB		)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES128, SYM_CIPHER_MODE_OFB	);	}
TEST(EncoderTest, CipherTest_AES128_CTR		)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES128, SYM_CIPHER_MODE_CTR	);	}

//////////////////////////////////////////////////////////////////////////
TEST(EncoderTest, CipherTest_AES192_ECB		)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES192, SYM_CIPHER_MODE_ECB	);	}
TEST(EncoderTest, CipherTest_AES192_CBC		)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES192, SYM_CIPHER_MODE_CBC	);	}
TEST(EncoderTest, CipherTest_AES192_PCBC	)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES192, SYM_CIPHER_MODE_PCBC);	}
TEST(EncoderTest, CipherTest_AES192_CFB		)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES192, SYM_CIPHER_MODE_CFB	);	}
TEST(EncoderTest, CipherTest_AES192_OFB		)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES192, SYM_CIPHER_MODE_OFB	);	}
TEST(EncoderTest, CipherTest_AES192_CTR		)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES192, SYM_CIPHER_MODE_CTR	);	}

//////////////////////////////////////////////////////////////////////////
TEST(EncoderTest, CipherTest_AES256_ECB		)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES256, SYM_CIPHER_MODE_ECB	);	}
TEST(EncoderTest, CipherTest_AES256_CBC		)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES256, SYM_CIPHER_MODE_CBC	);	}
TEST(EncoderTest, CipherTest_AES256_PCBC	)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES256, SYM_CIPHER_MODE_PCBC);	}
TEST(EncoderTest, CipherTest_AES256_CFB		)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES256, SYM_CIPHER_MODE_CFB	);	}
TEST(EncoderTest, CipherTest_AES256_OFB		)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES256, SYM_CIPHER_MODE_OFB	);	}
TEST(EncoderTest, CipherTest_AES256_CTR		)	{	SYM_CIPHER_TEST(SYM_CIPHER_TYPE_AES256, SYM_CIPHER_MODE_CTR	);	}
