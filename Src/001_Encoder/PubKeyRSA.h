#pragma once

#include "../../Inc/randpool.h"
#include "../../Inc/rsa.h"
#include "../../Inc/hex.h"
#include "../../Inc/files.h"
#include "../../Inc/filters.h"
#include "../../Inc/aes.h"
#include "../../Inc/modes.h"

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
