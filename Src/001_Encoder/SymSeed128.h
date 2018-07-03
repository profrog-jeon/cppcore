#pragma once

#include "ISymAlgorithm.h"

namespace core
{
	class CSymSeed128 : public ISymAlgorithm
	{
		DWORD m_dwRoundKey[32];

	public:
		CSymSeed128(void);
		~CSymSeed128(void);

		ECODE Init(E_CIPHER_METHOD nMethod, const ST_SYM_CIPHER_KEY& stKey);
		ECODE Encrypt(LPCBYTE pSrc, LPBYTE pDest);
		ECODE Decrypt(LPCBYTE pSrc, LPBYTE pDest);
		size_t BlockSize(void)		{	return 16;	}
	};
}
