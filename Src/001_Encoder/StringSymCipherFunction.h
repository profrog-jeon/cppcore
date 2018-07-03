#pragma once

#include "../__Common/Type.h"
#include "Type.h"

#ifdef UNICODE
#define EncodeString			EncodeStringW
#define DefaultEncodeString		DefaultEncodeStringW
#else
#define EncodeString			EncodeStringA
#define DefaultEncodeString		DefaultEncodeStringA
#endif

namespace core
{
	std::string		EncodeStringA(const ST_SYM_CIPHER_KEY& stCipherKey, std::string strUTF8);
	std::wstring	EncodeStringW(const ST_SYM_CIPHER_KEY& stCipherKey, std::string strUTF8);
	std::string		DecodeString(const ST_SYM_CIPHER_KEY& stCipherKey, std::string strEncrypted);
	std::string		DecodeString(const ST_SYM_CIPHER_KEY& stCipherKey, std::wstring strEncrypted);

	std::string		DefaultEncodeStringA(std::string strUTF8);
	std::wstring	DefaultEncodeStringW(std::string strUTF8);
	std::string		DefaultDecodeString(std::string strEncrypted);
	std::string		DefaultDecodeString(std::wstring strEncrypted);

}