#pragma once

namespace core
{
	HANDLE		InitAsymCipher(ST_SYM_CIPHER_KEY stCipherKey, E_CIPHER_METHOD nMethod);
	ECODE		UpdateAsymCipher(HANDLE hCipher, const BYTE* pSrc, size_t tBlockCount, BYTE* pDest);
	ECODE		FinalAsymCipher(HANDLE hCipher);
}