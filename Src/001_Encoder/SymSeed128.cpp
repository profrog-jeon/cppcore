#include "stdafx.h"
#include "SymSeed128.h"
#include "SEED/SEED_128_KISA.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CSymSeed128::CSymSeed128(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CSymSeed128::~CSymSeed128(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymSeed128::Init(E_CIPHER_METHOD nMethod, const ST_SYM_CIPHER_KEY& stKey)
	{
		seed128::SeedRoundKey(m_dwRoundKey, stKey.btKey);
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymSeed128::Encrypt(LPCBYTE pSrc, LPBYTE pDest)
	{
		memcpy(pDest, pSrc, BlockSize());
		seed128::SeedEncrypt(pDest, m_dwRoundKey);
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymSeed128::Decrypt(LPCBYTE pSrc, LPBYTE pDest)
	{
		memcpy(pDest, pSrc, BlockSize());
		seed128::SeedDecrypt(pDest, m_dwRoundKey);
		return EC_SUCCESS;
	}
}

