#pragma once

// for compatiblity,  ANSI -> ANSI
#define CalcUnicodeCchFromANSI		CalcUnicodeCchFromANSI
#define ANSI_TO_ANSI				ANSI_TO_ANSI
#define ANSI_TO_UTF16				ANSI_TO_UTF16
#define UTF16_TO_ANSI				UTF16_TO_ANSI
#define ANSI_TO_UTF32				ANSI_TO_UTF32
#define UTF32_TO_ANSI				UTF32_TO_ANSI

namespace core
{
	size_t CalcUnicodeCchFromANSI(LPCSTR pSrc, size_t tSrcCch, size_t* ptSrcReadCch);
	bool IsInvalidUTF8(LPCSTR pData, size_t tDataSize);
	bool IsInvalidUTF8(const std::string& strUTF8);
	void MaskUnreadableUTF8(char* pSrc, size_t tSrcSize);
	std::string& MaskUnreadableUTF8(std::string& strUTF8);

	size_t ANSI_TO_ANSI(const char* pSrc, size_t tSrcCch, char* pDest, size_t tDestCch);
	size_t UTF8_TO_UTF8(const char* pSrc, size_t tSrcCch, char* pDest, size_t tDestCch);
	size_t UTF16_TO_UTF16(const WORD* pSrc, size_t tSrcCch, WORD* pDest, size_t tDestCch);
	size_t UTF32_TO_UTF32(const DWORD* pSrc, size_t tSrcCch, DWORD* pDest, size_t tDestCch);

	size_t UTF8_TO_UTF16(const char* pSrc, size_t tSrcCch, WORD* pDest, size_t tDestCch, size_t* ptReadSize = NULL);
	size_t UTF16_TO_UTF8(const WORD* pSrc, size_t tSrcCch, char* pDest, size_t tDestCch);

	size_t ANSI_TO_UTF16(const char* pSrc, size_t tSrcCch, WORD* pDest, size_t tDestCch, size_t* ptReadSize = NULL);
	size_t UTF16_TO_ANSI(const WORD* pSrc, size_t tSrcCch, char* pDest, size_t tDestCch);
	size_t UTF8_TO_UTF32(const char* pSrc, size_t tSrcCch, DWORD* pDest, size_t tDestCch, size_t* ptReadSize = NULL);
	size_t UTF32_TO_UTF8(const DWORD* pSrc, size_t tSrcCch, char* pDest, size_t tDestCch);

	size_t ANSI_TO_UTF32(const char* pSrc, size_t tSrcCch, DWORD* pDest, size_t tDestCch, size_t* ptReadSize = NULL);
	size_t UTF32_TO_ANSI(const DWORD* pSrc, size_t tSrcCch, char* pDest, size_t tDestCch);	
	size_t UTF16_TO_UTF32(const WORD* pSrc, size_t tSrcCch, DWORD* pDest, size_t tDestCch);
	size_t UTF32_TO_UTF16(const DWORD* pSrc, size_t tSrcCch, WORD* pDest, size_t tDestCch);
}
