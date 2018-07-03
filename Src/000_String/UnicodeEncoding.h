#pragma once

namespace core
{
	size_t CalcUnicodeCchFromEUCKR(LPCSTR pSrc, size_t tSrcCch, size_t* ptSrcReadCch);

	size_t EUCKR_TO_EUCKR(const char* pSrc, size_t tSrcCch, char* pDest, size_t tDestCch);
	size_t UTF8_TO_UTF8(const char* pSrc, size_t tSrcCch, char* pDest, size_t tDestCch);
	size_t UTF16_TO_UTF16(const WORD* pSrc, size_t tSrcCch, WORD* pDest, size_t tDestCch);
	size_t UTF32_TO_UTF32(const DWORD* pSrc, size_t tSrcCch, DWORD* pDest, size_t tDestCch);

	size_t UTF8_TO_UTF16(const char* pSrc, size_t tSrcCch, WORD* pDest, size_t tDestCch, size_t* ptReadSize = NULL);
	size_t UTF16_TO_UTF8(const WORD* pSrc, size_t tSrcCch, char* pDest, size_t tDestCch);

	size_t EUCKR_TO_UTF16(const char* pSrc, size_t tSrcCch, WORD* pDest, size_t tDestCch, size_t* ptReadSize = NULL);
	size_t UTF16_TO_EUCKR(const WORD* pSrc, size_t tSrcCch, char* pDest, size_t tDestCch);
	size_t UTF8_TO_UTF32(const char* pSrc, size_t tSrcCch, DWORD* pDest, size_t tDestCch, size_t* ptReadSize = NULL);
	size_t UTF32_TO_UTF8(const DWORD* pSrc, size_t tSrcCch, char* pDest, size_t tDestCch);

	size_t EUCKR_TO_UTF32(const char* pSrc, size_t tSrcCch, DWORD* pDest, size_t tDestCch, size_t* ptReadSize = NULL);
	size_t UTF32_TO_EUCKR(const DWORD* pSrc, size_t tSrcCch, char* pDest, size_t tDestCch);	
	size_t UTF16_TO_UTF32(const WORD* pSrc, size_t tSrcCch, DWORD* pDest, size_t tDestCch);
	size_t UTF32_TO_UTF16(const DWORD* pSrc, size_t tSrcCch, WORD* pDest, size_t tDestCch);
}
