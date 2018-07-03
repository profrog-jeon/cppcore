#pragma once

#ifdef UNICODE
#define EncodeBase64	EncodeBase64W
#else
#define EncodeBase64	EncodeBase64A
#endif

namespace core
{
	std::string EncodeBase64A(LPCBYTE pData, size_t tDataSize);
	std::wstring EncodeBase64W(LPCBYTE pData, size_t tDataSize);
	size_t DecodeBase64(std::string strBase64, LPBYTE pDest = NULL);
	size_t DecodeBase64(std::wstring strBase64, LPBYTE pDest = NULL);
}
