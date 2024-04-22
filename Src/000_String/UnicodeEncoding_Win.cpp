#include "stdafx.h"
#include <windows.h>
#include "UnicodeEncoding.h"
#include <vector>

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	size_t ANSI_TO_UTF16(LPCSTR pSrc, size_t tSrcCch, WORD* pDest, size_t tDestCch, size_t* ptReadSize)
	{
		try
		{
			if( tSrcCch == 0 )
				return 0;

			size_t tSrcReadCch;
			size_t tReqDestCch = CalcUnicodeCchFromANSI(pSrc, tSrcCch, &tSrcReadCch);

			if( NULL == pDest )
			{
				size_t tDstLen = ::MultiByteToWideChar(CP_ACP, 0, pSrc, (int)tSrcReadCch, NULL, 0);
				if( 0 == tDstLen )
					throw std::runtime_error("MultiByteToWideChar failure, calc requiredSize");
			}
			else
			{
				size_t tDstLen = ::MultiByteToWideChar(CP_ACP, 0, pSrc, (int)tSrcReadCch, (LPWSTR)pDest, (int)tDestCch);
				if( 0 == tDstLen )
					throw std::runtime_error("MultiByteToWideChar failure, memory copy");
			}

			if( ptReadSize )
				*ptReadSize = tSrcReadCch;

			return tReqDestCch;
		}
		catch(std::exception& e)
		{
			ECODE nRet = ::GetLastError();
			printf("Conv failure(MBS -> WCS) - %s, Error:%d\n", e.what(), nRet);
			return 0;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	size_t UTF16_TO_ANSI(const WORD* pSrc, size_t tSrcCch, char* pDest, size_t tDestCch)
	{
		size_t tDstLen = 0;

		try
		{
			if( tSrcCch == 0 )
				return 0;

			if( NULL == pDest )
			{
				tDstLen = ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)pSrc, (int)tSrcCch, NULL, 0, NULL, NULL);
				if( 0 == tDstLen )
					throw std::runtime_error("WideCharToMultiByte failure, calc requiredSize");
			}
			else
			{
				tDstLen = ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)pSrc, (int)tSrcCch, (LPSTR)pDest, (int)tDestCch, NULL, NULL);
				if( 0 == tDstLen )
					throw std::runtime_error("WideCharToMultiByte failure, memory copy");
			}
		}
		catch(std::exception& e)
		{
			ECODE nRet = ::GetLastError();
			printf("Conv failure(WCS -> MBS) - %s, Error:%d\n", e.what(), nRet);
			return 0;
		}

		return tDstLen;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t ANSI_TO_UTF32(const char* pSrc, size_t tSrcCch, DWORD* pDest, size_t tDestCch, size_t* ptReadSize)
	{
		// [TODO] optimization
		size_t tReqCch = ANSI_TO_UTF16(pSrc, tSrcCch, (WORD*)pDest, tDestCch, ptReadSize);
		if( pDest )
		{
			// memory rearrangement
			size_t i = tReqCch;			
			while(i--)
				pDest[i] = ((WORD*)pDest)[i];
		}
		return tReqCch;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t UTF32_TO_ANSI(const DWORD* pSrc, size_t tSrcCch, char* pDest, size_t tDestCch)
	{
		// [TODO] optimization
		std::vector<WORD> vecUTF16;
		vecUTF16.resize(tSrcCch);

		size_t i;
		for(i=0; i<tSrcCch; i++)
			vecUTF16[i] = (WORD)pSrc[i];

		return UTF16_TO_ANSI(&vecUTF16[0], tSrcCch, pDest, tDestCch);
	}
}