#include "stdafx.h"
#include "XMLSerializer_V3.h"
#include "XMLFunctions.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CXMLSerializer_V3::CXMLSerializer_V3(core::IChannel& channel, E_BOM_TYPE nBOM, std::tstring strRoot)
	: IFormatter(channel)
	, m_nBOM(nBOM)
	, m_stkTraverse()
	, m_stkContext()
	, m_strRootTag(strRoot)
	, m_strAttrContext()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CXMLSerializer_V3::~CXMLSerializer_V3(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	static inline std::tstring BuildXmlEndContext(size_t tIndent, std::tstring strKey)
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
		return Format(TEXT("%s</%s>"), szIndent, strKey.c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring CXMLSerializer_V3::BuildXmlContext(size_t tIndent, const ST_INTERNAL_TREE_DATA& stData)
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
		return Format(TEXT("%s<%s%s>%s"), szIndent, stData.strKey.c_str(), stData.strAttr.c_str(), stData.strValue.c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CXMLSerializer_V3::BeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		m_stkContext.push(BuildXmlEndContext(m_stkTraverse.size(), strKey));
		m_stkTraverse.push(ST_INTERNAL_TREE_DATA(strKey));
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CXMLSerializer_V3::EndDictionary()
	{
		ST_INTERNAL_TREE_DATA stData = m_stkTraverse.top();
		m_stkTraverse.pop();

		m_stkContext.push(BuildXmlContext(m_stkTraverse.size(), stData));
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CXMLSerializer_V3::BeginArray(std::tstring& strKey, const size_t tSize)
	{
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CXMLSerializer_V3::EndArray()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CXMLSerializer_V3::BeginObject(std::tstring& strKey)
	{
		m_stkContext.push(BuildXmlEndContext(m_stkTraverse.size(), strKey));
		m_stkTraverse.push(ST_INTERNAL_TREE_DATA(strKey));
	}

	//////////////////////////////////////////////////////////////////////////
	void CXMLSerializer_V3::EndObject()
	{
		ST_INTERNAL_TREE_DATA stData = m_stkTraverse.top();
		m_stkTraverse.pop();
		
		m_stkContext.push(BuildXmlContext(m_stkTraverse.size(), stData));
	}

	//////////////////////////////////////////////////////////////////////////
	void CXMLSerializer_V3::BeginRoot()
	{
		m_stkContext.push(BuildXmlEndContext(m_stkTraverse.size(), m_strRootTag));
		m_stkTraverse.push(ST_INTERNAL_TREE_DATA(m_strRootTag));
	}

	//////////////////////////////////////////////////////////////////////////
	void CXMLSerializer_V3::EndRoot()
	{
		ST_INTERNAL_TREE_DATA stData = m_stkTraverse.top();
		m_stkTraverse.pop();

		m_stkContext.push(BuildXmlContext(m_stkTraverse.size(), stData));

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

			m_stkContext.push(Format(TEXT("<?xml version=\"1.0\" encoding=\"%s\"?>"), pszEncoding));
		}

		while(!m_stkContext.empty())
		{
			std::tstring strContext = m_stkContext.top() + TEXT("\n");
			m_stkContext.pop();

			m_Channel.Access((void*)strContext.c_str(), strContext.length() * sizeof(TCHAR));
		}
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	static void UpdateAttrValue(std::tstring& strAttr, std::tstring strKey, T& aValue)
	{
		std::tstring strValue = EncodeXmlString(StringFrom(aValue));
		strAttr += Format(TEXT(" %s=\"%s\""), strKey.c_str(), strValue.c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V3::Sync(std::tstring& strKey, std::tstring* pValue)
	{
		if( TEXT("VALUE") == strKey )	{	m_stkTraverse.top().strValue = StringFrom(*pValue);	return *this;	}
		UpdateAttrValue(m_stkTraverse.top().strAttr, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT & CXMLSerializer_V3::Sync(std::tstring & strKey, std::ntstring * pValue)
	{
		if (TEXT("VALUE") == strKey) { m_stkTraverse.top().strValue = StringFrom(*pValue);	return *this; }
		UpdateAttrValue(m_stkTraverse.top().strAttr, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V3::Sync(std::tstring& strKey, bool* pValue)
	{
		if( TEXT("VALUE") == strKey )	{	m_stkTraverse.top().strValue = StringFrom(*pValue);	return *this;	}
		UpdateAttrValue(m_stkTraverse.top().strAttr, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V3::Sync(std::tstring& strKey, char* pValue)
	{
		if( TEXT("VALUE") == strKey )	{	m_stkTraverse.top().strValue = StringFrom(*pValue);	return *this;	}
		UpdateAttrValue(m_stkTraverse.top().strAttr, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V3::Sync(std::tstring& strKey, short* pValue)
	{
		if( TEXT("VALUE") == strKey )	{	m_stkTraverse.top().strValue = StringFrom(*pValue);	return *this;	}
		UpdateAttrValue(m_stkTraverse.top().strAttr, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V3::Sync(std::tstring& strKey, int32_t* pValue)
	{
		if( TEXT("VALUE") == strKey )	{	m_stkTraverse.top().strValue = StringFrom(*pValue);	return *this;	}
		UpdateAttrValue(m_stkTraverse.top().strAttr, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V3::Sync(std::tstring& strKey, int64_t* pValue)
	{
		if( TEXT("VALUE") == strKey )	{	m_stkTraverse.top().strValue = StringFrom(*pValue);	return *this;	}
		UpdateAttrValue(m_stkTraverse.top().strAttr, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V3::Sync(std::tstring& strKey, BYTE* pValue)
	{
		if( TEXT("VALUE") == strKey )	{	m_stkTraverse.top().strValue = StringFrom(*pValue);	return *this;	}
		UpdateAttrValue(m_stkTraverse.top().strAttr, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V3::Sync(std::tstring& strKey, WORD* pValue)
	{
		if( TEXT("VALUE") == strKey )	{	m_stkTraverse.top().strValue = StringFrom(*pValue);	return *this;	}
		UpdateAttrValue(m_stkTraverse.top().strAttr, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V3::Sync(std::tstring& strKey, DWORD* pValue)
	{
		if( TEXT("VALUE") == strKey )	{	m_stkTraverse.top().strValue = StringFrom(*pValue);	return *this;	}
		UpdateAttrValue(m_stkTraverse.top().strAttr, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V3::Sync(std::tstring& strKey, QWORD* pValue)
	{
		if( TEXT("VALUE") == strKey )	{	m_stkTraverse.top().strValue = StringFrom(*pValue);	return *this;	}
		UpdateAttrValue(m_stkTraverse.top().strAttr, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V3::Sync(std::tstring& strKey, float* pValue)
	{
		if( TEXT("VALUE") == strKey )	{	m_stkTraverse.top().strValue = StringFrom(*pValue);	return *this;	}
		UpdateAttrValue(m_stkTraverse.top().strAttr, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V3::Sync(std::tstring& strKey, double* pValue)
	{
		if( TEXT("VALUE") == strKey )	{	m_stkTraverse.top().strValue = StringFrom(*pValue);	return *this;	}
		UpdateAttrValue(m_stkTraverse.top().strAttr, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CXMLSerializer_V3::Sync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		// Ignore
		return *this;
	}
}

