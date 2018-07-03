#pragma once

#include <vector>
#include <string>

#ifdef UNICODE
#define TCSFromWCS		WCSFromWCS
#define TCSFromMBS		WCSFromMBS
#define TCSFromEUCKR	WCSFromEUCKR
#define TCSFromUTF8		WCSFromUTF8
#define TCSFromUTF16	WCSFromUTF16
#define TCSFromUTF32	WCSFromUTF32
#define NTCSFromWCS		MBSFromWCS
#define NTCSFromMBS		MBSFromMBS
#define NTCSFromEUCKR	MBSFromEUCKR
#define NTCSFromUTF8	MBSFromUTF8
#define NTCSFromUTF16	MBSFromUTF16
#define NTCSFromUTF32	MBSFromUTF32

#define WCSFromTCS		WCSFromWCS	
#define MBSFromTCS		MBSFromWCS
#define EUCKRFromTCS	EUCKRFromWCS
#define UTF8FromTCS		UTF8FromWCS
#define UTF16FromTCS	UTF16FromWCS
#define UTF32FromTCS	UTF32FromWCS
#define WCSFromNTCS		WCSFromMBS
#define MBSFromNTCS		MBSFromMBS
#define EUCKRFromNTCS	EUCKRFromMBS
#define UTF8FromNTCS	UTF8FromMBS
#define UTF16FromNTCS	UTF16FromMBS
#define UTF32FromNTCS	UTF32FromMBS

#define NTCSFromTCS		MBSFromWCS
#define TCSFromNTCS		WCSFromMBS
#else
#define TCSFromWCS		MBSFromWCS		
#define TCSFromMBS		MBSFromMBS		
#define TCSFromEUCKR	MBSFromEUCKR	
#define TCSFromUTF8		MBSFromUTF8		
#define TCSFromUTF16	MBSFromUTF16	
#define TCSFromUTF32	MBSFromUTF32	
#define NTCSFromWCS		WCSFromWCS		
#define NTCSFromMBS		WCSFromMBS		
#define NTCSFromEUCKR	WCSFromEUCKR	
#define NTCSFromUTF8	WCSFromUTF8	
#define NTCSFromUTF16	WCSFromUTF16	
#define NTCSFromUTF32	WCSFromUTF32	

#define WCSFromTCS		WCSFromMBS		
#define MBSFromTCS		MBSFromMBS		
#define EUCKRFromTCS	EUCKRFromMBS	
#define UTF8FromTCS		UTF8FromMBS		
#define UTF16FromTCS	UTF16FromMBS	
#define UTF32FromTCS	UTF32FromMBS	
#define WCSFromNTCS		WCSFromWCS		
#define MBSFromNTCS		MBSFromWCS		
#define EUCKRFromNTCS	EUCKRFromWCS	
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

	std::wstring	WCSFromUTF8(std::string strInput, size_t* ptReadSize = NULL);
	std::wstring	WCSFromUTF16(const WORD* pszInput, size_t tInputCch);
	std::wstring	WCSFromUTF32(const DWORD* pszInput, size_t tInputCch);
	std::wstring	WCSFromEUCKR(std::string strInput, size_t* ptReadSize = NULL);

	std::string		MBSFromUTF8(std::string strInput, size_t* ptReadSize = NULL);
	std::string		MBSFromUTF16(const WORD* pszInput, size_t tInputCch);
	std::string		MBSFromUTF32(const DWORD* pszInput, size_t tInputCch);
	std::string		MBSFromEUCKR(std::string strInput, size_t* ptReadSize = NULL);

	std::string		EUCKRFromMBS(std::string strInput);
	std::string		EUCKRFromWCS(std::wstring strInput);

	std::string		EUCKRFromUTF8(std::string strInput, size_t* ptReadSize = NULL);
	std::string		UTF8FromEUCKR(std::string strInput, size_t* ptReadSize = NULL);

	std::vector<WORD>	UTF16FromMBS(std::string strInput);
	std::vector<WORD>	UTF16FromWCS(std::wstring strInput);
	std::vector<DWORD>	UTF32FromMBS(std::string strInput);
	std::vector<DWORD>	UTF32FromWCS(std::wstring strInput);
}

