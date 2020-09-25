#pragma once

#include "../../Inc/cryptopp700/randpool.h"
#include "../../Inc/cryptopp700/rsa.h"
#include "../../Inc/cryptopp700/elgamal.h"
#include "../../Inc/cryptopp700/hex.h"
#include "../../Inc/cryptopp700/files.h"
#include "../../Inc/cryptopp700/filters.h"
#include "../../Inc/cryptopp700/aes.h"
#include "../../Inc/cryptopp700/modes.h"

#include "IPubKeyCipher.h"

namespace core
{
	class CPubKeyElGamalEncoder : public IPubKeyCipher
	{
		CryptoPP::ElGamal::Encryptor* m_pCryptor;

	public:
		CPubKeyElGamalEncoder(std::vector<BYTE> vecKey, std::string strSeed);
		~CPubKeyElGamalEncoder();

		void QueryInfo(ST_PUBKEY_CIPHER_INFO& outInfo);
		ECODE Crypt(LPCBYTE btSrc, LPBYTE btDest);
	};

	class CPubKeyElGamalDecoder : public IPubKeyCipher
	{
		CryptoPP::ElGamal::Decryptor* m_pCryptor;

	public:
		CPubKeyElGamalDecoder(std::vector<BYTE> vecKey, std::string strSeed);
		~CPubKeyElGamalDecoder();

		void QueryInfo(ST_PUBKEY_CIPHER_INFO& outInfo);
		ECODE Crypt(LPCBYTE btSrc, LPBYTE btDest);
	};
}
