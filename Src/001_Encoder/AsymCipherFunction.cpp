#include "stdafx.h"
#include "AsymCipherFunction.h"
#include "AsymRSA.h"

namespace core
{
	ECODE GenerateAsymCipherKey(E_ASYM_CIPHER_TYPE nType, size_t tKeyLength, const char* pszSeed, std::vector<BYTE>& vecPrivKey, std::vector<BYTE>& vecPubKey)
	{
		IAsymAlgorithm* pAlgorithm = NULL;
		switch (nType)
		{
		case ASYM_CIPHER_TYPE_RSA:
			pAlgorithm = new CAsymRSA();
			break;

		default:
			return EC_INVALID_ARGUMENT;
		}

		pAlgorithm->GenerateKey(tKeyLength, pszSeed, vecPrivKey, vecPubKey);
		delete pAlgorithm;
		return EC_SUCCESS;
	}

	HANDLE InitAsymCipher(E_ASYM_CIPHER_TYPE nType, E_CIPHER_METHOD nMethod, const char* pszSeed, std::vector<BYTE> vecKey)
	{
		IAsymAlgorithm* pAlgorithm = NULL;
		switch (nType)
		{
		case ASYM_CIPHER_TYPE_RSA:
			pAlgorithm = new CAsymRSA();
			break;

		default:
			return NULL;
		}

		switch (nMethod)
		{
		case CIPHER_METHOD_ENCRYPT:
			return new CAsymEncoder(pAlgorithm);

		case CIPHER_METHOD_DECRYPT:
			return new CAsymDecoder(pAlgorithm);
		}

		delete pAlgorithm;
		return NULL;
	}

	ECODE UpdateAsymCipher(HANDLE hCipher, const BYTE* pSrc, size_t tBlockCount, BYTE* pDest)
	{
		IAsymCipher* pCipher = (IAsymCipher*)hCipher;
		return pCipher->Update(pSrc, tBlockCount, pDest);
	}

	ECODE FinalAsymCipher(HANDLE hCipher)
	{
		IAsymCipher* pCipher = (IAsymCipher*)hCipher;
		ECODE nRet = pCipher->Final();
		delete pCipher;
		return nRet;
	}
}