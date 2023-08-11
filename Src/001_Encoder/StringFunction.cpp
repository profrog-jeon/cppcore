#include "stdafx.h"
#include "StringFunction.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	LPCTSTR GetHashTypeString(E_HASH_TYPE dwHashType)
	{
		switch(dwHashType)
		{
			CASE_TO_STR_T(HASH_TYPE_NONE	);
			CASE_TO_STR_T(HASH_TYPE_MD5		);
			CASE_TO_STR_T(HASH_TYPE_SHA1	);
			CASE_TO_STR_T(HASH_TYPE_SHA256	);
			CASE_TO_STR_T(HASH_TYPE_HAS160	);
			CASE_TO_STR_T(HASH_TYPE_SSDEEP	);
            default:
                return TEXT("UNDEFINED_HASH_TYPE");
		}
	}

	//////////////////////////////////////////////////////////////////////////
	LPCTSTR GetCipherTypeString(E_SYM_CIPHER_TYPE dwHashType)
	{
		switch(dwHashType)
		{
			CASE_TO_STR_T(SYM_CIPHER_TYPE_PLAIN);
			CASE_TO_STR_T(SYM_CIPHER_TYPE_SEED128);
			CASE_TO_STR_T(SYM_CIPHER_TYPE_SEED256);
			CASE_TO_STR_T(SYM_CIPHER_TYPE_AES128);
			CASE_TO_STR_T(SYM_CIPHER_TYPE_AES192);
			CASE_TO_STR_T(SYM_CIPHER_TYPE_AES256);
            default:
                return TEXT("UNDEFINED_CIPHER_TYPE");
		}
	}

	//////////////////////////////////////////////////////////////////////////
	LPCTSTR GetSymEncModeString(E_SYM_CIPHER_MODE dwEncMode)
	{
		switch(dwEncMode)
		{
			CASE_TO_STR_T(SYM_CIPHER_MODE_PLAIN	);
			CASE_TO_STR_T(SYM_CIPHER_MODE_ECB	);
			CASE_TO_STR_T(SYM_CIPHER_MODE_CBC	);
			CASE_TO_STR_T(SYM_CIPHER_MODE_PCBC	);
			CASE_TO_STR_T(SYM_CIPHER_MODE_CFB	);
			CASE_TO_STR_T(SYM_CIPHER_MODE_OFB	);
			CASE_TO_STR_T(SYM_CIPHER_MODE_CTR	);
            default:
                return TEXT("UNDEFINED_CIPHER_TYPE");
		}
	}

	//////////////////////////////////////////////////////////////////////////
	LPCTSTR GetPubKeyCipherTypeString(E_PUBKEY_CIPHER_TYPE dwHashType)
	{
		switch(dwHashType)
		{
			CASE_TO_STR_T(PUBKEY_CIPHER_TYPE_RSA);
            default:
                return TEXT("UNDEFINED_ASYM_CIPHER_TYPE");
		}
	}
}

