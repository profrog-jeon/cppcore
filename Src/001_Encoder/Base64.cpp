#include "stdafx.h"
#include "Base64.h"

namespace core
{
#ifdef UNICODE
#define g_szMimeBase64			g_szMimeBase64W
#define g_szDecodeMimeBase64	g_szDecodeMimeBase64W
#else
#define g_szMimeBase64			g_szMimeBase64A
#define g_szDecodeMimeBase64	g_szDecodeMimeBase64A
#endif

	//////////////////////////////////////////////////////////////////////////	
	/* base64 encoding table */
	static const char g_szMimeBase64[] = {
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
		'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
		'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
		'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
	};

	//////////////////////////////////////////////////////////////////////////	
	/* base64 decoding table */
	static char g_szDecodeMimeBase64[256] = {
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* 00-0F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* 10-1F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,	/* 20-2F */
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1,  0, -1, -1,	/* 30-3F */
		-1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,	/* 40-4F */
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,	/* 50-5F */
		-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,	/* 60-6F */
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,	/* 70-7F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* 80-8F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* 90-9F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* A0-AF */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* B0-BF */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* C0-CF */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* D0-DF */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* E0-EF */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1	/* F0-FF */
	};

	//////////////////////////////////////////////////////////////////////////
	std::tstring EncodeBase64(LPCBYTE pData, size_t tDataLen)
	{
		if( 0 == tDataLen )
			return TEXT("");

		std::tstring strRet;
		strRet.resize((4 * ((tDataLen + 2) / 3)));

		LPTSTR pDest = (LPTSTR)strRet.c_str();

		size_t tWritePos = 0;
		size_t i;
		for(i=0; (i+2)<tDataLen; i+=3)
		{
			BYTE szTempIn[3] = { pData[i], pData[i+1], pData[i+2] };
			BYTE szTempOut[4] = {
				static_cast<BYTE>((szTempIn[0] & 0xFC) >> 2),
				static_cast<BYTE>(((szTempIn[0] & 0x3) << 4) | (szTempIn[1] & 0xF0) >> 4),
				static_cast<BYTE>(((szTempIn[1] & 0xF) << 2) | (szTempIn[2] & 0xC0) >> 6),
				static_cast<BYTE>((szTempIn[2] & 0x3F))	};

			pDest[tWritePos++] = g_szMimeBase64[szTempOut[0]];
			pDest[tWritePos++] = g_szMimeBase64[szTempOut[1]];
			pDest[tWritePos++] = g_szMimeBase64[szTempOut[2]];
			pDest[tWritePos++] = g_szMimeBase64[szTempOut[3]];
		}

		// for padding
		if( i < tDataLen )
		{
			BYTE szTempIn[3] = { pData[i], 0, 0 };
			if( (i+1 < tDataLen) )
				szTempIn[1] = pData[i+1];

			BYTE szTempOut[4] = {
                static_cast<BYTE>((szTempIn[0] & 0xFC) >> 2),
                static_cast<BYTE>(((szTempIn[0] & 0x3) << 4) | (szTempIn[1] & 0xF0) >> 4),
                static_cast<BYTE>(((szTempIn[1] & 0xF) << 2) | (szTempIn[2] & 0xC0) >> 6),
                static_cast<BYTE>((szTempIn[2] & 0x3F))	};

			pDest[tWritePos++] = g_szMimeBase64[szTempOut[0]];
			pDest[tWritePos++] = g_szMimeBase64[szTempOut[1]];
			pDest[tWritePos++] = (i+1 < tDataLen)? g_szMimeBase64[szTempOut[2]] : '=';
			pDest[tWritePos++] = '=';
		}

		strRet.resize(tWritePos);
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t DecodeBase64(std::tstring strBase64, LPBYTE pDest)
	{
		size_t tBase64Length = strBase64.length();
		if( tBase64Length < 2 )
			return 0;

		size_t tReqSize = (tBase64Length / 4) * 3;
		if( '=' == strBase64.at(tBase64Length-1) )	tReqSize--;	// padding
		if( '=' == strBase64.at(tBase64Length-2) )	tReqSize--;	// padding
		if( NULL == pDest )
			return tReqSize;

		size_t tWrittenPos = 0;
		size_t i;
		for(i=0; (i+3)<strBase64.length(); i+=4)
		{
			char btMimeData[4] = {
				g_szDecodeMimeBase64[strBase64.at(i) & 0xFF],
				g_szDecodeMimeBase64[strBase64.at(i+1) & 0xFF],
				g_szDecodeMimeBase64[strBase64.at(i+2) & 0xFF],
				g_szDecodeMimeBase64[strBase64.at(i+3) & 0xFF]	};

			// exception: invalid character has found
			if( btMimeData[0] < 0 || btMimeData[1] < 0 || btMimeData[2] < 0 || btMimeData[3] < 0 )
				return 0;

			if( (tWrittenPos+2) < tReqSize )
			{
				pDest[tWrittenPos++] = (btMimeData[0] << 2) | (btMimeData[1] >> 4);
				pDest[tWrittenPos++] = (btMimeData[1] << 4) | (btMimeData[2] >> 2);
				pDest[tWrittenPos++] = (btMimeData[2] << 6) | (btMimeData[3] << 0);
			}
			else if( (tWrittenPos+1) < tReqSize )
			{
				pDest[tWrittenPos++] = (btMimeData[0] << 2) | (btMimeData[1] >> 4);
				pDest[tWrittenPos++] = (btMimeData[1] << 4) | (btMimeData[2] >> 2);
			}
			else if( tWrittenPos < tReqSize )
			{
				pDest[tWrittenPos++] = (btMimeData[0] << 2) | (btMimeData[1] >> 4);
			}
		}
		return tWrittenPos;
	}
}
