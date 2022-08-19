#include "stdafx.h"
#include "UnicodeEncoding.h"
#include "BOM.h"
#include <vector>
#include <iconv.h>
#include <errno.h>
#include <stdexcept>

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	inline E_BOM_TYPE ReadAndStripBOM(iconv_t hICONV, char** ppSrcPos, size_t* ptSrcLeft, char** ppDestPos, size_t* ptDestLeft)
	{
		const size_t tTempBOMSize = 8;
		char szBOMBuff[tTempBOMSize] = { 0, };

		char* pTempSrcPos = *ppSrcPos;
		size_t tTempSrcLeft = *ptSrcLeft;
		char* pszTempDestPos = szBOMBuff;
		size_t tTempDestLeft = tTempBOMSize;
		int nRet = (int)iconv(hICONV, &pTempSrcPos, &tTempSrcLeft, &pszTempDestPos, &tTempDestLeft);
		if( tTempBOMSize == tTempDestLeft )
			return BOM_UNDEFINED;

		size_t tTempReadLength = tTempBOMSize - tTempDestLeft;

		ST_BOM_INFO stBOMInfo;
		E_BOM_TYPE nBOMType = ReadBOM((unsigned char*)szBOMBuff, tTempReadLength, stBOMInfo);
		if( BOM_UNDEFINED == nBOMType )
			return BOM_UNDEFINED;

		size_t tSrcReadSize = (*ptSrcLeft - tTempSrcLeft);
		*ptSrcLeft -= tSrcReadSize;
		*ppSrcPos += tSrcReadSize;

		size_t tDestWrittenSize = tTempReadLength - stBOMInfo.tSize;
		memcpy(*ppDestPos, szBOMBuff + stBOMInfo.tSize, tDestWrittenSize);
		*ptDestLeft -= tDestWrittenSize;
		*ppDestPos += tDestWrittenSize;
		return nBOMType;
	}

	//////////////////////////////////////////////////////////////////////////
	static inline E_BOM_TYPE IconvWorker(const char* pszFrom, const char* pszTo, const char* pszSrc, size_t tSrcSize, char* pszDest, size_t tDestSize, size_t& tRefDestWrittenSize, size_t* ptSrcReadSize)
	{
		E_BOM_TYPE nRetType = BOM_UNDEFINED;
		iconv_t hICONV = iconv_open(pszTo, pszFrom);

		try
		{
			if( (iconv_t)-1 == hICONV )
				throw std::runtime_error("iconv_open failure");

			char* pSrcPos = (char*)pszSrc;
			size_t tSrcLeft = tSrcSize;

			char* pDestPos = pszDest;
			size_t tDestLeft = tDestSize;
			nRetType = ReadAndStripBOM(hICONV, &pSrcPos, &tSrcLeft, &pDestPos, &tDestLeft);

			int nRet = (int)::iconv(hICONV, &pSrcPos, &tSrcLeft, &pDestPos, &tDestLeft);
			if( tSrcLeft == tSrcSize )
				throw std::runtime_error("iconv operation failure");

			iconv_close(hICONV);
			tRefDestWrittenSize = tDestSize - tDestLeft;
			if( ptSrcReadSize )
				*ptSrcReadSize = tSrcSize - tSrcLeft;
			return nRetType;
		}
		catch(std::exception& e)
		{
			printf("%s, err:%d\n", e.what(), errno);
			if( (iconv_t)-1 != hICONV )
                ::iconv_close(hICONV);
			return nRetType;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	static inline int CalcDestMemorySize(const char* pszFrom, const char* pszTo, const char* pszSrc, size_t tSrcSize, size_t& tRefDestSize)
	{
		tRefDestSize = 0;
		iconv_t hICONV = iconv_open(pszTo, pszFrom);

		try
		{
			if( (iconv_t)-1 == hICONV )
				throw std::runtime_error("iconv_open failure");

			char* pSrcPos = (char*)pszSrc;
			size_t tSrcLeft = tSrcSize;

			// temporary buffer
			const size_t tBuffSize = 128;
			BYTE btBuff[tBuffSize];

			// set initial dest point
			char* pDestPos = (char*)btBuff;
			size_t tDestLeft = tBuffSize;

			int nRet = 0;
			do
			{
				nRet = (int)iconv(hICONV, &pSrcPos, &tSrcLeft, &pDestPos, &tDestLeft);

				size_t tWrittenSize = tBuffSize - tDestLeft;

				// check the first written data has BOM
				if( (0 == tRefDestSize) && (tWrittenSize >= 4) )
				{
					ST_BOM_INFO stBOM = { 0, };
					if( BOM_UNDEFINED != ReadBOM(btBuff, 4, stBOM) )
						tWrittenSize -= stBOM.tSize;
				}
				tRefDestSize += tWrittenSize;

				// reset dest buffer
				pDestPos = (char*)btBuff;
				tDestLeft = tBuffSize;
			}	while((nRet < 0) && (errno == E2BIG));

			iconv_close(hICONV);
			return nRet;
		}
		catch(std::exception& e)
		{
			printf("%s, err:%d\n", e.what(), errno);
			if( (iconv_t)-1 != hICONV )
				iconv_close(hICONV);
			return -1;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	size_t ANSI_TO_UTF16(const char* pSrc, size_t tSrcCch, WORD* pDest, size_t tDestCch, size_t* ptReadSize)
	{
		const size_t tCharSize = sizeof(WORD);
		size_t tRet = 0;
		size_t tSrcSize = tSrcCch;
		size_t tDestSize = tDestCch * tCharSize;

		if( NULL == pDest )
			return CalcUnicodeCchFromANSI(pSrc, tSrcSize, ptReadSize);

		E_BOM_TYPE nRetType = IconvWorker("CP949", "UTF-16", pSrc, tSrcSize, (char*)pDest, tDestSize, tRet, ptReadSize);
		if( BOM_UTF16_BE == nRetType )
		{
			size_t i;
			for(i=0; i<tDestCch; i++)
			{
				WORD dwTemp = pDest[i];
				pDest[i] =((dwTemp & 0xFF00) >> 8) | ((dwTemp & 0x00FF) << 8);
			}
		}

		return tRet / tCharSize;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t UTF16_TO_ANSI(const WORD* pSrc, size_t tSrcCch, char* pDest, size_t tDestCch)
	{
		const size_t tCharSize = sizeof(char);
		size_t tRet = 0;
		size_t tSrcSize = tSrcCch * sizeof(pSrc[0]);
		size_t tDestSize = tDestCch * tCharSize;

		if( pDest )
		{
			IconvWorker("UTF-16LE", "CP949", (const char*)pSrc, tSrcSize, (char*)pDest, tDestSize, tRet, NULL);
		}
		else
		{
			CalcDestMemorySize("UTF-16LE", "CP949", (const char*)pSrc, tSrcSize, tRet);
		}

		return tRet / tCharSize;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t ANSI_TO_UTF32(const char* pSrc, size_t tSrcCch, DWORD* pDest, size_t tDestCch, size_t* ptReadSize)
	{
		const size_t tCharSize = sizeof(DWORD);
		size_t tRet = 0;
		size_t tSrcSize = tSrcCch;
		size_t tDestSize = tDestCch * tCharSize;

		if( pDest )
		{
			E_BOM_TYPE nRetType = IconvWorker("CP949", "UTF-32", pSrc, tSrcSize, (char*)pDest, tDestSize, tRet, ptReadSize);
            if( BOM_UTF32_BE == nRetType )
            {
                size_t i;
                for(i=0; i<tDestCch; i++)
                {
                    DWORD dwTemp = pDest[i];
                    pDest[i] =((dwTemp & 0xFF000000) >> 24)
                            | ((dwTemp & 0x00FF0000) >> 8)
                            | ((dwTemp & 0x0000FF00) << 8)
                            | ((dwTemp & 0x000000FF) << 24);
                }
            }
		}
		else
		{
			CalcDestMemorySize("CP949", "UTF-32", pSrc, tSrcSize, tRet);
		}

		return tRet / tCharSize;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t UTF32_TO_ANSI(const DWORD* pSrc, size_t tSrcCch, char* pDest, size_t tDestCch)
	{
		const size_t tCharSize = sizeof(char);
		size_t tRet = 0;
		size_t tSrcSize = tSrcCch * sizeof(pSrc[0]);
		size_t tDestSize = tDestCch * tCharSize;

		if( pDest )
		{
			IconvWorker("UTF-32", "CP949", (const char*)pSrc, tSrcSize, (char*)pDest, tDestSize, tRet, NULL);
		}
		else
		{
			CalcDestMemorySize("UTF-32", "CP949", (const char*)pSrc, tSrcSize, tRet);
		}

		return tRet / tCharSize;
	}
}
