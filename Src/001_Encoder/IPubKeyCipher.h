#pragma once

#include <vector>
#include "Type.h"

namespace core
{
	struct ST_PUBKEY_CIPHER_INFO
	{
		DWORD dwPlainBlockSize;
		DWORD dwCipherBlockSize;
	};

	struct IPubKeyCipher
	{
		virtual ~IPubKeyCipher() {}

		virtual void QueryInfo(ST_PUBKEY_CIPHER_INFO& outInfo) = 0;
		virtual ECODE Crypt(LPCBYTE btSrc, LPBYTE btDest) = 0;
	};

}
