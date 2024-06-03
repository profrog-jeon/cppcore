#include "stdafx.h"
#include "HashFunction.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	void AppendData(std::vector<BYTE>& vecDest, const std::vector<BYTE>& vecData)
	{
		vecDest.insert(vecDest.end(), vecData.begin(), vecData.end());
	}

	//////////////////////////////////////////////////////////////////////////
	void AppendData(std::vector<BYTE>& vecDest, LPCBYTE pData, size_t tDataSize)
	{
		const size_t tPrevSize = vecDest.size();
		vecDest.resize(tPrevSize + tDataSize);
		memcpy(vecDest.data() + tPrevSize, pData, tDataSize);
	}

	//////////////////////////////////////////////////////////////////////////
	std::string BuildHash(E_HASH_TYPE nType, const std::vector<BYTE>& vecData)
	{
		HANDLE hHash = InitHash(nType);
		UpdateHash(hHash, vecData.data(), vecData.size());
		return FinalHash(hHash);
	}

	//////////////////////////////////////////////////////////////////////////
	void BuildHash(E_HASH_TYPE nType, LPCBYTE pData, size_t tDataSize, std::vector<BYTE>& outHash)
	{
		HANDLE hHash = InitHash(nType);
		UpdateHash(hHash, pData, tDataSize);
		FinalHash(hHash, outHash);
	}

	//////////////////////////////////////////////////////////////////////////
	void BuildHash(E_HASH_TYPE nType, const std::vector<BYTE>& vecData, std::vector<BYTE>& outHash)
	{
		HANDLE hHash = InitHash(nType);
		UpdateHash(hHash, vecData.data(), vecData.size());
		FinalHash(hHash, outHash);
	}

	//////////////////////////////////////////////////////////////////////////
	static const std::map<E_HASH_TYPE, size_t> g_mapHMAC_BlockSize = {
		{	HASH_TYPE_MD5, 64	},
		{	HASH_TYPE_SHA1, 64	},
		{	HASH_TYPE_SHA256, 64	},
		{	HASH_TYPE_SHA512, 128	},
	};

	//////////////////////////////////////////////////////////////////////////
	static inline ECODE BuildHMACWorker(E_HASH_TYPE nHashType,
		LPCBYTE pKey, size_t tKeyLen,
		LPCBYTE pMessage, size_t tMessageSize, std::vector<BYTE>& outResult)
	{
		auto iter = g_mapHMAC_BlockSize.find(nHashType);
		if (iter == g_mapHMAC_BlockSize.end())
			return EC_NOT_SUPPORTED;

		const size_t tBlockSize = iter->second;

		std::vector<BYTE> vecKey;
		if (tKeyLen < tBlockSize)
		{
			vecKey.resize(tBlockSize, 0);
			memcpy(vecKey.data(), pKey, tKeyLen);
		}
		else
		{
			BuildHash(nHashType, pKey, tKeyLen, vecKey);
		}

		std::vector<BYTE> vecOutterPaddedKey(tBlockSize, 0x5c);
		for (size_t i = 0; i < vecKey.size() && i < tBlockSize; i++)
			vecOutterPaddedKey[i] ^= vecKey[i];

		std::vector<BYTE> vecInnerPaddedKey(tBlockSize, 0x36);
		for (size_t i = 0; i < vecKey.size() && i < tBlockSize; i++)
			vecInnerPaddedKey[i] ^= vecKey[i];

		std::vector<BYTE> InnerHash;
		AppendData(vecInnerPaddedKey, pMessage, tMessageSize);
		BuildHash(nHashType, vecInnerPaddedKey, InnerHash);

		AppendData(vecOutterPaddedKey, InnerHash);
		BuildHash(nHashType, vecOutterPaddedKey, outResult);
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE BuildHMAC(E_HASH_TYPE nHashType, const std::vector<BYTE>& Key, const std::vector<BYTE>& Message, std::vector<BYTE>& outResult)
	{
		return BuildHMACWorker(nHashType, Key.data(), Key.size(), Message.data(), Message.size(), outResult);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE BuildHMAC(core::E_HASH_TYPE nHashType, const std::string& Key, const std::string& Message, std::vector<BYTE>& outResult)
	{
		return BuildHMACWorker(nHashType, (LPCBYTE)Key.c_str(), Key.length(), (LPCBYTE)Message.c_str(), Message.length(), outResult);
	}

}