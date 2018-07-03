#include "stdafx.h"
#include "SymAES.h"
#include "AES/aes.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CSymAES::CSymAES(size_t tLengthOfKey)
		: m_pCtx(NULL)
		, m_tLengthOfKey(tLengthOfKey)
	{
		m_pCtx = new(std::nothrow) aes_ctx;
	}

	//////////////////////////////////////////////////////////////////////////
	CSymAES::~CSymAES(void)
	{
		if( m_pCtx )
			delete (aes_ctx*)m_pCtx;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymAES::Init(E_CIPHER_METHOD nMethod, const ST_SYM_CIPHER_KEY& stKey)
	{
		aes_ctx* pCtx = (aes_ctx*)m_pCtx;
		pCtx->n_rnd = 0;
		pCtx->n_blk = (aes_32t)BlockSize();
		switch(nMethod)
		{
		case CIPHER_METHOD_ENCRYPT:
			aes_enc_key(stKey.btKey, m_tLengthOfKey, pCtx);
			break;
		case CIPHER_METHOD_DECRYPT:
			aes_dec_key(stKey.btKey, m_tLengthOfKey, pCtx);
			break;
		default:
			return EC_INVALID_ARGUMENT;
		}
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymAES::Encrypt(LPCBYTE pSrc, LPBYTE pDest)
	{
		if( aes_good != aes_enc_blk(pSrc, pDest, (aes_ctx*)m_pCtx) )
			return EC_INVALID_FUNCTION;
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSymAES::Decrypt(LPCBYTE pSrc, LPBYTE pDest)
	{
		if( aes_good != aes_dec_blk(pSrc, pDest, (aes_ctx*)m_pCtx) )
			return EC_INVALID_FUNCTION;
		return EC_SUCCESS;
	}
}

