#include "stdafx.h"
#include "AsymRSA.h"

#include "../../../cryptopp/Inc/randpool.h"
#include "../../../cryptopp/Inc/rsa.h"
#include "../../../cryptopp/Inc/hex.h"
#include "../../../cryptopp/Inc/files.h"
#include "../../../cryptopp/Inc/filters.h"
#include "../../../cryptopp/Inc/aes.h"
#include "../../../cryptopp/Inc/modes.h"

using namespace CryptoPP;
static OFB_Mode<AES>::Encryption s_globalRNG;

namespace core
{
	CAsymRSA::CAsymRSA()
		: m_pHandle(NULL)
	{
	}

	CAsymRSA::~CAsymRSA()
	{
	}

	RandomNumberGenerator & GlobalRNG()
	{
		return dynamic_cast<RandomNumberGenerator&>(s_globalRNG);
	}

	ECODE CAsymRSA::GenerateKey(DWORD dwKeySize, const char* pszSeed, std::vector<BYTE> vecPrivKey, std::vector<BYTE> vecPubKey)
	{
		// DEREncode() changed to Save() at Issue 569.
		RandomPool randPool;
		randPool.IncorporateEntropy((byte *)pszSeed, strlen(pszSeed));

		std::string strPrivKey, strPubKey;

		RSAES_OAEP_SHA_Decryptor priv(randPool, dwKeySize);
		HexEncoder privFile(new StringSink(strPrivKey));
		priv.AccessMaterial().Save(privFile);
		privFile.MessageEnd();

		RSAES_OAEP_SHA_Encryptor pub(priv);
		HexEncoder pubFile(new StringSink(strPubKey));
		pub.AccessMaterial().Save(pubFile);
		pubFile.MessageEnd();

		return EC_SUCCESS;
	}

	struct ST_RSA_HANDLE
	{
		E_CIPHER_METHOD nMethod;

		union
		{
			RSAES_OAEP_SHA_Encryptor* pEncoder;
			RSAES_OAEP_SHA_Decryptor* pDecoder;
		};
	};

	ECODE CAsymRSA::Init(E_CIPHER_METHOD nMethod, const std::vector<BYTE>& vecKey)
	{
		if (nMethod != CIPHER_METHOD_ENCRYPT && nMethod != CIPHER_METHOD_DECRYPT)
			return EC_INVALID_ARGUMENT;

		ST_RSA_HANDLE* pHandle = new ST_RSA_HANDLE;
		if (NULL == pHandle)
			return EC_NOT_ENOUGH_MEMORY;

		StringSource pubKeySource(vecKey.data(), vecKey.size(), true);

		if (CIPHER_METHOD_ENCRYPT == nMethod)
			pHandle->pEncoder = new RSAES_OAEP_SHA_Encryptor(pubKeySource);

		m_pHandle = pHandle;
		return EC_SUCCESS;
	}

	size_t CAsymRSA::GetPlainTextLength(void)
	{
		ST_RSA_HANDLE* pHandle = (ST_RSA_HANDLE*)m_pHandle;
		if(CIPHER_METHOD_ENCRYPT == pHandle->nMethod )
			return pHandle->pEncoder->FixedMaxPlaintextLength();
		if (CIPHER_METHOD_DECRYPT == pHandle->nMethod)
			return pHandle->pDecoder->FixedMaxPlaintextLength();
		return 0;
	}

	size_t CAsymRSA::GetCipherTextLength(void)
	{
		ST_RSA_HANDLE* pHandle = (ST_RSA_HANDLE*)m_pHandle;
		if (CIPHER_METHOD_ENCRYPT == pHandle->nMethod)
			return pHandle->pEncoder->FixedCiphertextLength();
		if (CIPHER_METHOD_DECRYPT == pHandle->nMethod)
			return pHandle->pDecoder->FixedCiphertextLength();
		return 0;
	}
}
