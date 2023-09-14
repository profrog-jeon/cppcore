#pragma once

#include <string>
#include <vector>
#include <map>

#include "FmtTypes.h"

//////////////////////////////////////////////////////////////////////////
namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	const char* GetTokenTypeStringA(E_TOKEN_TYPE nType);
	const wchar_t* GetTokenTypeStringW(E_TOKEN_TYPE nType);

	//////////////////////////////////////////////////////////////////////////
	std::string ConvertToJsonString(const std::string& strValue);
	std::wstring ConvertToJsonString(const std::wstring& strValue);
	std::string RestoreFromJsonString(const std::string& strValue);
	std::wstring RestoreFromJsonString(const std::wstring& strValue);

	//////////////////////////////////////////////////////////////////////////
	size_t Scan(const char* pszContext, std::vector<std::tstring>& vecToken);
	size_t Scan(std::tstring& strContext, std::vector<std::tstring>& vecToken);
	bool Parse(std::vector<std::tstring>& vecToken, std::vector<ST_JSON_CHUNK>& vecJsonToken, std::tstring& strErrMsgDump);
	size_t Build(std::vector<ST_JSON_CHUNK>& vecJsonToken, std::vector<ST_JSON_TOKEN>& vecJsonChunk);
	size_t ExpandArray(size_t tIndex, std::vector<ST_JSON_TOKEN>& vecJsonChunk);
}
