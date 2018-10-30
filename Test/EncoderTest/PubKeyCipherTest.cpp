#include "stdafx.h"
#include "../../Src/001_Encoder/PubKeyCipherFunction.h"

//////////////////////////////////////////////////////////////////////////
TEST(PubKeyCipherTest, ElGamalTest)
{
	std::vector<BYTE> vecPriv, vecPub;
	GenerateCipherKey(PUBKEY_CIPHER_TYPE_ELGAMAL, 2048, "test", vecPriv, vecPub);

	std::string strTest = "Hello world??";
	std::vector<BYTE> vecEnc;

	{
		ST_PUBKEY_CIPHER_INFO stInfo;
		HANDLE hEncoder = InitPubKeyCipher(PUBKEY_CIPHER_TYPE_ELGAMAL, vecPub, "test", CIPHER_METHOD_ENCRYPT, stInfo);
		strTest.resize(stInfo.dwPlainBlockSize);
		vecEnc.resize(stInfo.dwCipherBlockSize);
		UpdatePubKeyCipher(hEncoder, (LPCBYTE)strTest.c_str(), &vecEnc[0]);
		FinalPubKeyCipher(hEncoder);
	}

	std::string strRestored;
	{
		ST_PUBKEY_CIPHER_INFO stInfo;
		HANDLE hDecoder = InitPubKeyCipher(PUBKEY_CIPHER_TYPE_ELGAMAL, vecPriv, "test", CIPHER_METHOD_DECRYPT, stInfo);
		strRestored.resize(stInfo.dwPlainBlockSize);
		UpdatePubKeyCipher(hDecoder, &vecEnc[0], (LPBYTE)strRestored.c_str());
		FinalPubKeyCipher(hDecoder);
	}

	EXPECT_EQ(strTest, strRestored);
}

//////////////////////////////////////////////////////////////////////////
TEST(PubKeyCipherTest, RSATest)
{
	std::vector<BYTE> vecPriv, vecPub;
	GenerateCipherKey(PUBKEY_CIPHER_TYPE_RSA, 2048, "test", vecPriv, vecPub);

	std::string strTest = "Hello world??";
	std::vector<BYTE> vecEnc;

	{
		ST_PUBKEY_CIPHER_INFO stInfo;
		HANDLE hEncoder = InitPubKeyCipher(PUBKEY_CIPHER_TYPE_RSA, vecPub, "test", CIPHER_METHOD_ENCRYPT, stInfo);
		strTest.resize(stInfo.dwPlainBlockSize);
		vecEnc.resize(stInfo.dwCipherBlockSize);
		UpdatePubKeyCipher(hEncoder, (LPCBYTE)strTest.c_str(), &vecEnc[0]);
		FinalPubKeyCipher(hEncoder);
	}

	std::string strRestored;
	{
		ST_PUBKEY_CIPHER_INFO stInfo;
		HANDLE hDecoder = InitPubKeyCipher(PUBKEY_CIPHER_TYPE_RSA, vecPriv, "test", CIPHER_METHOD_DECRYPT, stInfo);
		strRestored.resize(stInfo.dwPlainBlockSize);
		UpdatePubKeyCipher(hDecoder, &vecEnc[0], (LPBYTE)strRestored.c_str());
		FinalPubKeyCipher(hDecoder);
	}

	EXPECT_EQ(strTest, strRestored);
}
