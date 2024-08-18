#pragma once

#define HashFile	BuildHash		// for lower compatibility

namespace core
{
	std::string BuildHash(E_HASH_TYPE nType, std::string strFilePath);
	std::string BuildHash(E_HASH_TYPE nType, std::wstring strFilePath);

	ECODE BuildHMAC(E_HASH_TYPE nHashType, const std::vector<BYTE>& Key, const std::vector<BYTE>& Message, std::vector<BYTE>& outResult);
	ECODE BuildHMAC(E_HASH_TYPE nHashType, const std::string& Key, const std::string& Message, std::vector<BYTE>& outResult);

	std::string BuildFileHash(E_HASH_TYPE nType, std::string strFilePath);
	std::wstring BuildFileHash(E_HASH_TYPE nType, std::wstring strFilePath);
	std::string BuildFileHashA(E_HASH_TYPE nType, std::string strFilePath);
	std::string BuildFileHashA(E_HASH_TYPE nType, std::wstring strFilePath);
	std::wstring BuildFileHashW(E_HASH_TYPE nType, std::string strFilePath);
	std::wstring BuildFileHashW(E_HASH_TYPE nType, std::wstring strFilePath);
	bool BuildFileHash(E_HASH_TYPE nType, std::string strFilePath, std::vector<BYTE>& vecHash);
	bool BuildFileHash(E_HASH_TYPE nType, std::wstring strFilePath, std::vector<BYTE>& vecHash);
}