#include "stdafx.h"
#include "HashFunction.h"
#include "Hash.h"
#include "HMAC.h"

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
	static inline ECODE BuildHMACWorker(E_HASH_TYPE nHashType,
		LPCBYTE pKey, size_t tKeyLen,
		LPCBYTE pMessage, size_t tMessageSize, std::vector<BYTE>& outResult)
	{
		HANDLE hHMAC = InitHMAC(nHashType, pKey, tKeyLen);
		UpdateHMAC(hHMAC, pMessage, tMessageSize);
		FinalHMAC(hHMAC, outResult);
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE BuildHMAC(E_HASH_TYPE nHashType, const std::vector<BYTE>& Key, LPCBYTE pMessage, size_t tMessageSize, std::vector<BYTE>& outResult)
	{
		return BuildHMACWorker(nHashType, Key.data(), Key.size(), pMessage, tMessageSize, outResult);
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