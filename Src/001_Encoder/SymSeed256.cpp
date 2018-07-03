#include "stdafx.h"
#include "SymSeed256.h"
#include "SEED/SEED_256_KISA.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CSymSeed256::CSymSeed256(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CSymSeed256::~CSymSeed256(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymSeed256::Init(E_CIPHER_METHOD nMethod, const ST_SYM_CIPHER_KEY& stKey)
	{
		seed256::SeedRoundKey(m_dwRoundKey, stKey.btKey);
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymSeed256::Encrypt(LPCBYTE pSrc, LPBYTE pDest)
	{
		memcpy(pDest, pSrc, BlockSize());
		seed256::SeedEncrypt(pDest, m_dwRoundKey);
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymSeed256::Decrypt(LPCBYTE pSrc, LPBYTE pDest)
	{
		memcpy(pDest, pSrc, BlockSize());
		seed256::SeedDecrypt(pDest, m_dwRoundKey);
		return EC_SUCCESS;
	}
}

