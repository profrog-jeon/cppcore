#include "stdafx.h"
#include "UnicodeEncoding.h"
#include "Utility.h"

namespace core
{
	template<typename T>
	inline T min(T a, T b)
	{
		return a < b? a : b;
	}

	template<typename T>
	inline T max(T a, T b)
	{
		return a > b? a : b;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CalcUnicodeCchFromANSI(LPCSTR pSrc, size_t tSrcCch, size_t* ptSrcReadCch)
	{
		size_t tDestLen = 0;
		size_t tPos = 0;
		while(tPos < tSrcCch)
		{
			size_t tPrePos = tPos;
			if( pSrc[tPos] < 0 )
				tPos += 2;
			else
				tPos++;

			if (tPos > tSrcCch)
			{
				tPos = tPrePos;
				break;
			}

			tDestLen++;
		}

		if( ptSrcReadCch )
			*ptSrcReadCch = tPos;
		return tDestLen;
	}

	//////////////////////////////////////////////////////////////////////////
	static inline size_t FindInvalidUTF8Char(LPCSTR pData, size_t tDataSize)
	{
		size_t i = 0;
		while(i < tDataSize)
		{
			size_t tRequiredSize = 0;

			// ANSI byte 
			if (0 == (pData[i+0] & 0x80) && IsReadableChar(pData[i]))
			{
				tRequiredSize = 1;
			}
			else // Start byte for 2byte
			if (((i + 1) < tDataSize) &&
				0xC0 == (pData[i+0] & 0xE0) &&
				0x80 == (pData[i+1] & 0xC0))
			{
				tRequiredSize = 2;
			}
			else // Start byte for 3byte
			if (((i + 2) < tDataSize) &&
				0xE0 == (pData[i+0] & 0xF0) &&
				0x80 == (pData[i+1] & 0xC0) &&
				0x80 == (pData[i+2] & 0xC0))
			{
				tRequiredSize = 3;
			}
			else // Start byte for 4byte
			if (((i + 3) < tDataSize) &&
				0xF0 == (pData[i+0] & 0xF8) &&
				0x80 == (pData[i+1] & 0xC0) &&
				0x80 == (pData[i+2] & 0xC0) &&
				0x80 == (pData[i+3] & 0xC0))
			{
				tRequiredSize = 4;
			}
			else // Start byte for 5byte
			if (((i + 4) < tDataSize) &&
				0xF8 == (pData[i+0] & 0xFC) &&
				0x80 == (pData[i+1] & 0xC0) &&
				0x80 == (pData[i+2] & 0xC0) &&
				0x80 == (pData[i+3] & 0xC0) &&
				0x80 == (pData[i+4] & 0xC0))
			{
				tRequiredSize = 5;
			}
			else // Start byte for 6byte
			if (((i + 5) < tDataSize) &&
				0xFC == (pData[i+0] & 0xFE) &&
				0x80 == (pData[i+1] & 0xC0) &&
				0x80 == (pData[i+2] & 0xC0) &&
				0x80 == (pData[i+3] & 0xC0) &&
				0x80 == (pData[i+4] & 0xC0) &&
				0x80 == (pData[i+5] & 0xC0))
			{
				tRequiredSize = 6;
			}

			if (0 == tRequiredSize)
				return i;

			i += tRequiredSize;
		}
		return std::string::npos;
	}

	//////////////////////////////////////////////////////////////////////////
	bool IsInvalidUTF8(LPCSTR pData, size_t tDataSize)
	{
		size_t tInvalidIndex = FindInvalidUTF8Char(pData, tDataSize);
		return tInvalidIndex < tDataSize;
	}

	//////////////////////////////////////////////////////////////////////////
	bool IsInvalidUTF8(const std::string& strUTF8)
	{
		size_t tInvalidIndex = FindInvalidUTF8Char(strUTF8.c_str(), strUTF8.length());
		return tInvalidIndex < strUTF8.length();
	}

	//////////////////////////////////////////////////////////////////////////
	void MaskUnreadableUTF8(char* pSrc, size_t tSrcSize)	
	{
		size_t tPos = 0;
		while (tPos < tSrcSize)
		{
			tPos = FindInvalidUTF8Char(pSrc, tSrcSize);
			if (std::string::npos == tPos)
				break;

			pSrc[tPos] = '?';
		}
	}

	//////////////////////////////////////////////////////////////////////////
	std::string& MaskUnreadableUTF8(std::string& strUTF8)
	{
		MaskUnreadableUTF8((char*)strUTF8.c_str(), strUTF8.length());
		return strUTF8;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t ANSI_TO_ANSI(const char* pSrc, size_t tSrcCch, char* pDest, size_t tDestCch)
	{
		if( pDest )
			memcpy(pDest, pSrc, min(tSrcCch, tDestCch));
		return tSrcCch;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t UTF8_TO_UTF8(const char* pSrc, size_t tSrcCch, char* pDest, size_t tDestCch)
	{
		if( pDest )
			memcpy(pDest, pSrc, min(tSrcCch, tDestCch));
		return tSrcCch;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t UTF16_TO_UTF16(const WORD* pSrc, size_t tSrcCch, WORD* pDest, size_t tDestCch)
	{
		if( pDest )
			memcpy(pDest, pSrc, min(tSrcCch, tDestCch));
		return tSrcCch;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t UTF32_TO_UTF32(const DWORD* pSrc, size_t tSrcCch, DWORD* pDest, size_t tDestCch)
	{
		if( pDest )
			memcpy(pDest, pSrc, min(tSrcCch, tDestCch));
		return tSrcCch;
	}

	//////////////////////////////////////////////////////////////////////////
	static inline size_t UTF32CHAR_TO_UTF8CHAR(DWORD uc, char* UTF8)
	{
		size_t tRequiredSize = 0;

		if (uc <= 0x7f)
		{
			if( NULL != UTF8 )
			{
				UTF8[0] = (char) uc;
				UTF8[1] = (char) '\0';
			}
			tRequiredSize = 1;
		}
		else if (uc <= 0x7ff)
		{
			if( NULL != UTF8 )
			{
				UTF8[0] = (char)(0xc0 + uc / (0x01 << 6));
				UTF8[1] = (char)(0x80 + uc % (0x01 << 6));
				UTF8[2] = (char) '\0';
			}
			tRequiredSize = 2;
		}
		else if (uc <= 0xffff)
		{
			if( NULL != UTF8 )
			{
				UTF8[0] = (char)(0xe0 + uc / (0x01 <<12));
				UTF8[1] = (char)(0x80 + uc / (0x01 << 6) % (0x01 << 6));
				UTF8[2] = (char)(0x80 + uc % (0x01 << 6));
				UTF8[3] = (char) '\0';
			}
			tRequiredSize = 3;
		}
		else if (uc <= 0x1fffff)
		{
			if( NULL != UTF8 )
			{
				UTF8[0] = (char)(0xf0 + uc / (0x01 <<18));
				UTF8[1] = (char)(0x80 + uc / (0x01 <<12) % (0x01 <<12));
				UTF8[2] = (char)(0x80 + uc / (0x01 << 6) % (0x01 << 6));
				UTF8[3] = (char)(0x80 + uc % (0x01 << 6));
				UTF8[4] = (char) '\0';
			}
			tRequiredSize = 4;
		}
		else if (uc <= 0x3ffffff)
		{
			if( NULL != UTF8 )
			{
				UTF8[0] = (char)(0xf8 + uc / (0x01 <<24));
				UTF8[1] = (char)(0x80 + uc / (0x01 <<18) % (0x01 <<18));
				UTF8[2] = (char)(0x80 + uc / (0x01 <<12) % (0x01 <<12));
				UTF8[3] = (char)(0x80 + uc / (0x01 << 6) % (0x01 << 6));
				UTF8[4] = (char)(0x80 + uc % (0x01 << 6));
				UTF8[5] = (char) '\0';
			}
			tRequiredSize = 5;
		}
		else if (uc <= 0x7fffffff)
		{
			if( NULL != UTF8 )
			{
				UTF8[0] = (char)(0xfc + uc / (0x01 <<30));
				UTF8[1] = (char)(0x80 + uc / (0x01 <<24) % (0x01 <<24));
				UTF8[2] = (char)(0x80 + uc / (0x01 <<18) % (0x01 <<18));
				UTF8[3] = (char)(0x80 + uc / (0x01 <<12) % (0x01 <<12));
				UTF8[4] = (char)(0x80 + uc / (0x01 << 6) % (0x01 << 6));
				UTF8[5] = (char)(0x80 + uc % (0x01 << 6));
				UTF8[6] = (char) '\0';
			}
			tRequiredSize = 6;
		}

		return tRequiredSize;
	}

	//////////////////////////////////////////////////////////////////////////
	static inline size_t UTF8CHAR_TO_UTF32CHAR(const char* UTF8, DWORD& uc)
	{
		size_t tRequiredSize = 0;

		uc = 0x00000000;

		// ANSI byte 
		if( 0 == (UTF8[0] & 0x80) )
		{
			uc = UTF8[0];
			tRequiredSize = 1;
		}
		else // Start byte for 2byte
		if( 0xC0 == (UTF8[0] & 0xE0) &&
			0x80 == (UTF8[1] & 0xC0) )
		{
			uc += (UTF8[0] & 0x1F) << 6;
			uc += (UTF8[1] & 0x3F) << 0;
			tRequiredSize = 2;
		}
		else // Start byte for 3byte
		if( 0xE0 == (UTF8[0] & 0xF0) &&
			0x80 == (UTF8[1] & 0xC0) &&
			0x80 == (UTF8[2] & 0xC0) )
		{
			uc += (UTF8[0] & 0x0F) << 12;
			uc += (UTF8[1] & 0x3F) << 6;
			uc += (UTF8[2] & 0x3F) << 0;
			tRequiredSize = 3;
		}
		else // Start byte for 4byte
		if( 0xF0 == (UTF8[0] & 0xF8) &&
			0x80 == (UTF8[1] & 0xC0) &&
			0x80 == (UTF8[2] & 0xC0) &&
			0x80 == (UTF8[3] & 0xC0) )
		{
			uc += (UTF8[0] & 0x07) << 18;
			uc += (UTF8[1] & 0xfF) << 12;
			uc += (UTF8[2] & 0x3F) << 6;
			uc += (UTF8[3] & 0x3F) << 0;
			tRequiredSize = 4;
		}
		else // Start byte for 5byte
		if( 0xF8 == (UTF8[0] & 0xFC) &&
			0x80 == (UTF8[1] & 0xC0) &&
			0x80 == (UTF8[2] & 0xC0) &&
			0x80 == (UTF8[3] & 0xC0) &&
			0x80 == (UTF8[4] & 0xC0) )
		{
			uc += (UTF8[0] & 0x03) << 24;
			uc += (UTF8[1] & 0x3F) << 18;
			uc += (UTF8[2] & 0x3F) << 12;
			uc += (UTF8[3] & 0x3F) << 6;
			uc += (UTF8[4] & 0x3F) << 0;
			tRequiredSize = 5;
		}
		else // Start byte for 6byte
		if( 0xFC == (UTF8[0] & 0xFE) &&
			0x80 == (UTF8[1] & 0xC0) &&
			0x80 == (UTF8[2] & 0xC0) &&
			0x80 == (UTF8[3] & 0xC0) &&
			0x80 == (UTF8[4] & 0xC0) &&
			0x80 == (UTF8[5] & 0xC0) )
		{
			uc += (UTF8[0] & 0x01) << 30;
			uc += (UTF8[1] & 0x3F) << 24;
			uc += (UTF8[2] & 0x3F) << 18;
			uc += (UTF8[3] & 0x3F) << 12;
			uc += (UTF8[4] & 0x3F) << 6;
			uc += (UTF8[5] & 0x3F) << 0;
			tRequiredSize = 6;
		}

		return tRequiredSize;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t UTF8_TO_UTF16(const char* pSrc, size_t tSrcCch, WORD* pDest, size_t tDestCch, size_t* ptReadSize)
	{
		size_t tReadPos = 0;

		size_t i;
		for(i=0; (tReadPos<tSrcCch) && pSrc[tReadPos]; i++)
		{
			DWORD tTempUnicodeChar = 0;
			size_t tSize = UTF8CHAR_TO_UTF32CHAR(pSrc + tReadPos, tTempUnicodeChar);

			// Invalid UTF8 character
			if( 0 == tSize )
				break;

			if( pDest && (i < tDestCch) )
				pDest[i] = (WORD)tTempUnicodeChar;

			tReadPos += tSize;
		}
		if( ptReadSize )
			*ptReadSize = tReadPos;
		return i;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t UTF16_TO_UTF8(const WORD* pSrc, size_t tSrcCch, char* pDest, size_t tDestCch)
	{
		size_t tWritePos = 0;

		size_t i;
		for(i=0; i<tSrcCch && pSrc[i]; i++)
		{
			size_t tSize = 0;
			if( pDest && (tWritePos < tDestCch) )
				tSize = UTF32CHAR_TO_UTF8CHAR(pSrc[i], pDest + tWritePos);
			else
				tSize = UTF32CHAR_TO_UTF8CHAR(pSrc[i], NULL);
			tWritePos += tSize;
		}

		return tWritePos;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t UTF8_TO_UTF32(const char* pSrc, size_t tSrcCch, DWORD* pDest, size_t tDestCch, size_t* ptReadSize)
	{
		size_t tReadPos = 0;

		size_t i;
		for(i=0; tReadPos<tSrcCch && pSrc[tReadPos]; i++)
		{
			DWORD tTempUnicodeChar = 0;
			size_t tSize = UTF8CHAR_TO_UTF32CHAR(pSrc + tReadPos, tTempUnicodeChar);

			// Invalid UTF8 character
			if( 0 == tSize )
				break;

			if( pDest && (i < tDestCch) )
				pDest[i] = tTempUnicodeChar;

			tReadPos += tSize;
		}
		if( ptReadSize )
			*ptReadSize = tReadPos;
		return i;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t UTF32_TO_UTF8(const DWORD* pSrc, size_t tSrcCch, char* pDest, size_t tDestCch)
	{
		size_t tWritePos = 0;

		size_t i=0;
		for(i=0; i<tSrcCch && pSrc[i]; i++)
		{
			size_t tSize = 0;
			if( pDest && (tWritePos < tDestCch) )
				tSize = UTF32CHAR_TO_UTF8CHAR(pSrc[i], pDest + tWritePos);
			else
				tSize = UTF32CHAR_TO_UTF8CHAR(pSrc[i], NULL);
			tWritePos += tSize;
		}

		return tWritePos;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t UTF16_TO_UTF32(const WORD* pSrc, size_t tSrcCch, DWORD* pDest, size_t tDestCch)
	{
		if( pDest )
		{
			size_t i;
			for(i=0; i<tSrcCch && i<tDestCch; i++)
				pDest[i] = pSrc[i];
		}

		return tSrcCch;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t UTF32_TO_UTF16(const DWORD* pSrc, size_t tSrcCch, WORD* pDest, size_t tDestCch)
	{
		if( pDest )
		{
			size_t i;
			for(i=0; i<tSrcCch && i<tDestCch; i++)
				pDest[i] = (WORD)pSrc[i];
		}

		return tSrcCch;
	}
}
