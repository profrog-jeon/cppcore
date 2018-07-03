#pragma once

#include "ISymAlgorithm.h"

namespace core
{
	class CSymSeed256 : public ISymAlgorithm
	{
		DWORD m_dwRoundKey[48];

	public:
		CSymSeed256(void);
		~CSymSeed256(void);

		ECODE Init(E_CIPHER_METHOD nMethod, const ST_SYM_CIPHER_KEY& stKey);
		ECODE Encrypt(LPCBYTE pSrc, LPBYTE pDest);
		ECODE Decrypt(LPCBYTE pSrc, LPBYTE pDest);
		size_t BlockSize(void)		{	return 16;	}
	};
}
