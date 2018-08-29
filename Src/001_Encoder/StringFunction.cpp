#include "stdafx.h"
#include "StringFunction.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	static TCHAR g_cHexCharTable[0x10] = { '0', '1', '2', '3', '4', '5', '6', '7', 
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'  };


	//////////////////////////////////////////////////////////////////////////
	static const char g_cDigitTable[256] = 
	{
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* 00-0F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* 10-1F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* 20-2F */
		0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,	/* 30-3F */
		-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* 40-4F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* 50-5F */
		-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* 60-6F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* 70-7F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* 80-8F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* 90-9F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* A0-AF */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* B0-BF */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* C0-CF */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* D0-DF */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* E0-EF */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* F0-FF */
	};

	//////////////////////////////////////////////////////////////////////////
	std::tstring StringFromHex(const unsigned char* pData, size_t tDataSize)
	{
		std::tstring strResult;
		strResult.resize(tDataSize * 2);
		TCHAR* pDest = (TCHAR*)strResult.c_str();

		size_t i;
		for(i=0; i<tDataSize; i++)
		{
			*(pDest++) = g_cHexCharTable[(pData[i]>>4) & 0x0F];
			*(pDest++) = g_cHexCharTable[(pData[i]>>0) & 0x0F];
		}

		return strResult;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring StringFromHex(const unsigned int* pData, size_t tDataSize)
	{
		std::tstring strResult;
		strResult.resize(tDataSize * 8);
		TCHAR* pDest = (TCHAR*)strResult.c_str();

		size_t i;
		for(i=0; i<tDataSize; i++)
		{
			*(pDest++) = g_cHexCharTable[(pData[i]>>28) & 0x0F];
			*(pDest++) = g_cHexCharTable[(pData[i]>>24) & 0x0F];
			*(pDest++) = g_cHexCharTable[(pData[i]>>20) & 0x0F];
			*(pDest++) = g_cHexCharTable[(pData[i]>>16) & 0x0F];
			*(pDest++) = g_cHexCharTable[(pData[i]>>12) & 0x0F];
			*(pDest++) = g_cHexCharTable[(pData[i]>>8) & 0x0F];
			*(pDest++) = g_cHexCharTable[(pData[i]>>4) & 0x0F];
			*(pDest++) = g_cHexCharTable[(pData[i]>>0) & 0x0F];
		}

		return strResult;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE HexFromString(LPBYTE pDest, size_t tDestSize, std::tstring strContext)
	{
		size_t i;
		for(i=0; (i*2+1)<strContext.length() && i<tDestSize; i++)
		{
			DWORD cFront = strContext.at(i*2);
			DWORD cBack  = strContext.at(i*2+1);
			if( cFront > 0xFF )
				return EC_INVALID_DATA;
			if( cBack > 0xFF )
				return EC_INVALID_DATA;

			BYTE btFrontValue = g_cDigitTable[cFront];
			BYTE btBackValue = g_cDigitTable[cBack];
			if( btFrontValue == 0xFF )
				return EC_INVALID_DATA;
			if( btBackValue == 0xFF )
				return EC_INVALID_DATA;

			pDest[i] = (btFrontValue << 4) | (btBackValue << 0);
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	LPCTSTR GetHashTypeString(E_HASH_TYPE dwHashType)
	{
		switch(dwHashType)
		{
			CASE_TO_STR_T(HASH_TYPE_NONE	);
			CASE_TO_STR_T(HASH_TYPE_MD5		);
			CASE_TO_STR_T(HASH_TYPE_SHA1	);
			CASE_TO_STR_T(HASH_TYPE_SHA256	);
			CASE_TO_STR_T(HASH_TYPE_HAS160	);
            default:
                return TEXT("UNDEFINED_HASH_TYPE");
		}
	}

	//////////////////////////////////////////////////////////////////////////
	LPCTSTR GetCipherTypeString(E_SYM_CIPHER_TYPE dwHashType)
	{
		switch(dwHashType)
		{
			CASE_TO_STR_T(SYM_CIPHER_TYPE_PLAIN);
			CASE_TO_STR_T(SYM_CIPHER_TYPE_SEED128);
			CASE_TO_STR_T(SYM_CIPHER_TYPE_SEED256);
			CASE_TO_STR_T(SYM_CIPHER_TYPE_AES128);
			CASE_TO_STR_T(SYM_CIPHER_TYPE_AES192);
			CASE_TO_STR_T(SYM_CIPHER_TYPE_AES256);
            default:
                return TEXT("UNDEFINED_CIPHER_TYPE");
		}
	}

	//////////////////////////////////////////////////////////////////////////
	LPCTSTR GetSymEncModeString(E_SYM_CIPHER_MODE dwEncMode)
	{
		switch(dwEncMode)
		{
			CASE_TO_STR_T(SYM_CIPHER_MODE_PLAIN	);
			CASE_TO_STR_T(SYM_CIPHER_MODE_ECB	);
			CASE_TO_STR_T(SYM_CIPHER_MODE_CBC	);
			CASE_TO_STR_T(SYM_CIPHER_MODE_PCBC	);
			CASE_TO_STR_T(SYM_CIPHER_MODE_CFB	);
			CASE_TO_STR_T(SYM_CIPHER_MODE_OFB	);
			CASE_TO_STR_T(SYM_CIPHER_MODE_CTR	);
            default:
                return TEXT("UNDEFINED_CIPHER_TYPE");
		}
	}

	//////////////////////////////////////////////////////////////////////////
	LPCTSTR GetPubKeyCipherTypeString(E_PUBKEY_CIPHER_TYPE dwHashType)
	{
		switch(dwHashType)
		{
			CASE_TO_STR_T(PUBKEY_CIPHER_TYPE_RSA);
            default:
                return TEXT("UNDEFINED_ASYM_CIPHER_TYPE");
		}
	}
}

