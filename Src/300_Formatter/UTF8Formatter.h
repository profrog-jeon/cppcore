#pragma once

#include <string>
#include <vector>
#include "Interface.h"

////////////////////////////////////////////////////////////////////////////
// JSON formatter interfaces
////////////////////////////////////////////////////////////////////////////
namespace core
{
	namespace UTF8
	{
		// JSON
		bool WriteJsonToFile(const IFormatterObjectW* pObject, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);
		bool WriteJsonToFile(const IFormatterObjectA* pObject, std::string strFilename, std::string* pStrErrMsg = NULL);
		bool WriteJsonToFile(const IFormatterObjectW* pObject, std::wstring strFilename, bool bWriteBOM);
		bool WriteJsonToFile(const IFormatterObjectA* pObject, std::string strFilename, bool bWriteBOM);
		bool ReadJsonFromFile(IFormatterObjectW* pObject, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);
		bool ReadJsonFromFile(IFormatterObjectA* pObject, std::string strFilename, std::string* pStrErrMsg = NULL);
		bool WriteJsonToString(const IFormatterObjectW* pObject, std::string& strOutput);
		bool WriteJsonToString(const IFormatterObjectA* pObject, std::string& strOutput);
		bool ReadJsonFromString(IFormatterObjectW* pObject, std::string strInput, std::wstring* pStrErrMsg = NULL);
		bool ReadJsonFromString(IFormatterObjectA* pObject, std::string strInput, std::string* pStrErrMsg = NULL);

		// YAML
		bool WriteYamlToFile(const IFormatterObjectW* pObject, std::wstring strFilename, std::wstring strRootName = L"default", std::wstring* pStrErrMsg = NULL);
		bool WriteYamlToFile(const IFormatterObjectA* pObject, std::string strFilename, std::string strRootName = "default", std::string* pStrErrMsg = NULL);
		bool ReadYamlFromFile(IFormatterObjectW* pObject, std::wstring strFilename, std::wstring* pOutRootName = NULL, std::wstring* pStrErrMsg = NULL);
		bool ReadYamlFromFile(IFormatterObjectA* pObject, std::string strFilename, std::string* pOutRootName = NULL, std::string* pStrErrMsg = NULL);
		bool WriteYamlToString(const IFormatterObjectW* pObject, std::string& strOutput, std::wstring strRootName = L"default", std::wstring* pStrErrMsg = NULL);
		bool WriteYamlToString(const IFormatterObjectA* pObject, std::string& strOutput, std::string strRootName = "default", std::string* pStrErrMsg = NULL);
		bool ReadYamlFromString(IFormatterObjectW* pObject, std::string strInput, std::wstring* pOutRootName = NULL, std::wstring* pStrErrMsg = NULL);
		bool ReadYamlFromString(IFormatterObjectA* pObject, std::string strInput, std::string* pOutRootName = NULL, std::string* pStrErrMsg = NULL);

		// CSV
		bool WriteCSVToFile(const IFormatterObjectW* pObject, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);
		bool WriteCSVToFile(const IFormatterObjectA* pObject, std::string strFilename, std::string* pStrErrMsg = NULL);
		bool WriteCSVToFile(const IFormatterObjectW* pObject, std::wstring strFilename, bool bWriteBOM);
		bool WriteCSVToFile(const IFormatterObjectA* pObject, std::string strFilename, bool bWriteBOM);
		bool ReadCSVFromFile(IFormatterObjectW* pObject, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);
		bool ReadCSVFromFile(IFormatterObjectA* pObject, std::string strFilename, std::string* pStrErrMsg = NULL);
		bool WriteCSVToString(const IFormatterObjectW* pObject, std::string& strOutput, std::wstring* pStrErrMsg = NULL);
		bool WriteCSVToString(const IFormatterObjectA* pObject, std::string& strOutput, std::string* pStrErrMsg = NULL);
		bool ReadCSVFromString(IFormatterObjectW* pObject, std::string strInput, std::wstring* pStrErrMsg = NULL);
		bool ReadCSVFromString(IFormatterObjectA* pObject, std::string strInput, std::string* pStrErrMsg = NULL);
		bool ReadCSVFromString(IFormatterObjectW* pObject, std::string strInput, bool bSkipTitle, std::wstring* pStrErrMsg = NULL);
		bool ReadCSVFromString(IFormatterObjectA* pObject, std::string strInput, bool bSkipTitle, std::string* pStrErrMsg = NULL);

		// XML
		bool WriteXmlToFile(const IFormatterObjectW* pObject, std::wstring strFilename, LPCWSTR pszRootTag = L"root", std::map<std::wstring, std::wstring>* pRootAttr = NULL);
		bool WriteXmlToFile(const IFormatterObjectA* pObject, std::string strFilename, LPCSTR pszRootTag = "root", std::map<std::string, std::string>* pRootAttr = NULL);
		bool WriteXmlToFile(const IFormatterObjectW* pObject, std::wstring strFilename, bool bWriteBOM, LPCWSTR pszRootTag = L"root", std::map<std::wstring, std::wstring>* pRootAttr = NULL);
		bool WriteXmlToFile(const IFormatterObjectA* pObject, std::string strFilename, bool bWriteBOM, LPCSTR pszRootTag = "root", std::map<std::string, std::string>* pRootAttr = NULL);
		bool ReadXmlFromFile(IFormatterObjectW* pObject, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);
		bool ReadXmlFromFile(IFormatterObjectA* pObject, std::string strFilename, std::string* pStrErrMsg = NULL);
		bool WriteXmlToString(const IFormatterObjectW* pObject, std::string& strOutput, LPCWSTR pszRootTag = L"root", std::map<std::wstring, std::wstring>* pRootAttr = NULL);
		bool WriteXmlToString(const IFormatterObjectA* pObject, std::string& strOutput, LPCSTR pszRootTag = "root", std::map<std::string, std::string>* pRootAttr = NULL);
		bool ReadXmlFromString(IFormatterObjectW* pObject, std::string strInput, std::wstring* pStrErrMsg = NULL);
		bool ReadXmlFromString(IFormatterObjectA* pObject, std::string strInput, std::string* pStrErrMsg = NULL);

		bool WriteXmlV2ToFile(const IFormatterObjectW* pObject, std::wstring strFilename, LPCWSTR pszRootTag = L"root", std::map<std::wstring, std::wstring>* pRootAttr = NULL);
		bool WriteXmlV2ToFile(const IFormatterObjectA* pObject, std::string strFilename, LPCSTR pszRootTag = "root", std::map<std::string, std::string>* pRootAttr = NULL);
		bool WriteXmlV2ToFile(const IFormatterObjectW* pObject, std::wstring strFilename, bool bWriteBOM, LPCWSTR pszRootTag = L"root", std::map<std::wstring, std::wstring>* pRootAttr = NULL);
		bool WriteXmlV2ToFile(const IFormatterObjectA* pObject, std::string strFilename, bool bWriteBOM, LPCSTR pszRootTag = "root", std::map<std::string, std::string>* pRootAttr = NULL);
		bool ReadXmlV2FromFile(IFormatterObjectW* pObject, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);
		bool ReadXmlV2FromFile(IFormatterObjectA* pObject, std::string strFilename, std::string* pStrErrMsg = NULL);
		bool WriteXmlV2ToString(const IFormatterObjectW* pObject, std::string& strOutput, LPCWSTR pszRootTag = L"root", std::map<std::wstring, std::wstring>* pRootAttr = NULL);
		bool WriteXmlV2ToString(const IFormatterObjectA* pObject, std::string& strOutput, LPCSTR pszRootTag = "root", std::map<std::string, std::string>* pRootAttr = NULL);
		bool ReadXmlV2FromString(IFormatterObjectW* pObject, std::string strInput, std::wstring* pStrErrMsg = NULL);
		bool ReadXmlV2FromString(IFormatterObjectA* pObject, std::string strInput, std::string* pStrErrMsg = NULL);

		bool WriteXmlV3ToFile(const IFormatterObjectW* pObject, std::wstring strFilename, LPCWSTR pszRootTag = L"root");
		bool WriteXmlV3ToFile(const IFormatterObjectA* pObject, std::string strFilename, LPCSTR pszRootTag = "root");
		bool WriteXmlV3ToFile(const IFormatterObjectW* pObject, std::wstring strFilename, LPCWSTR pszRootTag, bool bWriteBOM);
		bool WriteXmlV3ToFile(const IFormatterObjectA* pObject, std::string strFilename, LPCSTR pszRootTag, bool bWriteBOM);
		bool WriteXmlV3ToString(const IFormatterObjectW* pObject, std::wstring& strOutput, LPCWSTR pszRootTag = L"root");
		bool WriteXmlV3ToString(const IFormatterObjectA* pObject, std::string& strOutput, LPCSTR pszRootTag = "root");

		bool WriteMarkerToFile(const IFormatterObjectW* pObject, std::wstring strSchema, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);
		bool WriteMarkerToFile(const IFormatterObjectA* pObject, std::string strSchema, std::string strFilename, std::string* pStrErrMsg = NULL);
		bool WriteMarkerToFile(const IFormatterObjectW* pObject, std::wstring strSchema, std::wstring strFilename, bool bWriteBOM);
		bool WriteMarkerToFile(const IFormatterObjectA* pObject, std::string strSchema, std::string strFilename, bool bWriteBOM);
		bool WriteMarkerToString(const IFormatterObjectW* pObject, std::wstring strSchema, std::string& strOutput, std::wstring* pStrErrMsg = NULL);
		bool WriteMarkerToString(const IFormatterObjectA* pObject, std::string strSchema, std::string& strOutput, std::string* pStrErrMsg = NULL);

		// INI
		bool WriteIniToFile(const IFormatterObjectW* pObject, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);
		bool WriteIniToFile(const IFormatterObjectA* pObject, std::string strFilename, std::string* pStrErrMsg = NULL);
		bool WriteIniToFile(const IFormatterObjectW* pObject, std::wstring strFilename, bool bWriteBOM);
		bool WriteIniToFile(const IFormatterObjectA* pObject, std::string strFilename, bool bWriteBOM);
		bool ReadIniFromFile(IFormatterObjectW* pObject, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);
		bool ReadIniFromFile(IFormatterObjectA* pObject, std::string strFilename, std::string* pStrErrMsg = NULL);
		bool WriteIniToString(const IFormatterObjectW* pObject, std::string& strOutput, std::wstring* pStrErrMsg = NULL);
		bool WriteIniToString(const IFormatterObjectA* pObject, std::string& strOutput, std::string* pStrErrMsg = NULL);
		bool ReadIniFromString(IFormatterObjectW* pObject, std::string strInput, std::wstring* pStrErrMsg = NULL);
		bool ReadIniFromString(IFormatterObjectA* pObject, std::string strInput, std::string* pStrErrMsg = NULL);
		bool ReadIniSectionFromFile(IFormatterObjectW* pObject, std::wstring strFilename, std::wstring strSection, std::wstring* pStrErrMsg = NULL);
		bool ReadIniSectionFromFile(IFormatterObjectA* pObject, std::string strFilename, std::string strSection, std::string* pStrErrMsg = NULL);
		bool ReadIniSectionFromString(IFormatterObjectW* pObject, std::string strInput, std::wstring strSection, std::wstring* pStrErrMsg = NULL);
		bool ReadIniSectionFromString(IFormatterObjectA* pObject, std::string strInput, std::string strSection, std::string* pStrErrMsg = NULL);

		// strSchema : ex) "[KEY]=\'[VALUE]\'"  - [KEY][VALUE] will be replaced key, value
		bool WriteKeyValueArrayToString(const IFormatterObjectW* pObject, std::wstring strSep, std::wstring strSchema, std::string& strOutput, bool bUseHex = false);
		bool WriteKeyValueArrayToString(const IFormatterObjectA* pObject, std::string strSep, std::string strSchema, std::string& strOutput, bool bUseHex = false);
		bool WriteKeyValueArrayToFile(const IFormatterObjectW* pObject, std::wstring strSep, std::wstring strSchema, std::wstring strFilename, bool bWriteBOM, bool bUseHex = false);
		bool WriteKeyValueArrayToFile(const IFormatterObjectA* pObject, std::string strSep, std::string strSchema, std::string strFilename, bool bWriteBOM, bool bUseHex = false);

		// [KEY]=[VALUE]\n[KEY]=[VALUE]: strTokenDelimiter = '\n', strSpliter = '='
		bool ReadKeyValueArrayFromString(IFormatterObjectW* pObject, std::wstring strTokenDelimiter, std::wstring strSpliter, std::string strContext, std::wstring* pStrErrMsg = NULL);
		bool ReadKeyValueArrayFromString(IFormatterObjectA* pObject, std::string strTokenDelimiter, std::string strSpliter, std::string strContext, std::string* pStrErrMsg = NULL);
		bool ReadKeyValueArrayFromFile(IFormatterObjectW* pObject, std::wstring strTokenDelimiter, std::wstring strSpliter, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);
		bool ReadKeyValueArrayFromFile(IFormatterObjectA* pObject, std::string strTokenDelimiter, std::string strSpliter, std::string strFilename, std::string* pStrErrMsg = NULL);	

		bool WriteKeyArrayToString(const IFormatterObjectW* pObject, std::wstring strSep, std::wstring strQuo, std::string& strOutput);
		bool WriteKeyArrayToString(const IFormatterObjectA* pObject, std::string strSep, std::string strQuo, std::string& strOutput);	
		bool WriteValueArrayToString(const IFormatterObjectW* pObject, std::wstring strSep, std::wstring strQuo, std::string& strOutput);
		bool WriteValueArrayToString(const IFormatterObjectA* pObject, std::string strSep, std::string strQuo, std::string& strOutput);
		bool WriteValueArrayToFile(const IFormatterObjectW* pObject, std::wstring strSep, std::wstring strQuo, std::wstring strFilename, bool bWriteBOM);
		bool WriteValueArrayToFile(const IFormatterObjectA* pObject, std::string strSep, std::string strQuo, std::string strFilename, bool bWriteBOM);

		bool WriteDBValueArrayToString(const IFormatterObjectW* pObject, std::string& strOutput);
		bool WriteDBValueArrayToString(const IFormatterObjectA* pObject, std::string& strOutput);

		//bool ReadKeyArrayFromString(const IFormatterObjectW* pObject, std::wstring strSep, std::wstring strQuo, std::wstring& strOutput);
		//bool ReadKeyArrayFromString(const IFormatterObjectA* pObject, std::string strSep, std::string strQuo, std::string& strOutput);	
		bool ReadValueArrayFromString(IFormatterObjectW* pObject, std::wstring strSep, std::wstring strQuo, std::string strContext, std::wstring* pStrErrMsg = NULL);
		bool ReadValueArrayFromString(IFormatterObjectA* pObject, std::string strSep, std::string strQuo, std::string strContext, std::string* pStrErrMsg = NULL);
		bool ReadValueArrayFromFile(IFormatterObjectW* pObject, std::wstring strSep, std::wstring strQuo, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);
		bool ReadValueArrayFromFile(IFormatterObjectA* pObject, std::string strSep, std::string strQuo, std::string strFilename, std::string* pStrErrMsg = NULL);
	}
}

