#pragma once

#define HashFile	BuildHash		// for lower compatibility

namespace core
{
	std::string BuildHash(E_HASH_TYPE nType, std::string strFilePath);
	std::string BuildHash(E_HASH_TYPE nType, std::wstring strFilePath);
	std::string BuildHash(E_HASH_TYPE nType, const std::vector<BYTE>& vecData);

	std::string BuildFileHash(E_HASH_TYPE nType, std::string strFilePath);
	std::wstring BuildFileHash(E_HASH_TYPE nType, std::wstring strFilePath);
	std::string BuildFileHashA(E_HASH_TYPE nType, std::string strFilePath);
	std::string BuildFileHashA(E_HASH_TYPE nType, std::wstring strFilePath);
	std::wstring BuildFileHashW(E_HASH_TYPE nType, std::string strFilePath);
	std::wstring BuildFileHashW(E_HASH_TYPE nType, std::wstring strFilePath);
	bool BuildFileHash(E_HASH_TYPE nType, std::string strFilePath, std::vector<BYTE>& vecHash);
	bool BuildFileHash(E_HASH_TYPE nType, std::wstring strFilePath, std::vector<BYTE>& vecHash);

	std::string BuildStringHash(E_HASH_TYPE nType, std::string strContext);
	std::wstring BuildStringHash(E_HASH_TYPE nType, std::wstring strContext);
	std::string BuildStringHashA(E_HASH_TYPE nType, std::string strContext);
	std::string BuildStringHashA(E_HASH_TYPE nType, std::wstring strContext);
	std::wstring BuildStringHashW(E_HASH_TYPE nType, std::string strContext);
	std::wstring BuildStringHashW(E_HASH_TYPE nType, std::wstring strContext);
	bool BuildStringHash(E_HASH_TYPE nType, std::string strContext, std::vector<BYTE>& vecHash);
	bool BuildStringHash(E_HASH_TYPE nType, std::wstring strContext, std::vector<BYTE>& vecHash);
}