#include "stdafx.h"
#include "SymCipher.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CSymEncoder::CSymEncoder(ISymAlgorithm* pAlgorithm, ISymEncryptMode* pMode)
		: m_pAlgorithm(pAlgorithm)
		, m_pMode(pMode)
		, m_tBlockSize(0)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CSymEncoder::~CSymEncoder(void)
	{
		if( m_pAlgorithm )
			delete m_pAlgorithm;
		if( m_pMode )
			delete m_pMode;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymEncoder::Init(ST_SYM_CIPHER_KEY stKey)
	{
		m_tBlockSize	= m_pAlgorithm->BlockSize();
		return m_pMode->Init(CIPHER_METHOD_ENCRYPT, m_pAlgorithm, stKey);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymEncoder::Update(const BYTE* pSrc, size_t tBlockCount, BYTE* pDest)
	{		
		if( 0 == tBlockCount )
			return EC_NO_DATA;

		size_t i;
		for(i=0; i<tBlockCount; i++)
		{
			m_pMode->Encrypt(m_pAlgorithm, pSrc, pDest);
			pSrc += m_tBlockSize;
			pDest += m_tBlockSize;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymEncoder::Final(void)
	{
		return EC_SUCCESS;
	}



	//////////////////////////////////////////////////////////////////////////
	CSymDecoder::CSymDecoder(ISymAlgorithm* pAlgorithm, ISymDecryptMode* pMode)
		: m_pAlgorithm(pAlgorithm)
		, m_pMode(pMode)
		, m_tBlockSize(0)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CSymDecoder::~CSymDecoder(void)
	{
		if( m_pAlgorithm )
			delete m_pAlgorithm;
		if( m_pMode )
			delete m_pMode;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymDecoder::Init(ST_SYM_CIPHER_KEY stKey)
	{
		m_tBlockSize	= m_pAlgorithm->BlockSize();
		return m_pMode->Init(CIPHER_METHOD_DECRYPT, m_pAlgorithm, stKey);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymDecoder::Update(LPCBYTE pSrc, size_t tBlockCount, LPBYTE pDest)
	{
		if( 0 == tBlockCount )
			return EC_NO_DATA;

		size_t i;
		for(i=0; i<tBlockCount; i++)
		{
			m_pMode->Decrypt(m_pAlgorithm, pSrc, pDest);
			pSrc += m_tBlockSize;
			pDest += m_tBlockSize;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymDecoder::Final(void)
	{
		return EC_SUCCESS;
	}
}
