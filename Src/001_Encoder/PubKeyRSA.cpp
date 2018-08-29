#include "stdafx.h"
#include "PubKeyRSA.h"
#include "CryptoPPSingleton.h"

namespace core
{
	using namespace CryptoPP;

	CPubKeyRSAEncoder::CPubKeyRSAEncoder(std::vector<BYTE> vecKey, std::string strSeed)
		: m_pCryptor(NULL)
	{
		ArraySource source(&vecKey[0], vecKey.size(), true);
		m_pCryptor = new RSAES_OAEP_SHA_Encryptor(source);
	}

	CPubKeyRSAEncoder::~CPubKeyRSAEncoder()
	{
		if( m_pCryptor )
			delete m_pCryptor;
		m_pCryptor = NULL;
	}

	void CPubKeyRSAEncoder::QueryInfo(ST_PUBKEY_CIPHER_INFO& outInfo)
	{
		outInfo.dwPlainBlockSize = m_pCryptor->FixedMaxPlaintextLength();
		outInfo.dwCipherBlockSize = m_pCryptor->FixedCiphertextLength();
	}

	ECODE CPubKeyRSAEncoder::Crypt(LPCBYTE btSrc, LPBYTE btDest)
	{
		try
		{
			ArraySource(btSrc, m_pCryptor->FixedMaxPlaintextLength(), true, new PK_EncryptorFilter(CryptoPP()->RandPool, *m_pCryptor, new ArraySink(btDest, m_pCryptor->FixedCiphertextLength())));
			return EC_SUCCESS;
		}
		catch (std::exception& e)
		{
			printf("%s\n", e.what());
			return EC_INTERNAL_ERROR;
		}
	}



	CPubKeyRSADecoder::CPubKeyRSADecoder(std::vector<BYTE> vecKey, std::string strSeed)
		: m_pCryptor(NULL)
	{
		ArraySource source(&vecKey[0], vecKey.size(), true);
		m_pCryptor = new RSAES_OAEP_SHA_Decryptor(source);
	}

	CPubKeyRSADecoder::~CPubKeyRSADecoder()
	{
		if( m_pCryptor )
			delete m_pCryptor;
		m_pCryptor = NULL;
	}

	void CPubKeyRSADecoder::QueryInfo(ST_PUBKEY_CIPHER_INFO& outInfo)
	{
		outInfo.dwPlainBlockSize = m_pCryptor->FixedMaxPlaintextLength();
		outInfo.dwCipherBlockSize = m_pCryptor->FixedCiphertextLength();
	}

	ECODE CPubKeyRSADecoder::Crypt(LPCBYTE btSrc, LPBYTE btDest)
	{
		try
		{
			ArraySource(btSrc, m_pCryptor->FixedCiphertextLength(), true, new PK_DecryptorFilter(CryptoPP()->RandPool, *m_pCryptor, new ArraySink(btDest, m_pCryptor->FixedMaxPlaintextLength())));
			return EC_SUCCESS;
		}
		catch (std::exception& e)
		{
			printf("%s\n", e.what());
			return EC_INTERNAL_ERROR;
		}
	}
}
