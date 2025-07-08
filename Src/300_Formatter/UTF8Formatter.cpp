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
#include "BytesReader.h"
#include "BytesWriter.h"
#include "KeyValueArraySerializer.h"
#include "KeyValueArrayDeserializer.h"
#include "KeyArraySerializer.h"
#include "ValueArraySerializer.h"
#include "ValueArrayDeserializer.h"
#include "PacketSerializer.h"
#include "PacketDeserializer.h"
#include "UTF8FileReader.h"
#include "UTF8FileWritter.h"
#include "UTF8StringReader.h"
#include "UTF8StringWritter.h"
#include "YAMLSerializer.h"
#include "YAMLDeserializer.h"
#include "DBValueArraySerializer.h"

namespace core
{
	namespace UTF8
	{
		//////////////////////////////////////////////////////////////////////////
		bool WriteJsonToFile(const IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename.c_str());
			fmt_internal::CJSONSerializer formatter(channel);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteJsonToFile(const IFormatterObject* pObject, std::tstring strFilename, bool bWriteBOM)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename.c_str(), bWriteBOM);
			fmt_internal::CJSONSerializer formatter(channel);
			return formatter.Transform(pObject);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadJsonFromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileReader channel(strFilename.c_str());
			fmt_internal::CJSONDeserializer formatter(channel);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteJsonToString(const IFormatterObject* pObject, std::string& strOutput)
		{
			fmt_internal::CUTF8StringWriter channel(strOutput);
			fmt_internal::CJSONSerializer formatter(channel);
			return formatter.Transform(pObject);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadJsonFromString(IFormatterObject* pObject, std::string strInput, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringReader channel(strInput);
			fmt_internal::CJSONDeserializer formatter(channel);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteYamlToFile(const IFormatterObject* pObject, std::tstring strFilename, std::tstring strRootName, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename);
			fmt_internal::CYAMLSerializer formatter(channel);
			return formatter.Transform(pObject, pStrErrMsg, &strRootName);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadYamlFromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring* pOutRootName, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileReader channel(strFilename);
			fmt_internal::CYAMLDeserializer formatter(channel);
			return formatter.Transform(pObject, pStrErrMsg, pOutRootName);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteYamlToString(const IFormatterObject* pObject, std::string& strOutput, std::tstring strRootName, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringWriter channel(strOutput);
			fmt_internal::CYAMLSerializer formatter(channel);
			return formatter.Transform(pObject, pStrErrMsg, &strRootName);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadYamlFromString(IFormatterObject* pObject, std::string strInput, std::tstring* pOutRootName, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringReader channel(strInput);
			fmt_internal::CYAMLDeserializer formatter(channel);
			return formatter.Transform(pObject, pStrErrMsg, pOutRootName);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteCSVToFile(const IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename);
			fmt_internal::CKeyArraySerializer key_formatter(channel, TEXT(","), TEXT(""));
			((IChannel*)&channel)->Access((void*)TEXT("\r\n"), sizeof(TCHAR) * 2);

			fmt_internal::CCSVSerializer formatter(channel);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteCSVToFile(const IFormatterObject* pObject, std::tstring strFilename, bool bWriteBOM)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename, bWriteBOM);
			fmt_internal::CKeyArraySerializer key_formatter(channel, TEXT(","), TEXT(""));
			((IChannel*)&channel)->Access((void*)TEXT("\r\n"), sizeof(TCHAR) * 2);

			fmt_internal::CCSVSerializer formatter(channel);
			return formatter.Transform(pObject);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadCSVFromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileReader channel(strFilename);
			fmt_internal::CCSVDeserializer formatter(channel, true);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteCSVToString(const IFormatterObject* pObject, std::string& strOutput, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringWriter channel(strOutput);			
			fmt_internal::CKeyArraySerializer key_formatter(channel, TEXT(","), TEXT(""));
			((IChannel*)&channel)->Access((void*)TEXT("\r\n"), sizeof(TCHAR) * 2);

			fmt_internal::CCSVSerializer formatter(channel);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadCSVFromString(IFormatterObject* pObject, std::string strInput, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringReader channel(strInput);
			fmt_internal::CCSVDeserializer formatter(channel, true);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadCSVFromString(IFormatterObject* pObject, std::string strInput, bool bSkipTitle, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringReader channel(strInput);
			fmt_internal::CCSVDeserializer formatter(channel, bSkipTitle);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteXmlToFile(const IFormatterObject* pObject, std::tstring strFilename, LPCTSTR pszRootTag, std::map<std::tstring, std::tstring>* pRootAttr)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename.c_str());
			fmt_internal::CXMLSerializer formatter(channel, BOM_UTF8, pszRootTag? pszRootTag : TEXT("root"), pRootAttr);
			return formatter.Transform(pObject);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteXmlToFile(const IFormatterObject* pObject, std::tstring strFilename, bool bWriteBOM, LPCTSTR pszRootTag, std::map<std::tstring, std::tstring>* pRootAttr)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename.c_str(), bWriteBOM);
			fmt_internal::CXMLSerializer formatter(channel, BOM_UTF8, pszRootTag? pszRootTag : TEXT("root"), pRootAttr);
			return formatter.Transform(pObject);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadXmlFromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileReader channel(strFilename.c_str());
			fmt_internal::CXMLDeserializer formatter(channel);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteXmlToString(const IFormatterObject* pObject, std::string& strOutput, LPCTSTR pszRootTag, std::map<std::tstring, std::tstring>* pRootAttr)
		{
			fmt_internal::CUTF8StringWriter channel(strOutput);
			fmt_internal::CXMLSerializer formatter(channel, BOM_UTF8, pszRootTag? pszRootTag : TEXT("root"), pRootAttr);
			return formatter.Transform(pObject);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadXmlFromString(IFormatterObject* pObject, std::string strInput, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringReader channel(strInput);
			fmt_internal::CXMLDeserializer formatter(channel);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteXmlV2ToFile(const IFormatterObject* pObject, std::tstring strFilename, LPCTSTR pszRootTag, std::map<std::tstring, std::tstring>* pRootAttr)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename.c_str());
			fmt_internal::CXMLSerializer_V2 formatter(channel, BOM_UTF8, pszRootTag? pszRootTag : TEXT("root"), pRootAttr);
			return formatter.Transform(pObject);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteXmlV2ToFile(const IFormatterObject* pObject, std::tstring strFilename, bool bWriteBOM, LPCTSTR pszRootTag, std::map<std::tstring, std::tstring>* pRootAttr)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename.c_str(), bWriteBOM);
			fmt_internal::CXMLSerializer_V2 formatter(channel, BOM_UTF8, pszRootTag? pszRootTag : TEXT("root"), pRootAttr);
			return formatter.Transform(pObject);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadXmlV2FromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileReader channel(strFilename.c_str());
			fmt_internal::CXMLDeserializer_V2 formatter(channel);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteXmlV2ToString(const IFormatterObject* pObject, std::string& strOutput, LPCTSTR pszRootTag, std::map<std::tstring, std::tstring>* pRootAttr)
		{
			fmt_internal::CUTF8StringWriter channel(strOutput);
			fmt_internal::CXMLSerializer_V2 formatter(channel, BOM_UTF8, pszRootTag? pszRootTag : TEXT("root"), pRootAttr);
			return formatter.Transform(pObject);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadXmlV2FromString(IFormatterObject* pObject, std::string strInput, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringReader channel(strInput);
			fmt_internal::CXMLDeserializer_V2 formatter(channel);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteXmlV3ToFile(const IFormatterObject* pObject, std::tstring strFilename, LPCTSTR pszRootTag)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename);
			fmt_internal::CXMLSerializer_V3 formatter(channel, GetCurrentBOM(), pszRootTag? pszRootTag : TEXT("root"));
			return formatter.Transform(pObject);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteXmlV3ToFile(const IFormatterObject* pObject, std::tstring strFilename, bool bWriteBOM, LPCTSTR pszRootTag)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename, bWriteBOM);
			fmt_internal::CXMLSerializer_V3 formatter(channel, GetCurrentBOM(), pszRootTag? pszRootTag : TEXT("root"));
			return formatter.Transform(pObject);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteXmlV3ToString(const IFormatterObject* pObject, std::string& strOutput, LPCTSTR pszRootTag)
		{
			fmt_internal::CUTF8StringWriter channel(strOutput);
			fmt_internal::CXMLSerializer_V3 formatter(channel, GetCurrentBOM(), pszRootTag? pszRootTag : TEXT("root"));
			return formatter.Transform(pObject);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteMarkerToFile(const IFormatterObject* pObject, std::tstring strSchema, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename.c_str());
			fmt_internal::CMarkerSerializer formatter(channel, strSchema);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteMarkerToFile(const IFormatterObject* pObject, std::tstring strSchema, std::tstring strFilename, bool bWriteBOM)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename.c_str(), bWriteBOM);
			fmt_internal::CMarkerSerializer formatter(channel, strSchema);
			return formatter.Transform(pObject);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteMarkerToString(const IFormatterObject* pObject, std::tstring strSchema, std::string& strOutput, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringWriter channel(strOutput);
			fmt_internal::CMarkerSerializer formatter(channel, strSchema);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteKeyValueArrayToString(const IFormatterObject* pObject, std::tstring strSep, std::tstring strSchema, std::string& strOutput, bool bUseHex)
		{
			fmt_internal::CUTF8StringWriter channel(strOutput);
			fmt_internal::CKeyValueArraySerializer formatter(channel, strSep, strSchema, bUseHex);
			return formatter.Transform(pObject);
		}

		//////////////////////////////////////////////////////////////////////////	
		bool WriteKeyValueArrayToFile(const IFormatterObject* pObject, std::tstring strSep, std::tstring strSchema, std::tstring strFilename, bool bUseHex, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename);
			fmt_internal::CKeyValueArraySerializer formatter(channel, strSep, strSchema, bUseHex);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////	
		bool WriteKeyValueArrayToFile(const IFormatterObject* pObject, std::tstring strSep, std::tstring strSchema, std::tstring strFilename, bool bWriteBOM, bool bUseHex)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename, bWriteBOM);
			fmt_internal::CKeyValueArraySerializer formatter(channel, strSep, strSchema, bUseHex);
			return formatter.Transform(pObject);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadKeyValueArrayFromString(IFormatterObject* pObject, std::tstring strTokenDelimiter, std::tstring strSpliter, std::string strContext, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringReader channel(strContext);
			fmt_internal::CKeyValueArrayDeserializer formatter(channel, strTokenDelimiter, strSpliter);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadKeyValueArrayFromFile(IFormatterObject* pObject, std::tstring strTokenDelimiter, std::tstring strSpliter, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileReader channel(strFilename);
			fmt_internal::CKeyValueArrayDeserializer formatter(channel, strTokenDelimiter, strSpliter);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		////////////////////////////////////////////////////////////////////////////
		//void WriteValueArrayToString(const IFormatterObject* pObject, std::tstring strSep, std::tstring strQuo, std::string& strOutput)
		//{
		//	fmt_internal::CUTF8StringWritter channel(strOutput);
		//	fmt_internal::CValueArraySerializer formatter(channel, strSep, strQuo);
		//}

		////////////////////////////////////////////////////////////////////////////
		//bool ReadValueArrayFromString(IFormatterObject* pObject, std::string strSep, std::string strQuo, std::vector<std::tstring> vecKey, bool bIgnoreCase, std::string strContext, std::tstring* pStrErrMsg)
		//{
		//	fmt_internal::CUTF8StringReader channel(strContext);
		//	fmt_internal::CValueArrayDeserializer formatter(channel, strSep, strQuo, vecKey, bIgnoreCase);
		//	return formatter.Transform(pObject, pStrErrMsg);
		//}

		////////////////////////////////////////////////////////////////////////////
		//bool ReadValueArrayFromArray(IFormatterObject* pObject, std::vector<std::tstring> vecKey, std::vector<std::tstring> vecValue, bool bIgnoreCase, std::tstring* pStrErrMsg)
		//{
		//	fmt_internal::CUTF8StringReader dummyChannel(TEXT(""));
		//	fmt_internal::CValueArrayDeserializer formatter(dummyChannel, pObject, vecKey, vecValue, bIgnoreCase);
		//	return formatter.Transform(pObject, pStrErrMsg);
		//}

		//////////////////////////////////////////////////////////////////////////
		bool WriteIniToFile(const IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename.c_str());
			fmt_internal::CINISerializer formatter(channel);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteIniToFile(const IFormatterObject* pObject, std::tstring strFilename, bool bWriteBOM)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename.c_str(), bWriteBOM);
			fmt_internal::CINISerializer formatter(channel);
			return formatter.Transform(pObject);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadIniFromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileReader channel(strFilename.c_str());
			fmt_internal::CINIDeserializer formatter(channel);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteIniToString(const IFormatterObject* pObject, std::string& strOutput, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringWriter channel(strOutput);
			fmt_internal::CINISerializer formatter(channel);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadIniFromString(IFormatterObject* pObject, std::string strInput, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringReader channel(strInput);
			fmt_internal::CINIDeserializer formatter(channel);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadIniSectionFromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring strSection, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileReader channel(strFilename.c_str());
			fmt_internal::CINIDeserializer formatter(channel, strSection);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadIniSectionFromString(IFormatterObject* pObject, std::string strInput, std::tstring strSection, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringReader channel(strInput);
			fmt_internal::CINIDeserializer formatter(channel, strSection);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteKeyArrayToString(const IFormatterObject* pObject, std::tstring strSep, std::tstring strQuo, std::string& strOutput)
		{
			fmt_internal::CUTF8StringWriter channel(strOutput);
			fmt_internal::CKeyArraySerializer formatter(channel, strSep, strQuo);
			return formatter.Transform(pObject);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteValueArrayToString(const IFormatterObject* pObject, std::tstring strSep, std::tstring strQuo, std::string& strOutput, bool bUseHex)
		{
			fmt_internal::CUTF8StringWriter channel(strOutput);
			fmt_internal::CValueArraySerializer formatter(channel, strSep, strQuo, bUseHex);
			return formatter.Transform(pObject);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteValueArrayToFile(const IFormatterObject* pObject, std::tstring strSep, std::tstring strQuo, std::tstring strFilename, bool bWriteBOM, bool bUseHex)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename, bWriteBOM);
			fmt_internal::CValueArraySerializer formatter(channel, strSep, strQuo, bUseHex);
			return formatter.Transform(pObject);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteDBValueArrayToString(const IFormatterObject* pObject, std::string& strOutput)
		{
			fmt_internal::CUTF8StringWriter channel(strOutput);
			fmt_internal::CDBValueArraySerializer formatter(channel);
			return formatter.Transform(pObject);
		}

		////////////////////////////////////////////////////////////////////////////
		//bool ReadKeyArrayFromString(const IFormatterObject* pObject, std::tstring strSep, std::tstring strQuo, std::tstring& strOutput)
		//{
		//	fmt_internal::CUTF8StringReader channel(strFilename);
		//	fmt_internal::CKeyArrayDeserializer formatter(channel, strTokenDelimiter, strSpliter);
		//	return formatter.Transform(pObject, pStrErrMsg);
		//}

		//////////////////////////////////////////////////////////////////////////
		bool ReadValueArrayFromString(IFormatterObject* pObject, std::tstring strSep, std::tstring strQuo, std::string& strContext, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringReader channel(strContext);
			fmt_internal::CValueArrayDeserializer formatter(channel, strSep, strQuo);
			return formatter.Transform(pObject, pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadValueArrayFromFile(IFormatterObject* pObject, std::tstring strSep, std::tstring strQuo, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileReader channel(strFilename);
			fmt_internal::CValueArrayDeserializer formatter(channel, strSep, strQuo);
			return formatter.Transform(pObject, pStrErrMsg);
		}

	}
}
