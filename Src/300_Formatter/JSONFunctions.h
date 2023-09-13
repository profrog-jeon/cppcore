#pragma once

#include <string>
#include <vector>
#include <map>

#include "FmtTypes.h"

//////////////////////////////////////////////////////////////////////////
namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	const char* GetTokenTypeStringA(eTokenType nType);
	const wchar_t* GetTokenTypeStringW(eTokenType nType);

	//////////////////////////////////////////////////////////////////////////
	std::string ConvertToJsonString(const std::string& strValue);
	std::wstring ConvertToJsonString(const std::wstring& strValue);
	std::string RestoreFromJsonString(const std::string& strValue);
	std::wstring RestoreFromJsonString(const std::wstring& strValue);

	void ConvertToJsonString(const std::vector<BYTE>& vecValue, std::string& outJsonStr);
	void ConvertToJsonString(const std::vector<BYTE>& vecValue, std::wstring& outJsonStr);
	void RestoreFromJsonString(const std::string& strValue, std::vector<BYTE>& outValue);
	void RestoreFromJsonString(const std::wstring& strValue, std::vector<BYTE>& outValue);

	//////////////////////////////////////////////////////////////////////////
	size_t Scan(const char* pszContext, std::vector<std::string>& vecToken);
	size_t Scan(const wchar_t* pszContext, std::vector<std::wstring>& vecToken);
	size_t Scan(std::string& strContext, std::vector<std::string>& vecToken);
	size_t Scan(std::wstring& strContext, std::vector<std::wstring>& vecToken);
	bool Parse(std::vector<std::string>& vecToken, std::vector<sTokenA>& vecJsonToken, std::string& strErrMsgDump);
	bool Parse(std::vector<std::wstring>& vecToken, std::vector<sTokenW>& vecJsonToken, std::wstring& strErrMsgDump);
	size_t Build(std::vector<sTokenA>& vecJsonToken, std::vector<sChunkA>& vecJsonChunk);
	size_t Build(std::vector<sTokenW>& vecJsonToken, std::vector<sChunkW>& vecJsonChunk);
	size_t ExpandArray(size_t tIndex, std::vector<sChunkA>& vecJsonChunk);
	size_t ExpandArray(size_t tIndex, std::vector<sChunkW>& vecJsonChunk);
}
