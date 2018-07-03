#pragma once

#include "ISymAlgorithm.h"

namespace core
{
	class CSymPlain : public ISymAlgorithm
	{
	public:
		CSymPlain(void);
		~CSymPlain(void);

		ECODE Init(E_CIPHER_METHOD nMethod, const ST_SYM_CIPHER_KEY& stKey);
		ECODE Encrypt(LPCBYTE pSrc, LPBYTE pDest);
		ECODE Decrypt(LPCBYTE pSrc, LPBYTE pDest);
		size_t BlockSize(void)		{	return 16;	}
	};
}
