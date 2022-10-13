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
}