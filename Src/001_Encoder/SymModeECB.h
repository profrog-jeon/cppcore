#pragma once

#include "ISymMode.h"

namespace core
{
	class CSymModeECB : public ISymMode
	{
	public:
		CSymModeECB(void);
		~CSymModeECB(void);

		ECODE Init(E_CIPHER_METHOD nMethod, ISymAlgorithm* pAlgorithm, const ST_SYM_CIPHER_KEY& stKey);
		ECODE Encrypt(ISymAlgorithm* pAlgorithm, LPCBYTE pSrc, LPBYTE pDest);
		ECODE Decrypt(ISymAlgorithm* pAlgorithm, LPCBYTE pSrc, LPBYTE pDest);
	};

}
