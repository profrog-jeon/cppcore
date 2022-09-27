#include "stdafx.h"
#include "ValueArraySerializer.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CValueArraySerializer::CValueArraySerializer(core::IChannel& channel, std::tstring strSeperator, std::tstring strQuotator, bool bUseHex)
		: IFormatter(channel)
		, m_strErrMsg()
		, m_strSeperator(strSeperator)
		, m_strQuotator(strQuotator)
		, m_bValidity(false)
		, m_bFirst(true)
		, m_bUseHexValue(bUseHex)
	{
		m_bValidity = channel.CheckValidity(m_strErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	CValueArraySerializer::~CValueArraySerializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CValueArraySerializer::BeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArraySerializer::EndDictionary()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CValueArraySerializer::BeginArray(std::tstring& strKey, const size_t tSize)
	{
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArraySerializer::BeginArrayItem(size_t tIndex, size_t tCount)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArraySerializer::EndArrayItem(size_t tIndex, size_t tCount)
	{
		std::tstring strOutput = TEXT("\n");
		m_Channel.Access((void*)strOutput.c_str(), strOutput.length() * sizeof(TCHAR));

		m_bFirst = true;
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArraySerializer::EndArray()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArraySerializer::BeginObject(std::tstring& strKey)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArraySerializer::EndObject()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArraySerializer::BeginRoot()
	{
		m_bFirst = true;
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArraySerializer::EndRoot()
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
	core::IFormatterT& CValueArraySerializer::Sync(std::tstring& strKey, std::tstring* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT & CValueArraySerializer::Sync(std::tstring & strKey, std::ntstring * pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArraySerializer::Sync(std::tstring& strKey, bool* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArraySerializer::Sync(std::tstring& strKey, char* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArraySerializer::Sync(std::tstring& strKey, short* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArraySerializer::Sync(std::tstring& strKey, int32_t* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArraySerializer::Sync(std::tstring& strKey, int64_t* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArraySerializer::Sync(std::tstring& strKey, BYTE* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArraySerializer::Sync(std::tstring& strKey, WORD* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArraySerializer::Sync(std::tstring& strKey, DWORD* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArraySerializer::Sync(std::tstring& strKey, QWORD* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArraySerializer::Sync(std::tstring& strKey, float* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArraySerializer::Sync(std::tstring& strKey, double* pValue)
	{
		ValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, pValue, m_bFirst, m_bUseHexValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArraySerializer::Sync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		// Ignore
		return *this;
	}
}
