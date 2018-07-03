#include "stdafx.h"
#include "SymModeECB.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CSymModeECB::CSymModeECB(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CSymModeECB::~CSymModeECB(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymModeECB::Init(E_CIPHER_METHOD nMethod, ISymAlgorithm* pAlgorithm, const ST_SYM_CIPHER_KEY& stKey)
	{
		return pAlgorithm->Init(nMethod, stKey);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymModeECB::Encrypt(ISymAlgorithm* pAlgorithm, LPCBYTE pSrc, LPBYTE pDest)
	{
		pAlgorithm->Encrypt(pSrc, pDest);
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymModeECB::Decrypt(ISymAlgorithm* pAlgorithm, LPCBYTE pSrc, LPBYTE pDest)
	{
		pAlgorithm->Decrypt(pSrc, pDest);
		return EC_SUCCESS;
	}
}
