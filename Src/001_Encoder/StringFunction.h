#pragma once

#include "Type.h"

#ifdef UNICODE
#define StringFromHex				StringFromHexW
#define	GetHashTypeString			GetHashTypeStringW		
#define	GetCipherTypeString			GetCipherTypeStringW		
#define	GetSymEncModeString			GetSymEncModeStringW		
#define	GetASymCipherTypeString		GetASymCipherTypeStringW	
#else
#define StringFromHex				StringFromHexA
#define	GetHashTypeString			GetHashTypeStringA		
#define	GetCipherTypeString			GetCipherTypeStringA		
#define	GetSymEncModeString			GetSymEncModeStringA		
#define	GetASymCipherTypeString		GetASymCipherTypeStringA	
#endif

namespace core
{
	std::string StringFromHexA(const unsigned char* pData, size_t tDataSize);
	std::string StringFromHexA(const unsigned int* pData, size_t tDataSize);
	std::wstring StringFromHexW(const unsigned char* pData, size_t tDataSize);
	std::wstring StringFromHexW(const unsigned int* pData, size_t tDataSize);
	ECODE		HexFromString(LPBYTE pDest, size_t tDestSize, std::string strContext);
	ECODE		HexFromString(LPBYTE pDest, size_t tDestSize, std::wstring strContext);

	LPCSTR		GetHashTypeStringA		(E_HASH_TYPE dwHashType);
	LPCSTR		GetCipherTypeStringA	(E_SYM_CIPHER_TYPE dwHashType);
	LPCSTR		GetSymEncModeStringA	(E_SYM_CIPHER_MODE dwEncMode);
	LPCSTR		GetASymCipherTypeStringA(E_ASYM_CIPHER_TYPE dwHashType);

	LPCWSTR		GetHashTypeStringW		(E_HASH_TYPE dwHashType);
	LPCWSTR		GetCipherTypeStringW	(E_SYM_CIPHER_TYPE dwHashType);
	LPCWSTR		GetSymEncModeStringW	(E_SYM_CIPHER_MODE dwEncMode);
	LPCWSTR		GetASymCipherTypeStringW(E_ASYM_CIPHER_TYPE dwHashType);
}
