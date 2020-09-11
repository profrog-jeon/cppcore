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
#include "KeyValueArraySerializer.h"
#include "KeyValueArrayDeserializer.h"
#include "KeyArraySerializer.h"
#include "ValueArraySerializer.h"
#include "ValueArrayDeserializer.h"
#include "UTF8FileReader.h"
#include "UTF8FileWritter.h"
#include "UTF8StringReader.h"
#include "UTF8StringWritter.h"

namespace core
{
	namespace UTF8
	{
		//////////////////////////////////////////////////////////////////////////
		bool WriteJsonToFile(const IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename.c_str());
			fmt_internal::CJSONSerializer formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteJsonToFile(const IFormatterObject* pObject, std::tstring strFilename, bool bWriteBOM)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename.c_str(), bWriteBOM);
			fmt_internal::CJSONSerializer formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(NULL);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadJsonFromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileReader channel(strFilename.c_str());
			fmt_internal::CJSONDeserializer formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteJsonToString(const IFormatterObject* pObject, std::string& strOutput)
		{
			fmt_internal::CUTF8StringWriter channel(strOutput);
			fmt_internal::CJSONSerializer formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadJsonFromString(IFormatterObject* pObject, std::string strInput, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringReader channel(strInput);
			fmt_internal::CJSONDeserializer formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteCSVToFile(const IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename);
			fmt_internal::CKeyArraySerializer key_formatter(channel, TEXT(","), TEXT(""));
			const_cast<IFormatterObject*>(pObject)->Synchronize(key_formatter);
			((IChannel*)&channel)->OnAccess((void*)TEXT("\r\n"), sizeof(TCHAR) * 2);
			fmt_internal::CCSVSerializer value_formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(value_formatter);
			return value_formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteCSVToFile(const IFormatterObject* pObject, std::tstring strFilename, bool bWriteBOM)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename, bWriteBOM);
			fmt_internal::CKeyArraySerializer key_formatter(channel, TEXT(","), TEXT(""));
			const_cast<IFormatterObject*>(pObject)->Synchronize(key_formatter);
			((IChannel*)&channel)->OnAccess((void*)TEXT("\r\n"), sizeof(TCHAR) * 2);
			fmt_internal::CCSVSerializer value_formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(value_formatter);
			return value_formatter.CheckValidity(NULL);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadCSVFromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileReader channel(strFilename);
			fmt_internal::CCSVDeserializer formatter(channel, true);
			pObject->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteCSVToString(const IFormatterObject* pObject, std::string& strOutput, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringWriter channel(strOutput);			
			fmt_internal::CKeyArraySerializer key_formatter(channel, TEXT(","), TEXT(""));
			const_cast<IFormatterObject*>(pObject)->Synchronize(key_formatter);
			((IChannel*)&channel)->OnAccess((void*)TEXT("\r\n"), sizeof(TCHAR) * 2);
			fmt_internal::CCSVSerializer value_formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(value_formatter);
			return value_formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadCSVFromString(IFormatterObject* pObject, std::string strInput, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringReader channel(strInput);
			fmt_internal::CCSVDeserializer formatter(channel, true);
			pObject->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteXmlToFile(const IFormatterObject* pObject, std::tstring strFilename, LPCTSTR pszRootTag, std::map<std::tstring, std::tstring>* pRootAttr)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename.c_str());
			fmt_internal::CXMLSerializer formatter(channel, BOM_UTF8, pszRootTag? pszRootTag : TEXT("root"), pRootAttr);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(NULL);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteXmlToFile(const IFormatterObject* pObject, std::tstring strFilename, bool bWriteBOM, LPCTSTR pszRootTag, std::map<std::tstring, std::tstring>* pRootAttr)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename.c_str(), bWriteBOM);
			fmt_internal::CXMLSerializer formatter(channel, BOM_UTF8, pszRootTag? pszRootTag : TEXT("root"), pRootAttr);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(NULL);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadXmlFromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileReader channel(strFilename.c_str());
			fmt_internal::CXMLDeserializer formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteXmlToString(const IFormatterObject* pObject, std::string& strOutput, LPCTSTR pszRootTag, std::map<std::tstring, std::tstring>* pRootAttr)
		{
			fmt_internal::CUTF8StringWriter channel(strOutput);
			fmt_internal::CXMLSerializer formatter(channel, BOM_UTF8, pszRootTag? pszRootTag : TEXT("root"), pRootAttr);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadXmlFromString(IFormatterObject* pObject, std::string strInput, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringReader channel(strInput);
			fmt_internal::CXMLDeserializer formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteXmlV2ToFile(const IFormatterObject* pObject, std::tstring strFilename, LPCTSTR pszRootTag, std::map<std::tstring, std::tstring>* pRootAttr)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename.c_str());
			fmt_internal::CXMLSerializer_V2 formatter(channel, BOM_UTF8, pszRootTag? pszRootTag : TEXT("root"), pRootAttr);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(NULL);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteXmlV2ToFile(const IFormatterObject* pObject, std::tstring strFilename, bool bWriteBOM, LPCTSTR pszRootTag, std::map<std::tstring, std::tstring>* pRootAttr)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename.c_str(), bWriteBOM);
			fmt_internal::CXMLSerializer_V2 formatter(channel, BOM_UTF8, pszRootTag? pszRootTag : TEXT("root"), pRootAttr);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(NULL);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadXmlV2FromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileReader channel(strFilename.c_str());
			fmt_internal::CXMLDeserializer_V2 formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteXmlV2ToString(const IFormatterObject* pObject, std::string& strOutput, LPCTSTR pszRootTag, std::map<std::tstring, std::tstring>* pRootAttr)
		{
			fmt_internal::CUTF8StringWriter channel(strOutput);
			fmt_internal::CXMLSerializer_V2 formatter(channel, BOM_UTF8, pszRootTag? pszRootTag : TEXT("root"), pRootAttr);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadXmlV2FromString(IFormatterObject* pObject, std::string strInput, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringReader channel(strInput);
			fmt_internal::CXMLDeserializer_V2 formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteXmlV3ToFile(const IFormatterObject* pObject, std::tstring strFilename, LPCTSTR pszRootTag)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename);
			fmt_internal::CXMLSerializer_V3 formatter(channel, GetCurrentBOM(), pszRootTag? pszRootTag : TEXT("root"));
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(NULL);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteXmlV3ToFile(const IFormatterObject* pObject, std::tstring strFilename, bool bWriteBOM, LPCTSTR pszRootTag)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename, bWriteBOM);
			fmt_internal::CXMLSerializer_V3 formatter(channel, GetCurrentBOM(), pszRootTag? pszRootTag : TEXT("root"));
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(NULL);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteXmlV3ToString(const IFormatterObject* pObject, std::string& strOutput, LPCTSTR pszRootTag)
		{
			fmt_internal::CUTF8StringWriter channel(strOutput);
			fmt_internal::CXMLSerializer_V3 formatter(channel, GetCurrentBOM(), pszRootTag? pszRootTag : TEXT("root"));
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(NULL);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteMarkerToFile(const IFormatterObject* pObject, std::tstring strSchema, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename.c_str());
			fmt_internal::CMarkerSerializer formatter(channel, strSchema);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteMarkerToFile(const IFormatterObject* pObject, std::tstring strSchema, std::tstring strFilename, bool bWriteBOM)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename.c_str(), bWriteBOM);
			fmt_internal::CMarkerSerializer formatter(channel, strSchema);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(NULL);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteMarkerToString(const IFormatterObject* pObject, std::tstring strSchema, std::string& strOutput, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringWriter channel(strOutput);
			fmt_internal::CMarkerSerializer formatter(channel, strSchema);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		void WriteKeyValueArrayToString(const IFormatterObject* pObject, std::tstring strSep, std::tstring strSchema, std::string& strOutput, bool bUseHex)
		{
			fmt_internal::CUTF8StringWriter channel(strOutput);
			fmt_internal::CKeyValueArraySerializer formatter(channel, strSep, strSchema, bUseHex);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
		}

		//////////////////////////////////////////////////////////////////////////	
		bool WriteKeyValueArrayToFile(const IFormatterObject* pObject, std::tstring strSep, std::tstring strSchema, std::tstring strFilename, bool bUseHex, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename);
			fmt_internal::CKeyValueArraySerializer formatter(channel, strSep, strSchema, bUseHex);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////	
		bool WriteKeyValueArrayToFile(const IFormatterObject* pObject, std::tstring strSep, std::tstring strSchema, std::tstring strFilename, bool bWriteBOM, bool bUseHex)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename, bWriteBOM);
			fmt_internal::CKeyValueArraySerializer formatter(channel, strSep, strSchema, bUseHex);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(NULL);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadKeyValueArrayFromString(IFormatterObject* pObject, std::tstring strTokenDelimiter, std::tstring strSpliter, std::string strContext, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringReader channel(strContext);
			fmt_internal::CKeyValueArrayDeserializer formatter(channel, strTokenDelimiter, strSpliter);
			pObject->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadKeyValueArrayFromFile(IFormatterObject* pObject, std::tstring strTokenDelimiter, std::tstring strSpliter, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileReader channel(strFilename);
			fmt_internal::CKeyValueArrayDeserializer formatter(channel, strTokenDelimiter, strSpliter);
			pObject->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		////////////////////////////////////////////////////////////////////////////
		//void WriteValueArrayToString(const IFormatterObject* pObject, std::tstring strSep, std::tstring strQuo, std::string& strOutput)
		//{
		//	fmt_internal::CUTF8StringWritter channel(strOutput);
		//	fmt_internal::CValueArraySerializer formatter(channel, strSep, strQuo);
		//	const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
		//}

		////////////////////////////////////////////////////////////////////////////
		//bool ReadValueArrayFromString(IFormatterObject* pObject, std::string strSep, std::string strQuo, std::vector<std::tstring> vecKey, bool bIgnoreCase, std::string strContext, std::tstring* pStrErrMsg)
		//{
		//	fmt_internal::CUTF8StringReader channel(strContext);
		//	fmt_internal::CValueArrayDeserializer formatter(channel, strSep, strQuo, vecKey, bIgnoreCase);
		//	pObject->Synchronize(formatter);
		//	return formatter.CheckValidity(pStrErrMsg);
		//}

		////////////////////////////////////////////////////////////////////////////
		//bool ReadValueArrayFromArray(IFormatterObject* pObject, std::vector<std::tstring> vecKey, std::vector<std::tstring> vecValue, bool bIgnoreCase, std::tstring* pStrErrMsg)
		//{
		//	fmt_internal::CUTF8StringReader dummyChannel(TEXT(""));
		//	fmt_internal::CValueArrayDeserializer formatter(dummyChannel, vecKey, vecValue, bIgnoreCase);
		//	pObject->Synchronize(formatter);
		//	return formatter.CheckValidity(pStrErrMsg);
		//}

		//////////////////////////////////////////////////////////////////////////
		bool WriteIniToFile(const IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename.c_str());
			fmt_internal::CINISerializer formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteIniToFile(const IFormatterObject* pObject, std::tstring strFilename, bool bWriteBOM)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename.c_str(), bWriteBOM);
			fmt_internal::CINISerializer formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(NULL);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadIniFromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileReader channel(strFilename.c_str());
			fmt_internal::CINIDeserializer formatter(channel);
			pObject->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteIniToString(const IFormatterObject* pObject, std::string& strOutput, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringWriter channel(strOutput);
			fmt_internal::CINISerializer formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadIniFromString(IFormatterObject* pObject, std::string strInput, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringReader channel(strInput);
			fmt_internal::CINIDeserializer formatter(channel);
			pObject->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadIniSectionFromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring strSection, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileReader channel(strFilename.c_str());
			fmt_internal::CINIDeserializer formatter(channel, strSection);
			pObject->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadIniSectionFromString(IFormatterObject* pObject, std::string strInput, std::tstring strSection, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringReader channel(strInput);
			fmt_internal::CINIDeserializer formatter(channel, strSection);
			pObject->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		void WriteKeyArrayToString(const IFormatterObject* pObject, std::tstring strSep, std::tstring strQuo, std::string& strOutput)
		{
			fmt_internal::CUTF8StringWriter channel(strOutput);
			fmt_internal::CKeyArraySerializer formatter(channel, strSep, strQuo);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
		}

		//////////////////////////////////////////////////////////////////////////
		void WriteValueArrayToString(const IFormatterObject* pObject, std::tstring strSep, std::tstring strQuo, std::string& strOutput, bool bUseHex)
		{
			fmt_internal::CUTF8StringWriter channel(strOutput);
			fmt_internal::CValueArraySerializer formatter(channel, strSep, strQuo, bUseHex);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
		}

		//////////////////////////////////////////////////////////////////////////
		void WriteValueArrayToFile(const IFormatterObject* pObject, std::tstring strSep, std::tstring strQuo, std::tstring strFilename, bool bWriteBOM, bool bUseHex)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename, bWriteBOM);
			fmt_internal::CValueArraySerializer formatter(channel, strSep, strQuo, bUseHex);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
		}

		////////////////////////////////////////////////////////////////////////////
		//bool ReadKeyArrayFromString(const IFormatterObject* pObject, std::tstring strSep, std::tstring strQuo, std::tstring& strOutput)
		//{
		//	fmt_internal::CUTF8StringReader channel(strFilename);
		//	fmt_internal::CKeyArrayDeserializer formatter(channel, strTokenDelimiter, strSpliter);
		//	pObject->Synchronize(formatter);
		//	return formatter.CheckValidity(pStrErrMsg);
		//}

		//////////////////////////////////////////////////////////////////////////
		bool ReadValueArrayFromString(IFormatterObject* pObject, std::tstring strSep, std::tstring strQuo, std::string& strContext, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringReader channel(strContext);
			fmt_internal::CValueArrayDeserializer formatter(channel, strSep, strQuo);
			pObject->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadValueArrayFromFile(IFormatterObject* pObject, std::tstring strSep, std::tstring strQuo, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileReader channel(strFilename);
			fmt_internal::CValueArrayDeserializer formatter(channel, strSep, strQuo);
			pObject->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

	}
}
