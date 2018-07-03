#pragma once

#include "Type.h"

namespace core
{
	struct ISymAlgorithm
	{
		virtual ~ISymAlgorithm(void)	{}
		virtual ECODE Init(E_CIPHER_METHOD nMethod, const ST_SYM_CIPHER_KEY& stKey) = 0;
		virtual size_t BlockSize(void) = 0;
		virtual ECODE Encrypt(LPCBYTE pSrc, LPBYTE pDest) = 0;
		virtual ECODE Decrypt(LPCBYTE pSrc, LPBYTE pDest) = 0;
	};
}
