#pragma once

#include <vector>

#include "../__Common/Type.h"
#include "Type.h"

namespace core
{
	ECODE		GenerateCipherKey(E_SYM_CIPHER_TYPE nType, E_SYM_CIPHER_MODE nMode, std::string strKey, OUT ST_SYM_CIPHER_KEY &stCipher);
	ECODE		GenerateCipherKey(E_SYM_CIPHER_TYPE nType, E_SYM_CIPHER_MODE nMode, const BYTE pKey[], const BYTE pIV[], OUT ST_SYM_CIPHER_KEY &stCipher);
	HANDLE		InitSymCipher(ST_SYM_CIPHER_KEY stCipherKey, E_CIPHER_METHOD nMethod);
	ECODE		UpdateSymCipher(HANDLE hCipher, const BYTE* pSrc, size_t tBlockCount, BYTE* pDest);
	ECODE		FinalSymCipher(HANDLE hCipher);

	size_t		EncodeData(ST_SYM_CIPHER_KEY stCipherKey, LPCBYTE pSrc, size_t tSrcSize, LPBYTE pDest = NULL);
	size_t		DecodeData(ST_SYM_CIPHER_KEY stCipherKey, LPCBYTE pSrc, size_t tSrcSize, LPBYTE pDest = NULL);
}
