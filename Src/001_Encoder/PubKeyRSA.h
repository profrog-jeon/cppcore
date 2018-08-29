#pragma once

#include "cryptopp700/randpool.h"
#include "cryptopp700/rsa.h"
#include "cryptopp700/hex.h"
#include "cryptopp700/files.h"
#include "cryptopp700/filters.h"
#include "cryptopp700/aes.h"
#include "cryptopp700/modes.h"

#include "IPubKeyCipher.h"

namespace core
{
	class CPubKeyRSAEncoder : public IPubKeyCipher
	{
		CryptoPP::RSAES_OAEP_SHA_Encryptor* m_pCryptor;

	public:
		CPubKeyRSAEncoder(std::vector<BYTE> vecKey, std::string strSeed);
		~CPubKeyRSAEncoder();

		void QueryInfo(ST_PUBKEY_CIPHER_INFO& outInfo);
		ECODE Crypt(LPCBYTE btSrc, LPBYTE btDest);
	};

	class CPubKeyRSADecoder : public IPubKeyCipher
	{
		CryptoPP::RSAES_OAEP_SHA_Decryptor* m_pCryptor;

	public:
		CPubKeyRSADecoder(std::vector<BYTE> vecKey, std::string strSeed);
		~CPubKeyRSADecoder();

		void QueryInfo(ST_PUBKEY_CIPHER_INFO& outInfo);
		ECODE Crypt(LPCBYTE btSrc, LPBYTE btDest);
	};
}
