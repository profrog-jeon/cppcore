#include "stdafx.h"
#include "SymCipherFunction.h"

#include "SymCipher.h"

#include "SymPlain.h"
#include "SymSeed128.h"
#include "SymSeed256.h"
#include "SymAES.h"

#include "SymModePlain.h"
#include "SymModeECB.h"
#include "SymModeCBC.h"
#include "SymModePCBC.h"
#include "SymModeOFB.h"
#include "SymModeCFB.h"
#include "SymModeCTR.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	const BYTE g_cSecureKey[32] = 
	{
		0x89, 0x3F, 0xDA, 0x84, 0x07, 0x29, 0x35, 0x6A, 
		0xAF, 0x60, 0x41, 0x73, 0x52, 0x82, 0xF5, 0x48, 
		0x3F, 0xB5, 0xAF, 0x60, 0x65, 0x3C, 0x78, 0x2F, 
		0xB7, 0x48, 0x89, 0x9A, 0xD5, 0x10, 0x0F, 0xAF,
	};

	//////////////////////////////////////////////////////////////////////////
	const BYTE g_cSecureIV[32] = 
	{
		0x29, 0x8F, 0x2A, 0x63, 0x4D, 0xC3, 0xF3, 0x1E,
		0x1B, 0x99, 0xF1, 0xE1, 0xD7, 0x94, 0xAD, 0xE6,
		0x61, 0x69, 0x18, 0x25, 0x64, 0x62, 0x66, 0xD2,
		0xD1, 0x00, 0x20, 0x27, 0x8C, 0xE4, 0x75, 0x63,
	};

	//////////////////////////////////////////////////////////////////////////
	ECODE GenerateCipherKey(E_SYM_CIPHER_TYPE nType, E_SYM_CIPHER_MODE nMode, std::string strKey, OUT ST_SYM_CIPHER_KEY &stCipher)
	{
		BYTE btKey[32], btIV[32];

		{	// Run-time default shading
			size_t i;
			for(i=0; i<32; i+=4)
			{
				btKey[i+3] = g_cSecureKey[i+3] ^ 'T';
				btKey[i+2] = g_cSecureKey[i+2] ^ 'E';
				btKey[i+1] = g_cSecureKey[i+1] ^ 'C';
				btKey[i+0] = g_cSecureKey[i+0] ^ 'H';
				btIV[i+0] = g_cSecureIV[i+0] ^ 'N';
				btIV[i+1] = g_cSecureIV[i+1] ^ 'O';
				btIV[i+2] = g_cSecureIV[i+2] ^ 'L';
				btIV[i+3] = g_cSecureIV[i+3] ^ 'O';
			}
		}

		const size_t tLength = strKey.length();
		if( tLength > 0 )
		{	// Run-time Key shading

			size_t i;
			for(i=0; i<32; i+=4)
			{
				size_t tIndex = i % tLength;
				btKey[31-i] ^= strKey.at(tIndex);
				btIV[31-i] ^= strKey.at(tIndex);
			}
		}
		return GenerateCipherKey(nType, nMode, btKey, btIV, stCipher);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE GenerateCipherKey(E_SYM_CIPHER_TYPE nType, E_SYM_CIPHER_MODE nMode, const BYTE pKey[], const BYTE pIV[], OUT ST_SYM_CIPHER_KEY &stCipher)
	{
		stCipher.nType = nType;
		stCipher.nMode = nMode;
		stCipher.dwBlockSize = 16;
		memcpy(stCipher.btInitialVector, pIV, 16);
		switch(nType)
		{
		case SYM_CIPHER_TYPE_SEED128:
			memcpy(stCipher.btKey, pKey, 16);
			stCipher.dwBlockSize = 16;
			break;

		case SYM_CIPHER_TYPE_SEED256:
			memcpy(stCipher.btKey, pKey, 32);
			stCipher.dwBlockSize = 16;
			break;

		case SYM_CIPHER_TYPE_AES128:
			memcpy(stCipher.btKey, pKey, 16);
			stCipher.dwBlockSize = 16;
			break;

		case SYM_CIPHER_TYPE_AES192:
			memcpy(stCipher.btKey, pKey, 24);
			stCipher.dwBlockSize = 16;
			break;

		case SYM_CIPHER_TYPE_AES256:
			memcpy(stCipher.btKey, pKey, 32);
			stCipher.dwBlockSize = 16;
			break;
                
        default:
            return EC_NOT_SUPPORTED;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE InitSymCipher(const ST_SYM_CIPHER_KEY& stCipherKey, E_CIPHER_METHOD nMethod)
	{
		ISymAlgorithm* pAlgorithm = NULL;
		ISymMode* pMode = NULL;
		ISymCipher* pCipher = NULL;
		try
		{
			switch(stCipherKey.nType)
			{
			case SYM_CIPHER_TYPE_PLAIN:		pAlgorithm = new CSymPlain();	break;
			case SYM_CIPHER_TYPE_SEED128:	pAlgorithm = new CSymSeed128();	break;
			case SYM_CIPHER_TYPE_SEED256:	pAlgorithm = new CSymSeed256();	break;
			case SYM_CIPHER_TYPE_AES128:	pAlgorithm = new CSymAES128();	break;
			case SYM_CIPHER_TYPE_AES192:	pAlgorithm = new CSymAES192();	break;
			case SYM_CIPHER_TYPE_AES256:	pAlgorithm = new CSymAES256();	break;
			default:
				throw std::runtime_error("Invalid algorithm type.");
			}

			switch(stCipherKey.nMode)
			{
			case SYM_CIPHER_MODE_PLAIN:	pMode = new CSymModePlain();	break;
			case SYM_CIPHER_MODE_ECB:	pMode = new CSymModeECB();	break;
			case SYM_CIPHER_MODE_CBC:	pMode = new CSymModeCBC();	break;
			case SYM_CIPHER_MODE_PCBC:	pMode = new CSymModePCBC();	break;
			case SYM_CIPHER_MODE_CFB:	pMode = new CSymModeCFB();	break;
			case SYM_CIPHER_MODE_OFB:	pMode = new CSymModeOFB();	break;
			case SYM_CIPHER_MODE_CTR:	pMode = new CSymModeCTR();	break;
			default:
				throw std::runtime_error("Invalid cipher mode.");
			}

			if( CIPHER_METHOD_ENCRYPT == nMethod )
				pCipher = new CSymEncoder(pAlgorithm, pMode);

			if( CIPHER_METHOD_DECRYPT == nMethod )
				pCipher = new CSymDecoder(pAlgorithm, pMode);

			if( NULL == pCipher )
				throw std::runtime_error("Invalid cipher type.");

			pCipher->Init(stCipherKey);
		}
		catch (std::exception& e)
		{
			printf("%s\n", e.what());
			if( pCipher )
				delete pCipher;
			else
			{
				if( pAlgorithm )
					delete pAlgorithm;
				if( pMode )
					delete pMode;
			}
			return NULL;
		}

		return (HANDLE)pCipher;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE UpdateSymCipher(HANDLE hCipher, const BYTE* pSrc, size_t tBlockCount, BYTE* pDest)
	{	
		if( hCipher == NULL)
			return EC_INVALID_HANDLE;

		ISymCipher* pCipher = (ISymCipher*)hCipher;
		return pCipher->Update(pSrc, tBlockCount, pDest);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE FinalSymCipher(HANDLE hCipher)
	{
		if( hCipher == NULL)
			return EC_INVALID_HANDLE;

		ISymCipher* pCipher = (ISymCipher*)hCipher;
		ECODE nRet = pCipher->Final();
		delete pCipher;
		return nRet;
	}

	//////////////////////////////////////////////////////////////////////////
	inline size_t CommonCipherFunc(E_CIPHER_METHOD nMethod, const ST_SYM_CIPHER_KEY& stCipherKey, LPCBYTE pSrc, size_t tSrcSize, LPBYTE pDest)
	{
		if( 0 == tSrcSize )
			return 0;

		size_t tDestSize = ((tSrcSize - 1U) / stCipherKey.dwBlockSize + 1U) * stCipherKey.dwBlockSize;
		if( pDest )
		{
			HANDLE hCipher = InitSymCipher(stCipherKey, nMethod);
			if( NULL == hCipher )
				return 0;

			// process fully filled block
			size_t tFullyFilledBlockCount = tSrcSize / stCipherKey.dwBlockSize;
			UpdateSymCipher(hCipher, pSrc, tFullyFilledBlockCount, pDest);
			
			// process the last block with padding
			size_t tSrcReadPos = tFullyFilledBlockCount * stCipherKey.dwBlockSize;
			size_t tRemainedSrcSize = tSrcSize - tSrcReadPos;
			if( tRemainedSrcSize )
			{
				const size_t ctTempBuffSize = 256;
				BYTE btTempBuff[ctTempBuffSize] = { 0, };
				memcpy(btTempBuff, pSrc + tSrcReadPos, tRemainedSrcSize);

				UpdateSymCipher(hCipher, btTempBuff, 1, pDest + tSrcReadPos);
			}

			FinalSymCipher(hCipher);
		}

		return tDestSize;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t EncodeData(const ST_SYM_CIPHER_KEY& stCipherKey, LPCBYTE pSrc, size_t tSrcSize, LPBYTE pDest)
	{
		return CommonCipherFunc(CIPHER_METHOD_ENCRYPT, stCipherKey, pSrc, tSrcSize, pDest);
	}

	//////////////////////////////////////////////////////////////////////////
	size_t DecodeData(const ST_SYM_CIPHER_KEY& stCipherKey, LPCBYTE pSrc, size_t tSrcSize, LPBYTE pDest)
	{
		return CommonCipherFunc(CIPHER_METHOD_DECRYPT, stCipherKey, pSrc, tSrcSize, pDest);
	}
}

