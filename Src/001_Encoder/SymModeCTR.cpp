#include "stdafx.h"
#include "SymModeCTR.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CSymModeCTR::CSymModeCTR(void)
		: m_tBlockSize(0)
		, m_dwCounter(0)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CSymModeCTR::~CSymModeCTR(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymModeCTR::Init(E_CIPHER_METHOD nMethod, ISymAlgorithm* pAlgorithm, const ST_SYM_CIPHER_KEY& stKey)
	{
		memcpy(m_dwXOR, stKey.btInitialVector, sizeof(stKey.btInitialVector));
		m_tBlockSize = pAlgorithm->BlockSize();
		m_dwCounter = 0;
		return pAlgorithm->Init(CIPHER_METHOD_ENCRYPT, stKey);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymModeCTR::Encrypt(ISymAlgorithm* pAlgorithm, LPCBYTE pSrc, LPBYTE pDest)
	{
		DWORD dwInput[8];
		memcpy(dwInput, m_dwXOR, m_tBlockSize);

		switch(m_tBlockSize)
		{
		case 32:
			dwInput[7] ^= m_dwCounter;	break;
		case 24:
			dwInput[5] ^= m_dwCounter;	break;
		case 16:
			dwInput[3] ^= m_dwCounter;	break;
		case 8:
			dwInput[1] ^= m_dwCounter;	break;
			break;
		}

		m_dwCounter ++;

		ECODE nRet = pAlgorithm->Encrypt((LPCBYTE)dwInput, pDest);
		if( EC_SUCCESS != nRet )
			return nRet;

		const LPDWORD pInput =  (LPDWORD)pSrc;
		const LPDWORD pOutput = (LPDWORD)pDest;
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
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymModeCTR::Decrypt(ISymAlgorithm* pAlgorithm, LPCBYTE pSrc, LPBYTE pDest)
	{
		DWORD dwInput[8];
		memcpy(dwInput, m_dwXOR, m_tBlockSize);

		switch(m_tBlockSize)
		{
		case 32:
			dwInput[7] ^= m_dwCounter;	break;
		case 24:
			dwInput[5] ^= m_dwCounter;	break;
		case 16:
			dwInput[3] ^= m_dwCounter;	break;
		case 8:
			dwInput[1] ^= m_dwCounter;	break;
			break;
		}

		m_dwCounter ++;

		ECODE nRet = pAlgorithm->Encrypt((LPCBYTE)dwInput, pDest);
		if( EC_SUCCESS != nRet )
			return nRet;

		const LPDWORD pInput =  (LPDWORD)pSrc;
		const LPDWORD pOutput = (LPDWORD)pDest;
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
		return EC_SUCCESS;
	}
}
