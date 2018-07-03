#pragma once

#include "ISymAlgorithm.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	class CSymAES : public ISymAlgorithm
	{
		void*		m_pCtx;
		size_t		m_tLengthOfKey;

	protected:
					CSymAES(size_t tLengthOfKey);
		virtual		~CSymAES(void);

	public:
		ECODE Init(E_CIPHER_METHOD nMethod, const ST_SYM_CIPHER_KEY& stKey);
		ECODE Encrypt(LPCBYTE pSrc, LPBYTE pDest);
		ECODE Decrypt(LPCBYTE pSrc, LPBYTE pDest);
		size_t BlockSize(void) {	return 16;	}
	};

	//////////////////////////////////////////////////////////////////////////
	class CSymAES128 : public CSymAES
	{
	public:
		CSymAES128(void): CSymAES(16)	{}
	};

	//////////////////////////////////////////////////////////////////////////
	class CSymAES192 : public CSymAES
	{
	public:
		CSymAES192(void): CSymAES(24)	{}
	};

	//////////////////////////////////////////////////////////////////////////
	class CSymAES256 : public CSymAES
	{
	public:
		CSymAES256(void): CSymAES(32)	{}
	};
}
