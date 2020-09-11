#pragma once

#include "../../Inc/randpool.h"
#include "../../Inc/rsa.h"
#include "../../Inc/elgamal.h"
#include "../../Inc/hex.h"
#include "../../Inc/files.h"
#include "../../Inc/filters.h"
#include "../../Inc/aes.h"
#include "../../Inc/modes.h"

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
