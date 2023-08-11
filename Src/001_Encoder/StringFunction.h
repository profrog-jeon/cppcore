#pragma once

#include "Type.h"

#ifdef UNICODE
#define	GetHashTypeString			GetHashTypeStringW		
#define	GetCipherTypeString			GetCipherTypeStringW		
#define	GetSymEncModeString			GetSymEncModeStringW		
#define	GetPubKeyCipherTypeString		GetASymCipherTypeStringW	
#else
#define	GetHashTypeString			GetHashTypeStringA		
#define	GetCipherTypeString			GetCipherTypeStringA		
#define	GetSymEncModeString			GetSymEncModeStringA		
#define	GetPubKeyCipherTypeString		GetASymCipherTypeStringA	
#endif

namespace core
{
	LPCSTR		GetHashTypeStringA		(E_HASH_TYPE dwHashType);
	LPCSTR		GetCipherTypeStringA	(E_SYM_CIPHER_TYPE dwHashType);
	LPCSTR		GetSymEncModeStringA	(E_SYM_CIPHER_MODE dwEncMode);
	LPCSTR		GetASymCipherTypeStringA(E_PUBKEY_CIPHER_TYPE dwHashType);

	LPCWSTR		GetHashTypeStringW		(E_HASH_TYPE dwHashType);
	LPCWSTR		GetCipherTypeStringW	(E_SYM_CIPHER_TYPE dwHashType);
	LPCWSTR		GetSymEncModeStringW	(E_SYM_CIPHER_MODE dwEncMode);
	LPCWSTR		GetASymCipherTypeStringW(E_PUBKEY_CIPHER_TYPE dwHashType);
}
