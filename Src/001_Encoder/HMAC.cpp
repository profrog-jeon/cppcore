#include "stdafx.h"
#include "HMAC.h"
#include <map>
#include "HashFunction.h"
#include "Hash.h"

namespace core
{
	static const std::map<E_HASH_TYPE, size_t> g_mapHMAC_BlockSize = {
		{	HASH_TYPE_MD5, 64	},
		{	HASH_TYPE_SHA1, 64	},
		{	HASH_TYPE_SHA256, 64	},
		{	HASH_TYPE_SHA512, 128	},
	};

	struct __internal_ST_HMAC_INFO
	{
		E_HASH_TYPE nHashType;
		size_t tBlockSize;
		HANDLE hInnerHash;
		std::vector<BYTE> vecKey;

		void Init(LPCBYTE pKey, size_t tKeyLen, std::vector<BYTE>& outInnerPaddedKey)
		{
			if (tKeyLen < tBlockSize)
			{
				vecKey.resize(tBlockSize, 0);
				memcpy(vecKey.data(), pKey, tKeyLen);
			}
			else
			{
				BuildHash(nHashType, pKey, tKeyLen, vecKey);
			}

			outInnerPaddedKey.resize(tBlockSize, 0x36);
			for (size_t i = 0; i < vecKey.size() && i < tBlockSize; i++)
				outInnerPaddedKey[i] ^= vecKey[i];
		}
	};

	static inline HANDLE InitHmacWorker(E_HASH_TYPE nHashType, LPCBYTE pKey, size_t tKeyLen)
	{
		auto iter = g_mapHMAC_BlockSize.find(nHashType);
		if (iter == g_mapHMAC_BlockSize.end())
			return NULL;

		const size_t tBlockSize = iter->second;

		__internal_ST_HMAC_INFO* pHandle = new __internal_ST_HMAC_INFO;
		pHandle->nHashType = nHashType;
		pHandle->tBlockSize = tBlockSize;

		std::vector<BYTE> vecInnerPaddedKey;
		pHandle->Init(pKey, tKeyLen, vecInnerPaddedKey);

		pHandle->hInnerHash = InitHash(nHashType);
		UpdateHash(pHandle->hInnerHash, vecInnerPaddedKey.data(), vecInnerPaddedKey.size());
		return pHandle;
	}

	HANDLE InitHMAC(E_HASH_TYPE nHashType, std::string strKey)
	{
		return InitHmacWorker(nHashType, (LPCBYTE)strKey.c_str(), strKey.length());
	}

	HANDLE InitHMAC(E_HASH_TYPE nHashType, LPCBYTE pKey, size_t tKeyLength)
	{
		return InitHmacWorker(nHashType, pKey, tKeyLength);
	}

	void UpdateHMAC(HANDLE hHash, LPCBYTE pData, size_t tSize)
	{
		__internal_ST_HMAC_INFO* pHandle = (__internal_ST_HMAC_INFO*)hHash;
		UpdateHash(pHandle->hInnerHash, pData, tSize);
	}

	void FinalHMAC(HANDLE hHash, std::vector<BYTE>& outBuff)
	{
		__internal_ST_HMAC_INFO* pHandle = (__internal_ST_HMAC_INFO*)hHash;
		std::vector<BYTE> InnerHash;
		FinalHash(pHandle->hInnerHash, InnerHash);

		std::vector<BYTE> vecOutterPaddedKey;
		vecOutterPaddedKey.resize(pHandle->tBlockSize, 0x5c);
		for (size_t i = 0; i < pHandle->vecKey.size() && i < pHandle->tBlockSize; i++)
			vecOutterPaddedKey[i] ^= pHandle->vecKey[i];

		AppendData(vecOutterPaddedKey, InnerHash);
		BuildHash(pHandle->nHashType, vecOutterPaddedKey, outBuff);

		delete pHandle;
	}
}
