#pragma once

#include <vector>
#include <string>

// for compatiblity,  ASCII -> ANSI
#define WCSFromASCII				WCSFromANSI
#define MBSFromASCII				MBSFromANSI
#define UTF8FromASCII				UTF8FromANSI
#define ASCIIFromWCS				ANSIFromWCS
#define ASCIIFromMBS				ANSIFromMBS
#define ASCIIFromUTF8				ANSIFromUTF8
#define TCSFromASCII				TCSFromANSI
#define NTCSFromASCII				NTCSFromANSI
#define ASCIIFromTCS				ANSIFromTCS
#define ASCIIFromNTCS				ANSIFromNTCS

#ifdef UNICODE
#define TCSFromWCS		WCSFromWCS
#define TCSFromMBS		WCSFromMBS
#define TCSFromANSI		WCSFromANSI
#define TCSFromUTF8		WCSFromUTF8
#define TCSFromUTF16	WCSFromUTF16
#define TCSFromUTF32	WCSFromUTF32
#define NTCSFromWCS		MBSFromWCS
#define NTCSFromMBS		MBSFromMBS
#define NTCSFromANSI	MBSFromANSI
#define NTCSFromUTF8	MBSFromUTF8
#define NTCSFromUTF16	MBSFromUTF16
#define NTCSFromUTF32	MBSFromUTF32

#define WCSFromTCS		WCSFromWCS	
#define MBSFromTCS		MBSFromWCS
#define ANSIFromTCS		ANSIFromWCS
#define UTF8FromTCS		UTF8FromWCS
#define UTF16FromTCS	UTF16FromWCS
#define UTF32FromTCS	UTF32FromWCS
#define WCSFromNTCS		WCSFromMBS
#define MBSFromNTCS		MBSFromMBS
#define ANSIFromNTCS	ANSIFromMBS
#define UTF8FromNTCS	UTF8FromMBS
#define UTF16FromNTCS	UTF16FromMBS
#define UTF32FromNTCS	UTF32FromMBS

#define NTCSFromTCS		MBSFromWCS
#define TCSFromNTCS		WCSFromMBS
#else
#define TCSFromWCS		MBSFromWCS		
#define TCSFromMBS		MBSFromMBS		
#define TCSFromANSI		MBSFromANSI	
#define TCSFromUTF8		MBSFromUTF8		
#define TCSFromUTF16	MBSFromUTF16	
#define TCSFromUTF32	MBSFromUTF32	
#define NTCSFromWCS		WCSFromWCS		
#define NTCSFromMBS		WCSFromMBS		
#define NTCSFromANSI	WCSFromANSI	
#define NTCSFromUTF8	WCSFromUTF8	
#define NTCSFromUTF16	WCSFromUTF16	
#define NTCSFromUTF32	WCSFromUTF32	

#define WCSFromTCS		WCSFromMBS		
#define MBSFromTCS		MBSFromMBS		
#define ANSIFromTCS		ANSIFromMBS	
#define UTF8FromTCS		UTF8FromMBS		
#define UTF16FromTCS	UTF16FromMBS	
#define UTF32FromTCS	UTF32FromMBS	
#define WCSFromNTCS		WCSFromWCS		
#define MBSFromNTCS		MBSFromWCS		
#define ANSIFromNTCS	ANSIFromWCS	
#define UTF8FromNTCS	UTF8FromWCS	
#define UTF16FromNTCS	UTF16FromWCS	
#define UTF32FromNTCS	UTF32FromWCS	

#define NTCSFromTCS		WCSFromMBS		
#define TCSFromNTCS		MBSFromWCS		
#endif

namespace core
{
	std::wstring	WCSFromWCS(std::wstring strInput);
	std::string		MBSFromMBS(std::string strInput);

	std::wstring	WCSFromMBS(std::string strInput);
	std::string		MBSFromWCS(std::wstring strInput);
	std::string		UTF8FromWCS(std::wstring strInput);
	std::string		UTF8FromMBS(std::string strInput);	

	std::wstring	WCSFromUTF8(LPCSTR pszContext, size_t tLength, size_t* ptReadSize = NULL);
	std::wstring	WCSFromUTF8(std::string strInput, size_t* ptReadSize = NULL);
	std::wstring	WCSFromUTF16(const WORD* pszInput, size_t tInputCch);
	std::wstring	WCSFromUTF32(const DWORD* pszInput, size_t tInputCch);
	std::wstring	WCSFromANSI(LPCSTR pszContext, size_t tLength, size_t* ptReadSize = NULL);
	std::wstring	WCSFromANSI(std::string strInput, size_t* ptReadSize = NULL);

	std::string		MBSFromUTF8(LPCSTR pszContext, size_t tLength, size_t* ptReadSize = NULL);
	std::string		MBSFromUTF8(std::string strInput, size_t* ptReadSize = NULL);
	std::string		MBSFromUTF16(const WORD* pszInput, size_t tInputCch);
	std::string		MBSFromUTF32(const DWORD* pszInput, size_t tInputCch);
	std::string		MBSFromANSI(LPCSTR pszContext, size_t tLength, size_t* ptReadSize = NULL);
	std::string		MBSFromANSI(std::string strInput, size_t* ptReadSize = NULL);

	std::string		ANSIFromMBS(std::string strInput);
	std::string		ANSIFromWCS(std::wstring strInput);

	std::string		ANSIFromUTF8(std::string strInput, size_t* ptReadSize = NULL);
	std::string		UTF8FromANSI(std::string strInput, size_t* ptReadSize = NULL);

	std::vector<WORD>	UTF16FromMBS(std::string strInput);
	std::vector<WORD>	UTF16FromWCS(std::wstring strInput);
	std::vector<DWORD>	UTF32FromMBS(std::string strInput);
	std::vector<DWORD>	UTF32FromWCS(std::wstring strInput);
}

