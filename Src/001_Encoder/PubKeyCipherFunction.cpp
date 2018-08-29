#include "stdafx.h"
#include "PubKeyCipherFunction.h"
#include "PubKeyRSA.h"
#include "PubKeyElGamal.h"
#include "CryptoPPSingleton.h"

namespace core
{
	using namespace CryptoPP;

	ECODE GenerateCipherKey(E_PUBKEY_CIPHER_TYPE nType, size_t tKeyLen, const char* pszSEED, std::vector<BYTE>& vecPrivKey, std::vector<BYTE>& vecPubKey)
	{
		switch (nType)
		{
			case PUBKEY_CIPHER_TYPE_RSA:
			{
				vecPrivKey.resize(tKeyLen);
				vecPubKey.resize(tKeyLen);

				RSAES_OAEP_SHA_Decryptor decryptor(CryptoPP()->RandPool, tKeyLen);
				ArraySink privKeySink = ArraySink(&vecPrivKey[0], vecPrivKey.size());
				decryptor.AccessMaterial().Save(privKeySink);
				vecPrivKey.resize(privKeySink.TotalPutLength());

				RSAES_OAEP_SHA_Encryptor encryptor(decryptor);
				ArraySink pubKeySink = ArraySink(&vecPubKey[0], vecPubKey.size());
				encryptor.AccessMaterial().Save(pubKeySink);
				vecPubKey.resize(pubKeySink.TotalPutLength());

				return EC_SUCCESS;
			}

			case PUBKEY_CIPHER_TYPE_ELGAMAL:
			{
				vecPrivKey.resize(tKeyLen);
				vecPubKey.resize(tKeyLen);

				ElGamal::Decryptor decryptor;
				decryptor.AccessKey().GenerateRandomWithKeySize(CryptoPP()->RandPool, 2048);	// Generating private key. This may take some time...
				const ElGamalKeys::PrivateKey& privateKey = decryptor.AccessKey();
				ArraySink privKeySink = ArraySink(&vecPrivKey[0], vecPrivKey.size());
				privateKey.Save(privKeySink);
				vecPrivKey.resize(privKeySink.TotalPutLength());

				ElGamal::Encryptor encryptor(decryptor);
				const PublicKey& publicKey = encryptor.AccessKey();
				ArraySink pubKeySink = ArraySink(&vecPubKey[0], vecPubKey.size());
				publicKey.Save(pubKeySink);
				vecPubKey.resize(pubKeySink.TotalPutLength());

				return EC_SUCCESS;
			}
		}

		return EC_INVALID_ARGUMENT;
	}

	HANDLE InitPubKeyCipher(E_PUBKEY_CIPHER_TYPE nType, std::vector<BYTE> vecKey, std::string strSeed, E_CIPHER_METHOD nMethod, ST_PUBKEY_CIPHER_INFO& outInfo)
	{
		IPubKeyCipher* pCipher = NULL;
		if (CIPHER_METHOD_ENCRYPT == nMethod)
		{
			switch (nType)
			{
			case PUBKEY_CIPHER_TYPE_RSA:
				pCipher = new CPubKeyRSAEncoder(vecKey, strSeed);
				break;

			case PUBKEY_CIPHER_TYPE_ELGAMAL:
				pCipher = new CPubKeyElGamalEncoder(vecKey, strSeed);
				break;
			}
		}
		if (CIPHER_METHOD_DECRYPT == nMethod)
		{
			switch (nType)
			{
			case PUBKEY_CIPHER_TYPE_RSA:
				pCipher = new CPubKeyRSADecoder(vecKey, strSeed);
				break;

			case PUBKEY_CIPHER_TYPE_ELGAMAL:
				pCipher = new CPubKeyElGamalDecoder(vecKey, strSeed);
				break;
			}
		}

		if (NULL == pCipher)
			return NULL;

		pCipher->QueryInfo(outInfo);
		return pCipher;

	}

	ECODE UpdatePubKeyCipher(HANDLE hCipher, const BYTE* pSrc, BYTE* pDest)
	{
		IPubKeyCipher* pCipher = (IPubKeyCipher*)hCipher;
		return pCipher->Crypt(pSrc, pDest);
	}

	void FinalPubKeyCipher(HANDLE hCipher)
	{
		IPubKeyCipher* pCipher = (IPubKeyCipher*)hCipher;
		delete pCipher;
	}
}