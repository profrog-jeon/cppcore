#include "stdafx.h"
#include "SymModePlain.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CSymModePlain::CSymModePlain(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CSymModePlain::~CSymModePlain(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymModePlain::Init(E_CIPHER_METHOD nMethod, ISymAlgorithm* pAlgorithm, const ST_SYM_CIPHER_KEY& stKey)
	{
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymModePlain::Encrypt(ISymAlgorithm* pAlgorithm, LPCBYTE pSrc, LPBYTE pDest)
	{
		return pAlgorithm->Encrypt(pSrc, pDest);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymModePlain::Decrypt(ISymAlgorithm* pAlgorithm, LPCBYTE pSrc, LPBYTE pDest)
	{
		return pAlgorithm->Decrypt(pSrc, pDest);
	}
}
