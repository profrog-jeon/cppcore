#include "stdafx.h"
#include "Unicode.h"
#include "UnicodeEncoding.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	std::string ANSIFromUTF8(std::string strInput, size_t* ptReadSize)
	{
		if( strInput.empty() )
			return "";

		std::vector<WORD> vecUTF16;
		{
			size_t tRequiredCch = UTF8_TO_UTF16(strInput.c_str(), strInput.length(), NULL, 0);
			vecUTF16.resize(tRequiredCch);
			UTF8_TO_UTF16(strInput.c_str(), strInput.length(), &vecUTF16[0], tRequiredCch, ptReadSize);
		}
		
		std::string strRet;
		size_t tRequiredCch = UTF16_TO_ANSI(&vecUTF16[0], vecUTF16.size(), NULL, 0);
		strRet.resize(tRequiredCch);
		UTF16_TO_ANSI(&vecUTF16[0], vecUTF16.size(), (char*)strRet.c_str(), tRequiredCch);
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	std::string UTF8FromANSI(std::string strInput, size_t* ptReadSize)
	{
		if( strInput.empty() )
			return "";

		std::vector<WORD> vecUTF16;
		{
			size_t tRequiredCch = ANSI_TO_UTF16(strInput.c_str(), strInput.length(), NULL, 0);
			vecUTF16.resize(tRequiredCch);
			ANSI_TO_UTF16(strInput.c_str(), strInput.length(), &vecUTF16[0], tRequiredCch, ptReadSize);
		}

		std::string strRet;
		size_t tRequiredCch = UTF16_TO_UTF8(&vecUTF16[0], vecUTF16.size(), NULL, 0);
		strRet.resize(tRequiredCch);
		UTF16_TO_UTF8(&vecUTF16[0], vecUTF16.size(), (char*)strRet.c_str(), tRequiredCch);
		return strRet;
	}
}
