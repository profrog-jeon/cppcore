#include "stdafx.h"
#include "XMLSerializer_V2.h"
#include "XMLFunctions.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CXMLSerializer_V2::CXMLSerializer_V2(core::IChannel& channel, E_BOM_TYPE nBOM, std::tstring strRoot, std::map<std::tstring, std::tstring>* pRootAttr)
	: IFormatter(channel)
	, m_nBOM(nBOM)
	, m_stkElement()
	, m_stkGroupingData()
	, m_strRootTag(strRoot)
	, m_strRootAttr()
	{
		if( pRootAttr )
		{
			std::map<std::tstring, std::tstring>::iterator iter;
			for(iter=pRootAttr->begin(); iter!=pRootAttr->end(); iter++)
				m_strRootAttr += Format(TEXT(" %s=\'%s\' "), iter->first.c_str(), iter->second.c_str());
		}
	}

	//////////////////////////////////////////////////////////////////////////
	CXMLSerializer_V2::~CXMLSerializer_V2(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	static inline void __WriteXmlThroughChannel(size_t tIndent, std::tstring strContext, IChannel& channel)
	{
		// Build TAB indentation
		const size_t tMaxIndex = 32;
		TCHAR szIndent[tMaxIndex+1] = {
			TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), 
			TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), 
			TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), 
			TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t')	};

		if( tIndent <= tMaxIndex )
			szIndent[tIndent] = 0;

		// Insert TAB indentation
		strContext = szIndent + strContext + TEXT("\r\n");

		channel.Access((void*)strContext.c_str(), strContext.length() * sizeof(TCHAR));
	}

	//////////////////////////////////////////////////////////////////////////
	static inline void __WriteXmlThroughChannel(size_t tIndent, sGroupingData& refGroupData, std::tstring strKey, std::tstring strValue, IChannel& channel)
	{
		// Build TAB indentation
		const size_t tMaxIndex = 32;
		TCHAR szIndent[tMaxIndex+1] = {
			TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), 
			TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), 
			TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), 
			TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t'), TEXT('\t')	};

		if( tIndent <= tMaxIndex )
			szIndent[tIndent] = 0;

		// Insert TAB indentation
		std::tstring strContext = Format(TEXT("%s<%s>%s</%s>\r\n"), szIndent, strKey.c_str(), strValue.c_str(), strKey.c_str());
		if( GT_DICTIONARY == refGroupData.nType )
		{
			std::tstring strSecond;
			std::tstring strFirst = Split(strKey, TEXT("="), &strSecond);
			if( !strSecond.empty() )
				strContext = Format(TEXT("%s<%s %s=\"%s\">%s</%s>\r\n"), szIndent, refGroupData.strName.c_str(), strFirst.c_str(), strSecond.c_str(), strValue.c_str(), refGroupData.strName.c_str());
		}

		channel.Access((void*)strContext.c_str(), strContext.length() * sizeof(TCHAR));
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CXMLSerializer_V2::BeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		m_stkGroupingData.push(sGroupingData(strKey, GT_DICTIONARY, tSize));
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CXMLSerializer_V2::EndDictionary()
	{
		m_stkGroupingData.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CXMLSerializer_V2::BeginArray(std::tstring& strKey, const size_t tSize)
	{
		m_stkGroupingData.push(sGroupingData(strKey, GT_ARRAY, tSize));
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CXMLSerializer_V2::EndArray()
	{
		m_stkGroupingData.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	void CXMLSerializer_V2::BeginObject(std::tstring& strKey)
	{
		std::tstring strContext = TEXT("<") + strKey + TEXT(">");
		__WriteXmlThroughChannel(m_stkElement.size(), strContext, m_Channel);
		m_stkElement.push(strKey);
		m_stkGroupingData.push(sGroupingData(strKey, GT_OBJECT, 0));
	}

	//////////////////////////////////////////////////////////////////////////
	void CXMLSerializer_V2::EndObject()
	{
		std::tstring strKey = m_stkElement.top();
		m_stkElement.pop();
		m_stkGroupingData.pop();

		std::tstring strContext = TEXT("</") + strKey + TEXT(">");
		__WriteXmlThroughChannel(m_stkElement.size(), strContext, m_Channel);
	}

	//////////////////////////////////////////////////////////////////////////
	void CXMLSerializer_V2::BeginRoot()
	{
		LPCTSTR pszEncoding = NULL;
		switch(m_nBOM)
		{
		case BOM_UTF32_BE	:		pszEncoding = TEXT("UTF-32BE");		break;
		case BOM_UTF32		:		pszEncoding = TEXT("UTF-32");		break;
		case BOM_UTF16_BE	:		pszEncoding = TEXT("UTF-16BE");		break;
		case BOM_UTF16		:		pszEncoding = TEXT("UTF-16");		break;
		case BOM_UTF8		:		pszEncoding = TEXT("UTF-8");		break;
		case BOM_UTF_EBCDIC	:		pszEncoding = TEXT("UTF-EBCDIC");	break;
		case BOM_GB_18030	:		pszEncoding = TEXT("GB-18030");		break;
		case BOM_UTF1		:		pszEncoding = TEXT("UTF-1");		break;
		case BOM_SCSU		:		pszEncoding = TEXT("SCSU");			break;
		case BOM_BOCU_1		:		pszEncoding = TEXT("BOCU-1");		break;
		default:					pszEncoding = TEXT("EUC-KR");		break;
		}

		std::tstring strHeader = Format(TEXT("<?xml version=\"1.0\" encoding=\"%s\"?>"), pszEncoding);
		__WriteXmlThroughChannel(m_stkGroupingData.size(), strHeader, m_Channel);

		std::tstring strContext = TEXT("<") + m_strRootTag + m_strRootAttr + TEXT(">");
		__WriteXmlThroughChannel(m_stkGroupingData.size(), strContext, m_Channel);
		m_stkElement.push(m_strRootTag);
		m_stkGroupingData.push(sGroupingData(m_strRootTag, GT_ROOT, 0));
	}

	//////////////////////////////////////////////////////////////////////////
	void CXMLSerializer_V2::EndRoot()
	{
		std::tstring strKey = m_stkElement.top();
		m_stkElement.pop();
		m_stkGroupingData.pop();

		std::tstring strContext = TEXT("</") + strKey + TEXT(">");
		__WriteXmlThroughChannel(m_stkElement.size(), strContext, m_Channel);
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V2::Sync(std::tstring& strKey, std::tstring* pValue)
	{
		std::tstring strValue = EncodeXmlString(StringFrom(*pValue));
		__WriteXmlThroughChannel(m_stkElement.size(), m_stkGroupingData.top(), strKey, strValue, m_Channel);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT & CXMLSerializer_V2::Sync(std::tstring & strKey, std::ntstring * pValue)
	{
		std::tstring strValue = EncodeXmlString(StringFrom(*pValue));
		__WriteXmlThroughChannel(m_stkElement.size(), m_stkGroupingData.top(), strKey, strValue, m_Channel);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V2::Sync(std::tstring& strKey, bool* pValue)
	{
		std::tstring strValue = EncodeXmlString(StringFrom(*pValue));
		__WriteXmlThroughChannel(m_stkElement.size(), m_stkGroupingData.top(), strKey, strValue, m_Channel);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V2::Sync(std::tstring& strKey, char* pValue)
	{
		std::tstring strValue = EncodeXmlString(StringFrom(*pValue));
		__WriteXmlThroughChannel(m_stkElement.size(), m_stkGroupingData.top(), strKey, strValue, m_Channel);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V2::Sync(std::tstring& strKey, short* pValue)
	{
		std::tstring strValue = EncodeXmlString(StringFrom(*pValue));
		__WriteXmlThroughChannel(m_stkElement.size(), m_stkGroupingData.top(), strKey, strValue, m_Channel);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V2::Sync(std::tstring& strKey, int32_t* pValue)
	{
		std::tstring strValue = EncodeXmlString(StringFrom(*pValue));
		__WriteXmlThroughChannel(m_stkElement.size(), m_stkGroupingData.top(), strKey, strValue, m_Channel);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V2::Sync(std::tstring& strKey, int64_t* pValue)
	{
		std::tstring strValue = EncodeXmlString(StringFrom(*pValue));
		__WriteXmlThroughChannel(m_stkElement.size(), m_stkGroupingData.top(), strKey, strValue, m_Channel);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V2::Sync(std::tstring& strKey, BYTE* pValue)
	{
		std::tstring strValue = EncodeXmlString(StringFrom(*pValue));
		__WriteXmlThroughChannel(m_stkElement.size(), m_stkGroupingData.top(), strKey, strValue, m_Channel);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V2::Sync(std::tstring& strKey, WORD* pValue)
	{
		std::tstring strValue = EncodeXmlString(StringFrom(*pValue));
		__WriteXmlThroughChannel(m_stkElement.size(), m_stkGroupingData.top(), strKey, strValue, m_Channel);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V2::Sync(std::tstring& strKey, DWORD* pValue)
	{
		std::tstring strValue = EncodeXmlString(StringFrom(*pValue));
		__WriteXmlThroughChannel(m_stkElement.size(), m_stkGroupingData.top(), strKey, strValue, m_Channel);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V2::Sync(std::tstring& strKey, QWORD* pValue)
	{
		std::tstring strValue = EncodeXmlString(StringFrom(*pValue));
		__WriteXmlThroughChannel(m_stkElement.size(), m_stkGroupingData.top(), strKey, strValue, m_Channel);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V2::Sync(std::tstring& strKey, float* pValue)
	{
		std::tstring strValue = EncodeXmlString(StringFrom(*pValue));
		__WriteXmlThroughChannel(m_stkElement.size(), m_stkGroupingData.top(), strKey, strValue, m_Channel);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V2::Sync(std::tstring& strKey, double* pValue)
	{
		std::tstring strValue = EncodeXmlString(StringFrom(*pValue));
		__WriteXmlThroughChannel(m_stkElement.size(), m_stkGroupingData.top(), strKey, strValue, m_Channel);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V2::Sync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		// Ignore
		return *this;
	}
}

