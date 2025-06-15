#include "stdafx.h"
#include "300_Formatter.h"
#include "CSVSerializer.h"
#include "CSVDeserializer.h"
#include "JSONSerializer.h"
#include "JSONDeserializer.h"
#include "XMLSerializer.h"
#include "XMLDeserializer.h"
#include "XMLSerializer_V2.h"
#include "XMLDeserializer_V2.h"
#include "XMLSerializer_V3.h"
#include "MarkerSerializer.h"
#include "INISerializer.h"
#include "INIDeserializer.h"
#include "FileWritter.h"
#include "FileReader.h"
#include "StringWriter.h"
#include "StringReader.h"
#include "BytesWriter.h"
#include "BytesReader.h"
#include "KeyValueArraySerializer.h"
#include "KeyValueArrayDeserializer.h"
#include "KeyArraySerializer.h"
#include "ValueArrayDeserializer.h"
#include "ValueArraySerializer.h"
#include "PacketSerializer.h"
#include "PacketDeserializer.h"
#include "UBJSONSerializer.h"
#include "UBJSONDeserializer.h"
#include "YAMLSerializer.h"
#include "YAMLDeserializer.h"
#include "DBValueArraySerializer.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	bool WriteJsonToFile(const IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
	{
		fmt_internal::CFileWritter channel(strFilename);
		fmt_internal::CJSONSerializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadJsonFromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
	{
		fmt_internal::CFileReader channel(strFilename);
		fmt_internal::CJSONDeserializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteJsonToString(const IFormatterObject* pObject, std::tstring& strOutput, std::tstring* pStrErrMsg)
	{
		fmt_internal::CStringWriter channel(strOutput);
		fmt_internal::CJSONSerializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadJsonFromString(IFormatterObject* pObject, std::tstring strInput, std::tstring* pStrErrMsg)
	{
		fmt_internal::CStringReader channel(strInput);
		fmt_internal::CJSONDeserializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteYamlToFile(const IFormatterObject* pObject, std::tstring strFilename, std::tstring strRootName, std::tstring* pStrErrMsg)
	{
		fmt_internal::CFileWritter channel(strFilename);
		fmt_internal::CYAMLSerializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg, &strRootName);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadYamlFromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring* pOutRootName, std::tstring* pStrErrMsg)
	{
		fmt_internal::CFileReader channel(strFilename);
		fmt_internal::CYAMLDeserializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg, pOutRootName);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteYamlToString(const IFormatterObject* pObject, std::tstring& strOutput, std::tstring strRootName, std::tstring* pStrErrMsg)
	{
		fmt_internal::CStringWriter channel(strOutput);
		fmt_internal::CYAMLSerializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg, &strRootName);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadYamlFromString(IFormatterObject* pObject, std::tstring strInput, std::tstring* pOutRootName, std::tstring* pStrErrMsg)
	{
		fmt_internal::CStringReader channel(strInput);
		fmt_internal::CYAMLDeserializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg, pOutRootName);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteCSVToFile(const IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
	{
		fmt_internal::CFileWritter channel(strFilename);
		fmt_internal::CKeyArraySerializer key_formatter(channel, TEXT(","), TEXT(""));
		((IChannel*)&channel)->Access((void*)TEXT("\r\n"), sizeof(TCHAR) * 2);
		fmt_internal::CCSVSerializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadCSVFromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
	{
		fmt_internal::CFileReader channel(strFilename);
		fmt_internal::CCSVDeserializer formatter(channel, true);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteCSVToString(const IFormatterObject* pObject, std::tstring& strOutput, std::tstring* pStrErrMsg)
	{
		fmt_internal::CStringWriter channel(strOutput);			
		fmt_internal::CKeyArraySerializer key_formatter(channel, TEXT(","), TEXT(""));
		((IChannel*)&channel)->Access((void*)TEXT("\r\n"), sizeof(TCHAR) * 2);
		fmt_internal::CCSVSerializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadCSVFromString(IFormatterObject* pObject, std::tstring strInput, std::tstring* pStrErrMsg)
	{
		fmt_internal::CStringReader channel(strInput);
		fmt_internal::CCSVDeserializer formatter(channel, true);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadCSVFromString(IFormatterObject* pObject, std::tstring strInput, bool bSkipTitle, std::tstring* pStrErrMsg)
	{
		fmt_internal::CStringReader channel(strInput);
		fmt_internal::CCSVDeserializer formatter(channel, bSkipTitle);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteXmlToFile(const IFormatterObject* pObject, std::tstring strFilename, LPCTSTR pszRootTag, std::map<std::tstring, std::tstring>* pRootAttr)
	{
		fmt_internal::CFileWritter channel(strFilename);
		fmt_internal::CXMLSerializer formatter(channel, GetCurrentBOM(), pszRootTag? pszRootTag : TEXT("root"), pRootAttr);
		return formatter.Transform(pObject);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadXmlFromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
	{
		fmt_internal::CFileReader channel(strFilename);
		fmt_internal::CXMLDeserializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteXmlToString(const IFormatterObject* pObject, std::tstring& strOutput, LPCTSTR pszRootTag , std::map<std::tstring, std::tstring>* pRootAttr)
	{
		fmt_internal::CStringWriter channel(strOutput);
		fmt_internal::CXMLSerializer formatter(channel, GetCurrentBOM(), pszRootTag? pszRootTag : TEXT("root"), pRootAttr);
		return formatter.Transform(pObject);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadXmlFromString(IFormatterObject* pObject, std::tstring strInput, std::tstring* pStrErrMsg)
	{
		fmt_internal::CStringReader channel(strInput);
		fmt_internal::CXMLDeserializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteXmlV2ToFile(const IFormatterObject* pObject, std::tstring strFilename, LPCTSTR pszRootTag, std::map<std::tstring, std::tstring>* pRootAttr)
	{
		fmt_internal::CFileWritter channel(strFilename);
		fmt_internal::CXMLSerializer_V2 formatter(channel, GetCurrentBOM(), pszRootTag? pszRootTag : TEXT("root"), pRootAttr);
		return formatter.Transform(pObject);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadXmlV2FromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
	{
		fmt_internal::CFileReader channel(strFilename);
		fmt_internal::CXMLDeserializer_V2 formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteXmlV2ToString(const IFormatterObject* pObject, std::tstring& strOutput, LPCTSTR pszRootTag , std::map<std::tstring, std::tstring>* pRootAttr)
	{
		fmt_internal::CStringWriter channel(strOutput);
		fmt_internal::CXMLSerializer_V2 formatter(channel, GetCurrentBOM(), pszRootTag? pszRootTag : TEXT("root"), pRootAttr);
		return formatter.Transform(pObject);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadXmlV2FromString(IFormatterObject* pObject, std::tstring strInput, std::tstring* pStrErrMsg)
	{
		fmt_internal::CStringReader channel(strInput);
		fmt_internal::CXMLDeserializer_V2 formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteXmlV3ToFile(const IFormatterObject* pObject, std::tstring strFilename, LPCTSTR pszRootTag)
	{
		fmt_internal::CFileWritter channel(strFilename);
		fmt_internal::CXMLSerializer_V3 formatter(channel, GetCurrentBOM(), pszRootTag? pszRootTag : TEXT("root"));
		return formatter.Transform(pObject);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteXmlV3ToString(const IFormatterObject* pObject, std::tstring& strOutput, LPCTSTR pszRootTag)
	{
		fmt_internal::CStringWriter channel(strOutput);
		fmt_internal::CXMLSerializer_V3 formatter(channel, GetCurrentBOM(), pszRootTag? pszRootTag : TEXT("root"));
		return formatter.Transform(pObject);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteMarkerToFile(const IFormatterObject* pObject, std::tstring strSchema, std::tstring strFilename, std::tstring* pStrErrMsg)
	{
		fmt_internal::CFileWritter channel(strFilename);
		fmt_internal::CMarkerSerializer formatter(channel, strSchema);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteMarkerToString(const IFormatterObject* pObject, std::tstring strSchema, std::tstring& strOutput, std::tstring* pStrErrMsg)
	{
		fmt_internal::CStringWriter channel(strOutput);
		fmt_internal::CMarkerSerializer formatter(channel, strSchema);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteKeyValueArrayToString(const IFormatterObject* pObject, std::tstring strSep, std::tstring strSchema, std::tstring& strOutput, bool bUseHex)
	{
		fmt_internal::CStringWriter channel(strOutput);
		fmt_internal::CKeyValueArraySerializer formatter(channel, strSep, strSchema, bUseHex);
		return formatter.Transform(pObject);
	}

	//////////////////////////////////////////////////////////////////////////	
	bool WriteKeyValueArrayToFile(const IFormatterObject* pObject, std::tstring strSep, std::tstring strSchema, std::tstring strFilename, bool bUseHex, std::tstring* pStrErrMsg)
	{
		fmt_internal::CFileWritter channel(strFilename);
		fmt_internal::CKeyValueArraySerializer formatter(channel, strSep, strSchema, bUseHex);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteKeyArrayToString(const IFormatterObject* pObject, std::tstring strSep, std::tstring strQuo, std::tstring& strOutput)
	{
		fmt_internal::CStringWriter channel(strOutput);
		fmt_internal::CKeyArraySerializer formatter(channel, strSep, strQuo);
		return formatter.Transform(pObject);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteValueArrayToString(const IFormatterObject* pObject, std::tstring strSep, std::tstring strQuo, std::tstring& strOutput, bool bUseHex)
	{
		fmt_internal::CStringWriter channel(strOutput);
		fmt_internal::CValueArraySerializer formatter(channel, strSep, strQuo, bUseHex);
		return formatter.Transform(pObject);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteValueArrayToFile(const IFormatterObject* pObject, std::tstring strSep, std::tstring strQuo, std::tstring strFilename, bool bUseHex)
	{
		fmt_internal::CFileWritter channel(strFilename);
		fmt_internal::CValueArraySerializer formatter(channel, strSep, strQuo, bUseHex);
		return formatter.Transform(pObject);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteDBValueArrayToString(const IFormatterObject* pObject, std::tstring& strOutput)
	{
		fmt_internal::CStringWriter channel(strOutput);
		fmt_internal::CDBValueArraySerializer formatter(channel);
		return formatter.Transform(pObject);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadKeyValueArrayFromString(IFormatterObject* pObject, std::tstring strTokenDelimiter, std::tstring strSpliter, std::tstring strContext, std::tstring* pStrErrMsg)
	{
		fmt_internal::CStringReader channel(strContext);
		fmt_internal::CKeyValueArrayDeserializer formatter(channel, strTokenDelimiter, strSpliter);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadKeyValueArrayFromFile(IFormatterObject* pObject, std::tstring strTokenDelimiter, std::tstring strSpliter, std::tstring strFilename, std::tstring* pStrErrMsg)
	{
		fmt_internal::CFileReader channel(strFilename);
		fmt_internal::CKeyValueArrayDeserializer formatter(channel, strTokenDelimiter, strSpliter);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	////////////////////////////////////////////////////////////////////////////
	//bool ReadKeyArrayFromString(const IFormatterObject* pObject, std::tstring strSep, std::tstring strQuo, std::tstring& strOutput)
	//{
	//	fmt_internal::CStringReader channel(strFilename);
	//	fmt_internal::CKeyArrayDeserializer formatter(channel, strTokenDelimiter, strSpliter);
	//	return formatter.Transform(pObject, pStrErrMsg);
	//}

	//////////////////////////////////////////////////////////////////////////
	bool ReadValueArrayFromString(IFormatterObject* pObject, std::tstring strSep, std::tstring strQuo, std::tstring strContext, std::tstring* pStrErrMsg)
	{
		fmt_internal::CStringReader channel(strContext);
		fmt_internal::CValueArrayDeserializer formatter(channel, strSep, strQuo);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadValueArrayFromFile(IFormatterObject* pObject, std::tstring strSep, std::tstring strQuo, std::tstring strFilename, std::tstring* pStrErrMsg)
	{
		fmt_internal::CFileReader channel(strFilename);
		fmt_internal::CValueArrayDeserializer formatter(channel, strSep, strQuo);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteIniToFile(const IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
	{
		fmt_internal::CFileWritter channel(strFilename);
		fmt_internal::CINISerializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadIniFromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
	{
		fmt_internal::CFileReader channel(strFilename);
		fmt_internal::CINIDeserializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteIniToString(const IFormatterObject* pObject, std::tstring& strOutput, std::tstring* pStrErrMsg)
	{
		fmt_internal::CStringWriter channel(strOutput);
		fmt_internal::CINISerializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadIniFromString(IFormatterObject* pObject, std::tstring strInput, std::tstring* pStrErrMsg)
	{
		fmt_internal::CStringReader channel(strInput);
		fmt_internal::CINIDeserializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadIniSectionFromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring strSection, std::tstring* pStrErrMsg)
	{
		fmt_internal::CFileReader channel(strFilename);
		fmt_internal::CINIDeserializer formatter(channel, strSection);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadIniSectionFromString(IFormatterObject* pObject, std::tstring strInput, std::tstring strSection, std::tstring* pStrErrMsg)
	{
		fmt_internal::CStringReader channel(strInput);
		fmt_internal::CINIDeserializer formatter(channel, strSection);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteBinToFile(const IFormatterObject* pObject, std::tstring strFilename)
	{
		fmt_internal::CFileWritter channel(strFilename);
		fmt_internal::CPacketSerializer formatter(channel);
		return formatter.Transform(pObject);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadBinFromFile(IFormatterObject* pObject, std::tstring strFilename)
	{
		fmt_internal::CFileReader channel(strFilename);
		fmt_internal::CPacketDeserializer formatter(channel);
		return formatter.Transform(pObject);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteUBJsonToFile(const IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
	{
		fmt_internal::CFileWritter channel(strFilename);
		fmt_internal::CUBJSONSerializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadUBJsonFromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
	{
		fmt_internal::CFileReader channel(strFilename);
		fmt_internal::CUBJSONDeserializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	void PacketizeToBytes(const core::IFormatterObject* pObject, std::vector<BYTE>& vecData)
	{
		fmt_internal::CBytesWriter channel(vecData);
		fmt_internal::CPacketSerializer formatter(channel);
	}

	//////////////////////////////////////////////////////////////////////////
	bool UnpacketizeFromBytes(LPCBYTE pData, size_t tDataSize, core::IFormatterObject* pObject, std::tstring* pStrErrMsg)
	{
		fmt_internal::CBytesReader channel(pData, tDataSize);
		fmt_internal::CPacketDeserializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteBinToPacket(const IFormatterObject* pObject, std::vector<BYTE>& vecPacket, std::tstring* pStrErrMsg)
	{
		fmt_internal::CBytesWriter channel(vecPacket);
		fmt_internal::CPacketSerializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadBinFromPacket(IFormatterObject* pObject, const std::vector<BYTE>& vecPacket)
	{
		fmt_internal::CBytesReader channel(vecPacket.data(), vecPacket.size());
		fmt_internal::CPacketDeserializer formatter(channel);
		return formatter.Transform(pObject);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteJsonToPacket(const IFormatterObject* pObject, std::vector<BYTE>& vecPacket, std::tstring* pStrErrMsg)
	{
		fmt_internal::CBytesWriter channel(vecPacket);
		fmt_internal::CJSONSerializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadJsonFromPacket(IFormatterObject* pObject, const std::vector<BYTE>& vecPacket, std::tstring* pStrErrMsg)
	{
		fmt_internal::CBytesReader channel(vecPacket.data(), vecPacket.size());
		fmt_internal::CJSONDeserializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteUBJsonToPacket(const IFormatterObject* pObject, std::vector<BYTE>& vecPacket, std::tstring* pStrErrMsg)
	{
		fmt_internal::CBytesWriter channel(vecPacket);
		fmt_internal::CUBJSONSerializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadUBJsonFromPacket(IFormatterObject* pObject, const std::vector<BYTE>& vecPacket, std::tstring* pStrErrMsg)
	{
		fmt_internal::CBytesReader channel(vecPacket.data(), vecPacket.size());
		fmt_internal::CUBJSONDeserializer formatter(channel);
		return formatter.Transform(pObject, pStrErrMsg);
	}
}
