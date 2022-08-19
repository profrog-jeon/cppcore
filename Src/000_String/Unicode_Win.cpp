#include "stdafx.h"
#include "Unicode.h"
#include "UnicodeEncoding.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	std::wstring WCSFromMBS(std::string strInput)
	{
		std::wstring strRet;
		size_t tRequiredCch = ANSI_TO_UTF16(strInput.c_str(), strInput.length(), NULL, 0, NULL);
		strRet.resize(tRequiredCch);
		ANSI_TO_UTF16(strInput.c_str(), strInput.length(), (WORD*)strRet.c_str(), tRequiredCch, NULL);
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	std::string MBSFromWCS(std::wstring strInput)
	{
		std::string strRet;
		size_t tRequiredCch = UTF16_TO_ANSI((const WORD*)strInput.c_str(), strInput.length(), NULL, 0);
		strRet.resize(tRequiredCch);
		UTF16_TO_ANSI((const WORD*)strInput.c_str(), strInput.length(), (char*)strRet.c_str(), tRequiredCch);
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	std::string UTF8FromWCS(std::wstring strInput)
	{
		std::string strRet;
		size_t tRequiredCch = UTF16_TO_UTF8((const WORD*)strInput.c_str(), strInput.length(), NULL, 0);
		strRet.resize(tRequiredCch);
		UTF16_TO_UTF8((const WORD*)strInput.c_str(), strInput.length(), (char*)strRet.c_str(), tRequiredCch);
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	std::wstring WCSFromUTF8(LPCSTR pszContext, size_t tLength, size_t* ptReadSize)
	{
		std::wstring strRet;
		size_t tRequiredCch = UTF8_TO_UTF16(pszContext, tLength, NULL, 0, NULL);
		strRet.resize(tRequiredCch);
		UTF8_TO_UTF16(pszContext, tLength, (WORD*)strRet.c_str(), tRequiredCch, ptReadSize);
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	std::wstring WCSFromUTF8(std::string strInput, size_t* ptReadSize)
	{
		std::wstring strRet;
		size_t tRequiredCch = UTF8_TO_UTF16(strInput.c_str(), strInput.length(), NULL, 0, NULL);
		strRet.resize(tRequiredCch);
		UTF8_TO_UTF16(strInput.c_str(), strInput.length(), (WORD*)strRet.c_str(), tRequiredCch, ptReadSize);
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	std::wstring WCSFromUTF16(const WORD* pszInput, size_t tInputCch)
	{
		std::wstring strRet;
		strRet.resize(tInputCch);
		memcpy((void*)strRet.c_str(), pszInput, tInputCch * 2);
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	std::wstring WCSFromUTF32(const DWORD* pszInput, size_t tInputCch)
	{
		std::wstring strRet;
		size_t tRequiredCch = UTF32_TO_UTF16(pszInput, tInputCch, NULL, 0);
		strRet.resize(tRequiredCch);
		UTF32_TO_UTF16(pszInput, tInputCch, (WORD*)strRet.c_str(), tRequiredCch);
		return strRet;
	}

	std::wstring WCSFromANSI(LPCSTR pszContext, size_t tLength, size_t* ptReadSize)
	{
		std::wstring strRet;
		size_t tRequiredCch = ANSI_TO_UTF16(pszContext, tLength, NULL, 0, ptReadSize);
		strRet.resize(tRequiredCch);
		ANSI_TO_UTF16(pszContext, tLength, (WORD*)strRet.c_str(), tRequiredCch, ptReadSize);
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	std::wstring WCSFromANSI(std::string strInput, size_t* ptReadSize)
	{
		std::wstring strRet;
		size_t tRequiredCch = ANSI_TO_UTF16(strInput.c_str(), strInput.length(), NULL, 0, ptReadSize);
		strRet.resize(tRequiredCch);
		ANSI_TO_UTF16(strInput.c_str(), strInput.length(), (WORD*)strRet.c_str(), tRequiredCch, ptReadSize);
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	std::string MBSFromUTF8(LPCSTR pszContext, size_t tLength, size_t* ptReadSize)
	{
		std::wstring strUTF16;
		size_t tUTF16Size = UTF8_TO_UTF16(pszContext, tLength, NULL, 0, NULL);
		strUTF16.resize(tUTF16Size);
		UTF8_TO_UTF16(pszContext, tLength, (WORD*)strUTF16.c_str(), tUTF16Size, ptReadSize);

		std::string strRet;
		size_t tRequiredCch = UTF16_TO_ANSI((const WORD*)strUTF16.c_str(), tUTF16Size, NULL, 0);
		strRet.resize(tRequiredCch);
		UTF16_TO_ANSI((const WORD*)strUTF16.c_str(), tUTF16Size, (char*)strRet.c_str(), tRequiredCch);
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	std::string MBSFromUTF8(std::string strInput, size_t* ptReadSize)
	{
		std::wstring strUTF16;
		size_t tUTF16Size = UTF8_TO_UTF16(strInput.c_str(), strInput.length(), NULL, 0, NULL);
		strUTF16.resize(tUTF16Size);
		UTF8_TO_UTF16(strInput.c_str(), strInput.length(), (WORD*)strUTF16.c_str(), tUTF16Size, ptReadSize);

		std::string strRet;
		size_t tRequiredCch = UTF16_TO_ANSI((const WORD*)strUTF16.c_str(), tUTF16Size, NULL, 0);
		strRet.resize(tRequiredCch);
		UTF16_TO_ANSI((const WORD*)strUTF16.c_str(), tUTF16Size, (char*)strRet.c_str(), tRequiredCch);	
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	std::string MBSFromUTF16(const WORD* pszInput, size_t tInputCch)
	{
		std::string strRet;
		size_t tRequiredCch = UTF16_TO_ANSI(pszInput, tInputCch, NULL, 0);
		strRet.resize(tRequiredCch);
		UTF16_TO_ANSI(pszInput, tInputCch, (char*)strRet.c_str(), tRequiredCch);	
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	std::string MBSFromUTF32(const DWORD* pszInput, size_t tInputCch)
	{
		std::string strRet;
		size_t tRequiredCch = UTF32_TO_ANSI(pszInput, tInputCch, NULL, 0);
		strRet.resize(tRequiredCch);
		UTF32_TO_ANSI(pszInput, tInputCch, (char*)strRet.c_str(), tRequiredCch);	
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	std::string MBSFromANSI(LPCSTR pszContext, size_t tLength, size_t* ptReadSize)
	{
		std::string strRet;
		strRet.resize(tLength);
		memcpy((void*)strRet.c_str(), pszContext, tLength);
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	std::string MBSFromANSI(std::string strInput, size_t* ptReadSize)
	{
		std::string strRet;
		strRet.resize(strInput.length());
		memcpy((void*)strRet.c_str(), strInput.c_str(), strInput.length());
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	std::string UTF8FromMBS(std::string strInput)
	{
		std::wstring strUTF16;
		size_t tUTF16Len = ANSI_TO_UTF16(strInput.c_str(), strInput.length(), NULL, 0, NULL);
		strUTF16.resize(tUTF16Len);
		ANSI_TO_UTF16(strInput.c_str(), strInput.length(), (WORD*)strUTF16.c_str(), tUTF16Len, NULL);

		std::string strRet;
		size_t tRequiredCch = UTF16_TO_UTF8((const WORD*)strUTF16.c_str(), strUTF16.length(), NULL, 0);
		strRet.resize(tRequiredCch);
		UTF16_TO_UTF8((const WORD*)strUTF16.c_str(), strUTF16.length(), (char*)strRet.c_str(), tRequiredCch);	
		
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	std::wstring WCSFromWCS(std::wstring strInput)
	{
		return strInput;
	}

	//////////////////////////////////////////////////////////////////////////
	std::string MBSFromMBS(std::string strInput)
	{
		return strInput;
	}

	//////////////////////////////////////////////////////////////////////////
	std::string ANSIFromMBS(std::string strInput)
	{
		return strInput;
	}

	//////////////////////////////////////////////////////////////////////////
	std::string ANSIFromWCS(std::wstring strInput)
	{
		std::string strRet;
		size_t tRequiredCch = UTF16_TO_ANSI((const WORD*)strInput.c_str(), strInput.length(), NULL, 0);
		strRet.resize(tRequiredCch);
		UTF16_TO_ANSI((const WORD*)strInput.c_str(), strInput.length(), (char*)strRet.c_str(), tRequiredCch);
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	std::vector<WORD> UTF16FromMBS(std::string strInput)
	{
		std::vector<WORD> strRet;
		size_t tRequiredCch = ANSI_TO_UTF16(strInput.c_str(), strInput.length(), NULL, 0, NULL);
		strRet.resize(tRequiredCch);
		ANSI_TO_UTF16(strInput.c_str(), strInput.length(), &strRet[0], tRequiredCch, NULL);
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	std::vector<WORD> UTF16FromWCS(std::wstring strInput)
	{
		std::vector<WORD> strRet;
		strRet.resize(strInput.length());

		size_t i;
		for(i=0; i<strInput.length(); i++)
			strRet[i] = strInput.at(i);

		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	std::vector<DWORD> UTF32FromMBS(std::string strInput)
	{
		std::vector<DWORD> strRet;
		size_t tRequiredCch = ANSI_TO_UTF32(strInput.c_str(), strInput.length(), NULL, 0);
		strRet.resize(tRequiredCch);
		ANSI_TO_UTF32(strInput.c_str(), strInput.length(), &strRet[0], tRequiredCch);
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	std::vector<DWORD> UTF32FromWCS(std::wstring strInput)
	{
		std::vector<DWORD> strRet;
		strRet.resize(strInput.length());

		size_t i;
		for(i=0; i<strInput.length(); i++)
			strRet[i] = strInput.at(i);

		return strRet;
	}
}
