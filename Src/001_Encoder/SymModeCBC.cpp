#include "stdafx.h"
#include "SymModeCBC.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CSymModeCBC::CSymModeCBC(void)
		: m_tBlockSize(0)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CSymModeCBC::~CSymModeCBC(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymModeCBC::Init(E_CIPHER_METHOD nMethod, ISymAlgorithm* pAlgorithm, const ST_SYM_CIPHER_KEY& stKey)
	{
		memcpy(m_dwXOR, stKey.btInitialVector, sizeof(stKey.btInitialVector));
		m_tBlockSize = pAlgorithm->BlockSize();
		return pAlgorithm->Init(nMethod, stKey);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymModeCBC::Encrypt(ISymAlgorithm* pAlgorithm, LPCBYTE pSrc, LPBYTE pDest)
	{
		const LPDWORD pInput = (const LPDWORD)pSrc;
		switch(m_tBlockSize)
		{
		case 32:
			m_dwXOR[7] ^= pInput[7];
			m_dwXOR[6] ^= pInput[6];
		case 24:
			m_dwXOR[5] ^= pInput[5];
			m_dwXOR[4] ^= pInput[4];
		case 16:
			m_dwXOR[3] ^= pInput[3];
			m_dwXOR[2] ^= pInput[2];
		case 8:
			m_dwXOR[1] ^= pInput[1];
			m_dwXOR[0] ^= pInput[0];
			break;
		}

		ECODE nRet = pAlgorithm->Encrypt((LPCBYTE)m_dwXOR, pDest);
		if( EC_SUCCESS != nRet )
			return nRet;

		LPDWORD pOutput = (LPDWORD)pDest;
		switch(m_tBlockSize)
		{
		case 32:
			m_dwXOR[7] = pOutput[7];
			m_dwXOR[6] = pOutput[6];
		case 24:
			m_dwXOR[5] = pOutput[5];
			m_dwXOR[4] = pOutput[4];
		case 16:
			m_dwXOR[3] = pOutput[3];
			m_dwXOR[2] = pOutput[2];
		case 8:
			m_dwXOR[1] = pOutput[1];
			m_dwXOR[0] = pOutput[0];
			break;
		}
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymModeCBC::Decrypt(ISymAlgorithm* pAlgorithm, LPCBYTE pSrc, LPBYTE pDest)
	{
		DWORD dwNextXOR[8];
		memcpy(dwNextXOR, pSrc, m_tBlockSize);

		ECODE nRet = pAlgorithm->Decrypt(pSrc, pDest);
		if( EC_SUCCESS != nRet )
			return nRet;

		LPDWORD pOutput = (LPDWORD)pDest;
		switch(m_tBlockSize)
		{
		case 32:
			pOutput[7] ^= m_dwXOR[7];
			pOutput[6] ^= m_dwXOR[6];
		case 24:
			pOutput[5] ^= m_dwXOR[5];
			pOutput[4] ^= m_dwXOR[4];
		case 16:
			pOutput[3] ^= m_dwXOR[3];
			pOutput[2] ^= m_dwXOR[2];
		case 8:
			pOutput[1] ^= m_dwXOR[1];
			pOutput[0] ^= m_dwXOR[0];
			break;
		}

		memcpy(m_dwXOR, dwNextXOR, m_tBlockSize);
		return EC_SUCCESS;
	}
}
