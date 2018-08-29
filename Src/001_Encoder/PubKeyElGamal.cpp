#include "stdafx.h"
#include "PubKeyElGamal.h"
#include "CryptoPPSingleton.h"

namespace core
{
	using namespace CryptoPP;

	CPubKeyElGamalEncoder::CPubKeyElGamalEncoder(std::vector<BYTE> vecKey, std::string strSeed)
		: m_pCryptor(NULL)
	{
		ElGamalKeys::PublicKey publicKey;
		ArraySource source(&vecKey[0], vecKey.size(), true);
		publicKey.Load(source);
		publicKey.Validate(CryptoPP()->RandPool, 3);
		m_pCryptor = new ElGamal::Encryptor(publicKey);
	}

	CPubKeyElGamalEncoder::~CPubKeyElGamalEncoder()
	{
		if( m_pCryptor )
			delete m_pCryptor;
		m_pCryptor = NULL;
	}

	void CPubKeyElGamalEncoder::QueryInfo(ST_PUBKEY_CIPHER_INFO& outInfo)
	{
		outInfo.dwPlainBlockSize = m_pCryptor->FixedMaxPlaintextLength();
		outInfo.dwCipherBlockSize = m_pCryptor->FixedCiphertextLength();
	}

	ECODE CPubKeyElGamalEncoder::Crypt(LPCBYTE btSrc, LPBYTE btDest)
	{
		try
		{
			m_pCryptor->Encrypt(CryptoPP()->RandPool, (byte*)btSrc, m_pCryptor->FixedMaxPlaintextLength(), btDest);
			return EC_SUCCESS;
		}
		catch (std::exception& e)
		{
			printf("%s\n", e.what());
			return EC_INTERNAL_ERROR;
		}
	}



	CPubKeyElGamalDecoder::CPubKeyElGamalDecoder(std::vector<BYTE> vecKey, std::string strSeed)
		: m_pCryptor(NULL)
	{
		ElGamalKeys::PrivateKey privateKey;
		ArraySource source(&vecKey[0], vecKey.size(), true);
		privateKey.Load(source);
		privateKey.Validate(CryptoPP()->RandPool, 3);
		m_pCryptor = new ElGamal::Decryptor(privateKey);
	}

	CPubKeyElGamalDecoder::~CPubKeyElGamalDecoder()
	{
		if( m_pCryptor )
			delete m_pCryptor;
		m_pCryptor = NULL;
	}

	void CPubKeyElGamalDecoder::QueryInfo(ST_PUBKEY_CIPHER_INFO& outInfo)
	{
		outInfo.dwPlainBlockSize = m_pCryptor->FixedMaxPlaintextLength();
		outInfo.dwCipherBlockSize = m_pCryptor->FixedCiphertextLength();
	}

	ECODE CPubKeyElGamalDecoder::Crypt(LPCBYTE btSrc, LPBYTE btDest)
	{
		try
		{
			m_pCryptor->Decrypt(CryptoPP()->RandPool, (LPCBYTE)btSrc, m_pCryptor->FixedCiphertextLength(), btDest);
			return EC_SUCCESS;
		}
		catch (std::exception& e)
		{
			printf("%s\n", e.what());
			return EC_INTERNAL_ERROR;
		}
	}
}
