#pragma once

#include <vector>

#include "../__Common/Type.h"
#include "Type.h"
#include "IPubKeyCipher.h"

namespace core
{
	ECODE	GenerateCipherKey(E_PUBKEY_CIPHER_TYPE nType, size_t tKeyLen, const char* pszSEED, std::vector<BYTE>& vecPrivKey, std::vector<BYTE>& vecPubKey);

	HANDLE		InitPubKeyCipher(E_PUBKEY_CIPHER_TYPE nType, std::vector<BYTE> vecKey, std::string strSeed, E_CIPHER_METHOD nMethod, ST_PUBKEY_CIPHER_INFO& outInfo);
	ECODE		UpdatePubKeyCipher(HANDLE hCipher, const BYTE* pSrc, BYTE* pDest);
	void		FinalPubKeyCipher(HANDLE hCipher);
}