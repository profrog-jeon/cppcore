#pragma once

#include "ISymMode.h"

namespace core
{
	class CSymModeOFB : public ISymMode
	{
		DWORD		m_dwXOR[8];
		size_t		m_tBlockSize;

	public:
		CSymModeOFB(void);
		~CSymModeOFB(void);

		ECODE Init(E_CIPHER_METHOD nMethod, ISymAlgorithm* pAlgorithm, const ST_SYM_CIPHER_KEY& stKey);
		ECODE Encrypt(ISymAlgorithm* pAlgorithm, LPCBYTE pSrc, LPBYTE pDest);
		ECODE Decrypt(ISymAlgorithm* pAlgorithm, LPCBYTE pSrc, LPBYTE pDest);
	};

}
