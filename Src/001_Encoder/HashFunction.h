#pragma once

#define HashFile	BuildHash		// for lower compatibility

namespace core
{
	void AppendData(std::vector<BYTE>& vecDest, const std::vector<BYTE>& vecData);
	void AppendData(std::vector<BYTE>& vecDest, LPCBYTE pData, size_t tDataSize);

	std::string BuildHash(E_HASH_TYPE nType, const std::vector<BYTE>& vecData);
	void BuildHash(E_HASH_TYPE nType, LPCBYTE pData, size_t tDataSize, std::vector<BYTE>& outHash);
	void BuildHash(E_HASH_TYPE nType, const std::vector<BYTE>& vecData, std::vector<BYTE>& outHash);

	ECODE BuildHMAC(E_HASH_TYPE nHashType, const std::vector<BYTE>& Key, LPCBYTE pMessage, size_t tMessageSize, std::vector<BYTE>& outResult);
	ECODE BuildHMAC(E_HASH_TYPE nHashType, const std::vector<BYTE>& Key, const std::vector<BYTE>& Message, std::vector<BYTE>& outResult);
	ECODE BuildHMAC(E_HASH_TYPE nHashType, const std::string& Key, const std::string& Message, std::vector<BYTE>& outResult);

	std::string BuildStringHash(E_HASH_TYPE nType, std::string strContext);
	std::wstring BuildStringHash(E_HASH_TYPE nType, std::wstring strContext);
	std::string BuildStringHashA(E_HASH_TYPE nType, std::string strContext);
	std::string BuildStringHashA(E_HASH_TYPE nType, std::wstring strContext);
	std::wstring BuildStringHashW(E_HASH_TYPE nType, std::string strContext);
	std::wstring BuildStringHashW(E_HASH_TYPE nType, std::wstring strContext);
	bool BuildStringHash(E_HASH_TYPE nType, std::string strContext, std::vector<BYTE>& vecHash);
	bool BuildStringHash(E_HASH_TYPE nType, std::wstring strContext, std::vector<BYTE>& vecHash);
}