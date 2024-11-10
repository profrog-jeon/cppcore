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

	// CSV
	bool WriteCSVToFile(const IFormatterObjectW* pObject, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);
	bool WriteCSVToFile(const IFormatterObjectA* pObject, std::string strFilename, std::string* pStrErrMsg = NULL);
	bool ReadCSVFromFile(IFormatterObjectW* pObject, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);
	bool ReadCSVFromFile(IFormatterObjectA* pObject, std::string strFilename, std::string* pStrErrMsg = NULL);
	bool WriteCSVToString(const IFormatterObjectW* pObject, std::wstring& strOutput, std::wstring* pStrErrMsg = NULL);
	bool WriteCSVToString(const IFormatterObjectA* pObject, std::string& strOutput, std::string* pStrErrMsg = NULL);
	bool ReadCSVFromString(IFormatterObjectW* pObject, std::wstring strInput, std::wstring* pStrErrMsg = NULL);
	bool ReadCSVFromString(IFormatterObjectA* pObject, std::string strInput, std::string* pStrErrMsg = NULL);
	bool ReadCSVFromString(IFormatterObjectW* pObject, std::wstring strInput, bool bSkipTitle, std::wstring* pStrErrMsg = NULL);
	bool ReadCSVFromString(IFormatterObjectA* pObject, std::string strInput, bool bSkipTitle, std::string* pStrErrMsg = NULL);

	// XML
	bool WriteXmlToFile(const IFormatterObjectW* pObject, std::wstring strFilename, LPCWSTR pszRootTag = L"root", std::map<std::wstring, std::wstring>* pRootAttr = NULL);
	bool WriteXmlToFile(const IFormatterObjectA* pObject, std::string strFilename, LPCSTR pszRootTag = "root", std::map<std::string, std::string>* pRootAttr = NULL);
	bool ReadXmlFromFile(IFormatterObjectW* pObject, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);
	bool ReadXmlFromFile(IFormatterObjectA* pObject, std::string strFilename, std::string* pStrErrMsg = NULL);
	bool WriteXmlToString(const IFormatterObjectW* pObject, std::wstring& strOutput, LPCWSTR pszRootTag = L"root", std::map<std::wstring, std::wstring>* pRootAttr = NULL);
	bool WriteXmlToString(const IFormatterObjectA* pObject, std::string& strOutput, LPCSTR pszRootTag = "root", std::map<std::string, std::string>* pRootAttr = NULL);
	bool ReadXmlFromString(IFormatterObjectW* pObject, std::wstring strInput, std::wstring* pStrErrMsg = NULL);
	bool ReadXmlFromString(IFormatterObjectA* pObject, std::string strInput, std::string* pStrErrMsg = NULL);

	// XML V2 - using partial attribute
	bool WriteXmlV2ToFile(const IFormatterObjectW* pObject, std::wstring strFilename, LPCWSTR pszRootTag = L"root", std::map<std::wstring, std::wstring>* pRootAttr = NULL);
	bool WriteXmlV2ToFile(const IFormatterObjectA* pObject, std::string strFilename, LPCSTR pszRootTag = "root", std::map<std::string, std::string>* pRootAttr = NULL);
	bool ReadXmlV2FromFile(IFormatterObjectW* pObject, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);
	bool ReadXmlV2FromFile(IFormatterObjectA* pObject, std::string strFilename, std::string* pStrErrMsg = NULL);
	bool WriteXmlV2ToString(const IFormatterObjectW* pObject, std::wstring& strOutput, LPCWSTR pszRootTag = L"root", std::map<std::wstring, std::wstring>* pRootAttr = NULL);
	bool WriteXmlV2ToString(const IFormatterObjectA* pObject, std::string& strOutput, LPCSTR pszRootTag = "root", std::map<std::string, std::string>* pRootAttr = NULL);
	bool ReadXmlV2FromString(IFormatterObjectW* pObject, std::wstring strInput, std::wstring* pStrErrMsg = NULL);
	bool ReadXmlV2FromString(IFormatterObjectA* pObject, std::string strInput, std::string* pStrErrMsg = NULL);

	// XML V3 - using full attribute
	bool WriteXmlV3ToFile(const IFormatterObjectW* pObject, std::wstring strFilename, LPCWSTR pszRootTag = L"root");
	bool WriteXmlV3ToFile(const IFormatterObjectA* pObject, std::string strFilename, LPCSTR pszRootTag = "root");
	bool WriteXmlV3ToString(const IFormatterObjectW* pObject, std::wstring& strOutput, LPCWSTR pszRootTag = L"root");
	bool WriteXmlV3ToString(const IFormatterObjectA* pObject, std::string& strOutput, LPCSTR pszRootTag = "root");

	// Replace Marker([parent.current]) in Schema
	// ex) abcd [root.leaf.value] to ... -> abcd SOMEVALUE to ...
	bool WriteMarkerToFile(const IFormatterObjectW* pObject, std::wstring strSchema, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);
	bool WriteMarkerToFile(const IFormatterObjectA* pObject, std::string strSchema, std::string strFilename, std::string* pStrErrMsg = NULL);
	bool WriteMarkerToString(const IFormatterObjectW* pObject, std::wstring strSchema, std::wstring& strOutput, std::wstring* pStrErrMsg = NULL);
	bool WriteMarkerToString(const IFormatterObjectA* pObject, std::string strSchema, std::string& strOutput, std::string* pStrErrMsg = NULL);

	// INI
	bool WriteIniToFile(const IFormatterObjectW* pObject, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);
	bool WriteIniToFile(const IFormatterObjectA* pObject, std::string strFilename, std::string* pStrErrMsg = NULL);
	bool ReadIniFromFile(IFormatterObjectW* pObject, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);
	bool ReadIniFromFile(IFormatterObjectA* pObject, std::string strFilename, std::string* pStrErrMsg = NULL);
	bool WriteIniToString(const IFormatterObjectW* pObject, std::wstring& strOutput, std::wstring* pStrErrMsg = NULL);
	bool WriteIniToString(const IFormatterObjectA* pObject, std::string& strOutput, std::string* pStrErrMsg = NULL);
	bool ReadIniFromString(IFormatterObjectW* pObject, std::wstring strInput, std::wstring* pStrErrMsg = NULL);
	bool ReadIniFromString(IFormatterObjectA* pObject, std::string strInput, std::string* pStrErrMsg = NULL);
	bool ReadIniSectionFromFile(IFormatterObjectW* pObject, std::wstring strFilename, std::wstring strSection, std::wstring* pStrErrMsg = NULL);
	bool ReadIniSectionFromFile(IFormatterObjectA* pObject, std::string strFilename, std::string strSection, std::string* pStrErrMsg = NULL);
	bool ReadIniSectionFromString(IFormatterObjectW* pObject, std::wstring strInput, std::wstring strSection, std::wstring* pStrErrMsg = NULL);
	bool ReadIniSectionFromString(IFormatterObjectA* pObject, std::string strInput, std::string strSection, std::string* pStrErrMsg = NULL);

	// strSchema : ex) "[KEY]=\'[VALUE]\'"  - [KEY][VALUE] will be replaced key, value
	void WriteKeyValueArrayToString(const IFormatterObjectW* pObject, std::wstring strSep, std::wstring strSchema, std::wstring& strOutput, bool bUseHex = false);
	void WriteKeyValueArrayToString(const IFormatterObjectA* pObject, std::string strSep, std::string strSchema, std::string& strOutput, bool bUseHex = false);
	bool WriteKeyValueArrayToFile(const IFormatterObjectW* pObject, std::wstring strSep, std::wstring strSchema, std::wstring strFilename, bool bUseHex = false, std::wstring* pStrErrMsg = NULL);
	bool WriteKeyValueArrayToFile(const IFormatterObjectA* pObject, std::string strSep, std::string strSchema, std::string strFilename, bool bUseHex = false, std::string* pStrErrMsg = NULL);
	void WriteKeyArrayToString(const IFormatterObjectW* pObject, std::wstring strSep, std::wstring strQuo, std::wstring& strOutput);
	void WriteKeyArrayToString(const IFormatterObjectA* pObject, std::string strSep, std::string strQuo, std::string& strOutput);	
	void WriteValueArrayToString(const IFormatterObjectW* pObject, std::wstring strSep, std::wstring strQuo, std::wstring& strOutput, bool bUseHex = false);
	void WriteValueArrayToString(const IFormatterObjectA* pObject, std::string strSep, std::string strQuo, std::string& strOutput, bool bUseHex = false);
	void WriteValueArrayToFile(const IFormatterObjectW* pObject, std::wstring strSep, std::wstring strQuo, std::wstring strFilename, bool bUseHex = false);
	void WriteValueArrayToFile(const IFormatterObjectA* pObject, std::string strSep, std::string strQuo, std::string strFilename, bool bUseHex = false);

	// [KEY]=[VALUE]\n[KEY]=[VALUE]: strTokenDelimiter = '\n', strSpliter = '='
	bool ReadKeyValueArrayFromString(IFormatterObjectW* pObject, std::wstring strTokenDelimiter, std::wstring strSpliter, std::wstring strContext, std::wstring* pStrErrMsg = NULL);
	bool ReadKeyValueArrayFromString(IFormatterObjectA* pObject, std::string strTokenDelimiter, std::string strSpliter, std::string strContext, std::string* pStrErrMsg = NULL);
	bool ReadKeyValueArrayFromFile(IFormatterObjectW* pObject, std::wstring strTokenDelimiter, std::wstring strSpliter, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);
	bool ReadKeyValueArrayFromFile(IFormatterObjectA* pObject, std::string strTokenDelimiter, std::string strSpliter, std::string strFilename, std::string* pStrErrMsg = NULL);	
	//bool ReadKeyArrayFromString(const IFormatterObjectW* pObject, std::wstring strSep, std::wstring strQuo, std::wstring& strOutput);
	//bool ReadKeyArrayFromString(const IFormatterObjectA* pObject, std::string strSep, std::string strQuo, std::string& strOutput);	
	bool ReadValueArrayFromString(IFormatterObjectW* pObject, std::wstring strSep, std::wstring strQuo, std::wstring strContext, std::wstring* pStrErrMsg = NULL);
	bool ReadValueArrayFromString(IFormatterObjectA* pObject, std::string strSep, std::string strQuo, std::string strContext, std::string* pStrErrMsg = NULL);
	bool ReadValueArrayFromFile(IFormatterObjectW* pObject, std::wstring strSep, std::wstring strQuo, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);
	bool ReadValueArrayFromFile(IFormatterObjectA* pObject, std::string strSep, std::string strQuo, std::string strFilename, std::string* pStrErrMsg = NULL);
		
	bool WriteBinToFile(const IFormatterObjectA* pObject, std::string strFilename);
	bool WriteBinToFile(const IFormatterObjectW* pObject, std::wstring strFilename);
	bool ReadBinFromFile(IFormatterObjectA* pObject, std::string strFilename);
	bool ReadBinFromFile(IFormatterObjectW* pObject, std::wstring strFilename);

	bool WriteUBJsonToFile(const IFormatterObjectA* pObject, std::string strFilename, std::string* pStrErrMsg = NULL);
	bool WriteUBJsonToFile(const IFormatterObjectW* pObject, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);
	bool ReadUBJsonFromFile(IFormatterObjectA* pObject, std::string strFilename, std::string* pStrErrMsg = NULL);
	bool ReadUBJsonFromFile(IFormatterObjectW* pObject, std::wstring strFilename, std::wstring* pStrErrMsg = NULL);

	// Packet
	void PacketizeToBytes(const core::IFormatterObjectA* pObject, std::vector<BYTE>& vecData);
	void PacketizeToBytes(const core::IFormatterObjectW* pObject, std::vector<BYTE>& vecData);
	bool UnpacketizeFromBytes(LPCBYTE pData, size_t tDataSize, core::IFormatterObjectA* pObject, std::string* pStrErrMsg = NULL);
	bool UnpacketizeFromBytes(LPCBYTE pData, size_t tDataSize, core::IFormatterObjectW* pObject, std::wstring* pStrErrMsg = NULL);

	bool WriteBinToPacket(const IFormatterObjectW* pObject, std::vector<BYTE>& vecPacket, std::wstring* pStrErrMsg = NULL);
	bool WriteBinToPacket(const IFormatterObjectA* pObject, std::vector<BYTE>& vecPacket, std::string* pStrErrMsg = NULL);
	bool ReadBinFromPacket(IFormatterObjectW* pObject, const std::vector<BYTE>& vecPacket);
	bool ReadBinFromPacket(IFormatterObjectA* pObject, const std::vector<BYTE>& vecPacket);

	bool WriteJsonToPacket(const IFormatterObjectW* pObject, std::vector<BYTE>& vecPacket, std::wstring* pStrErrMsg = NULL);
	bool WriteJsonToPacket(const IFormatterObjectA* pObject, std::vector<BYTE>& vecPacket, std::string* pStrErrMsg = NULL);
	bool ReadJsonFromPacket(IFormatterObjectW* pObject, const std::vector<BYTE>& vecPacket, std::wstring* pStrErrMsg = NULL);
	bool ReadJsonFromPacket(IFormatterObjectA* pObject, const std::vector<BYTE>& vecPacket, std::string* pStrErrMsg = NULL);

	bool WriteUBJsonToPacket(const IFormatterObjectW* pObject, std::vector<BYTE>& vecPacket, std::wstring* pStrErrMsg = NULL);
	bool WriteUBJsonToPacket(const IFormatterObjectA* pObject, std::vector<BYTE>& vecPacket, std::string* pStrErrMsg = NULL);
	bool ReadUBJsonFromPacket(IFormatterObjectW* pObject, const std::vector<BYTE>& vecPacket, std::wstring* pStrErrMsg = NULL);
	bool ReadUBJsonFromPacket(IFormatterObjectA* pObject, const std::vector<BYTE>& vecPacket, std::string* pStrErrMsg = NULL);
}

