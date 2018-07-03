#pragma once

#include <vector>
#include "ISymMode.h"
#include "ISymAlgorithm.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	struct ISymCipher
	{
		virtual ~ISymCipher(void)	{}
		virtual ECODE Init(ST_SYM_CIPHER_KEY stKey) = 0;
		virtual ECODE Update(LPCBYTE pSrc, size_t tBlockCount, LPBYTE pDest) = 0;
		virtual ECODE Final(void) = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	class CSymEncoder : public ISymCipher
	{
		ISymAlgorithm*			m_pAlgorithm	;
		ISymEncryptMode*		m_pMode			;

		size_t					m_tBlockSize	;

	public:
		CSymEncoder(ISymAlgorithm* pAlgorithm, ISymEncryptMode* pMode);
		virtual ~CSymEncoder(void);

		ECODE Init(ST_SYM_CIPHER_KEY stKey);
		ECODE Update(LPCBYTE pSrc, size_t tBlockCount, LPBYTE pDest);
		ECODE Final(void);
	};

	//////////////////////////////////////////////////////////////////////////
	class CSymDecoder : public ISymCipher
	{
		ISymAlgorithm*			m_pAlgorithm	;
		ISymDecryptMode*		m_pMode			;

		size_t					m_tBlockSize	;

	public:
		CSymDecoder(ISymAlgorithm* pAlgorithm, ISymDecryptMode* pMode);
		virtual ~CSymDecoder(void);

		ECODE Init(ST_SYM_CIPHER_KEY stKey);
		ECODE Update(LPCBYTE pSrc, size_t tBlockCount, LPBYTE pDest);
		ECODE Final(void);
	};
}
