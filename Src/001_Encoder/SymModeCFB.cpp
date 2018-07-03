#include "stdafx.h"
#include "SymModeCFB.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CSymModeCFB::CSymModeCFB(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CSymModeCFB::~CSymModeCFB(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymModeCFB::Init(E_CIPHER_METHOD nMethod, ISymAlgorithm* pAlgorithm, const ST_SYM_CIPHER_KEY& stKey)
	{
		memcpy(m_dwXOR, stKey.btInitialVector, sizeof(stKey.btInitialVector));
		m_tBlockSize = pAlgorithm->BlockSize();
		return pAlgorithm->Init(CIPHER_METHOD_ENCRYPT, stKey);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymModeCFB::Encrypt(ISymAlgorithm* pAlgorithm, LPCBYTE pSrc, LPBYTE pDest)
	{
		ECODE nRet = pAlgorithm->Encrypt((LPCBYTE)m_dwXOR, pDest);
		if( EC_SUCCESS != nRet )
			return nRet;

		const LPDWORD pInput = (const LPDWORD)pSrc;
		LPDWORD pOutput = (LPDWORD)pDest;
		switch(m_tBlockSize)
		{
		case 32:
			pOutput[7] ^= pInput[7];
			pOutput[6] ^= pInput[6];
		case 24:
			pOutput[5] ^= pInput[5];
			pOutput[4] ^= pInput[4];
		case 16:
			pOutput[3] ^= pInput[3];
			pOutput[2] ^= pInput[2];
		case 8:
			pOutput[1] ^= pInput[1];
			pOutput[0] ^= pInput[0];
			break;
		}
		memcpy(m_dwXOR, pOutput, m_tBlockSize);
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymModeCFB::Decrypt(ISymAlgorithm* pAlgorithm, LPCBYTE pSrc, LPBYTE pDest)
	{
		ECODE nRet = pAlgorithm->Encrypt((LPCBYTE)m_dwXOR, pDest);
		if( EC_SUCCESS != nRet )
			return nRet;

		const LPDWORD pInput = (const LPDWORD)pSrc;
		LPDWORD pOutput = (LPDWORD)pDest;
		switch(m_tBlockSize)
		{
		case 32:
			pOutput[7] ^= pInput[7];
			pOutput[6] ^= pInput[6];
		case 24:
			pOutput[5] ^= pInput[5];
			pOutput[4] ^= pInput[4];
		case 16:
			pOutput[3] ^= pInput[3];
			pOutput[2] ^= pInput[2];
		case 8:
			pOutput[1] ^= pInput[1];
			pOutput[0] ^= pInput[0];
			break;
		}
		memcpy(m_dwXOR, pSrc, m_tBlockSize);
		return EC_SUCCESS;
	}
}
