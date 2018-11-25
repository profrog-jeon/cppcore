#pragma once

#include <string>
#include <vector>
#include "Interface.h"
#include "Pair.h"

////////////////////////////////////////////////////////////////////////////
// JSON formatter interfaces
////////////////////////////////////////////////////////////////////////////
namespace core
{
	// JSON
	bool WriteJsonToFile(const IFormatterObjectW* pObject, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);
	bool WriteJsonToFile(const IFormatterObjectA* pObject, std::string strFilename, std::string* pStrErrMsg = NULL);
	bool ReadJsonFromFile(IFormatterObjectW* pObject, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);
	bool ReadJsonFromFile(IFormatterObjectA* pObject, std::string strFilename, std::string* pStrErrMsg = NULL);
	bool WriteJsonToString(const IFormatterObjectW* pObject, std::wstring& strOutput, std::wstring* pStrErrMsg = NULL);
	bool WriteJsonToString(const IFormatterObjectA* pObject, std::string& strOutput, std::string* pStrErrMsg = NULL);
	bool ReadJsonFromString(IFormatterObjectW* pObject, std::wstring strInput, std::wstring* pStrErrMsg = NULL);
	bool ReadJsonFromString(IFormatterObjectA* pObject, std::string strInput, std::string* pStrErrMsg = NULL);
}

