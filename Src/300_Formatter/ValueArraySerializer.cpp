#include "stdafx.h"
#include "ValueArraySerializer.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CValueArraySerializer::CValueArraySerializer(core::IChannel& channel, std::tstring strSeperator, std::tstring strQuotator, bool bUseHex)
		: CFormatterSuper(channel)
		, m_strSeperator(strSeperator)
		, m_strQuotator(strQuotator)
		, m_bFirst(true)
		, m_bUseHexValue(bUseHex)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CValueArraySerializer::~CValueArraySerializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool CValueArraySerializer::OnPrepare(IFormatterObject* pObject, std::tstring& strErrMsg)
	{
		return m_Channel.CheckValidity(strErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CValueArraySerializer::OnBeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArraySerializer::OnEndDictionary()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CValueArraySerializer::OnBeginArray(std::tstring& strKey, const size_t tSize)
	{
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArraySerializer::OnBeginArrayItem(size_t tIndex, size_t tCount)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArraySerializer::OnEndArrayItem(size_t tIndex, size_t tCount)
	{
		std::tstring strOutput = TEXT("\n");
		m_Channel.Access((void*)strOutput.c_str(), strOutput.length() * sizeof(TCHAR));

		m_bFirst = true;
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArraySerializer::OnEndArray()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArraySerializer::OnBeginObject(std::tstring& strKey)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArraySerializer::OnEndObject()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArraySerializer::OnBeginRoot(std::tstring& strRootName)
	{
		m_bFirst = true;
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArraySerializer::OnEndRoot()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline static void ValueArraySerializerMetaFunction(IChannel& channel, std::tstring strSeperator, std::tstring strQuotator, T* pValue, bool& bFirst, bool bUseHex)
	{
		std::tstring strValue = bUseHex? HexFrom(*pValue) : StringFrom(*pValue);
		std::tstring strOutput = strQuotator + strValue + strQuotator;
		if( bFirst )
			bFirst = false;
		else
			strOutput = strSeperator + strOutput;
		channel.Access((void*)strOutput.c_str(), strOutput.length() * sizeof(TCHAR));
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArraySerializer::OnSync(std::tstring& strKey, std::tstring* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArraySerializer::OnSync(std::tstring & strKey, std::ntstring * pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArraySerializer::OnSync(std::tstring& strKey, bool* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArraySerializer::OnSync(std::tstring& strKey, char* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArraySerializer::OnSync(std::tstring& strKey, short* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArraySerializer::OnSync(std::tstring& strKey, int32_t* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArraySerializer::OnSync(std::tstring& strKey, int64_t* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArraySerializer::OnSync(std::tstring& strKey, BYTE* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArraySerializer::OnSync(std::tstring& strKey, WORD* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArraySerializer::OnSync(std::tstring& strKey, DWORD* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArraySerializer::OnSync(std::tstring& strKey, QWORD* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArraySerializer::OnSync(std::tstring& strKey, float* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArraySerializer::OnSync(std::tstring& strKey, double* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArraySerializer::OnSync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		// Ignore
		return *this;
	}
}
