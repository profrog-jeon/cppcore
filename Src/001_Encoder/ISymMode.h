#pragma once

#include "Type.h"
#include "ISymAlgorithm.h"

namespace core
{
	struct ISymModeSuper
	{
		virtual ~ISymModeSuper(void)	{}
		virtual ECODE Init(E_CIPHER_METHOD nMethod, ISymAlgorithm* pAlgorithm, const ST_SYM_CIPHER_KEY& stKey) = 0;
	};

	struct ISymEncryptMode : public ISymModeSuper
	{
		virtual ECODE Encrypt(ISymAlgorithm* pAlgorithm, LPCBYTE pSrc, LPBYTE pDest) = 0;
	};

	struct ISymDecryptMode : public ISymModeSuper
	{
		virtual ECODE Decrypt(ISymAlgorithm* pAlgorithm, LPCBYTE pSrc, LPBYTE pDest) = 0;
	};

	struct ISymMode : public ISymEncryptMode, public ISymDecryptMode
	{
		virtual ~ISymMode(void)	{}
	};
}
