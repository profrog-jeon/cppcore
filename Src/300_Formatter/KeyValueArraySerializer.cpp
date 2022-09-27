#include "stdafx.h"
#include "KeyValueArraySerializer.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CKeyValueArraySerializer::CKeyValueArraySerializer(core::IChannel& channel, std::tstring strSeperator, std::tstring strKeyValueSchema, bool bUseHex)
		: IFormatter(channel)
		, m_strErrMsg()
		, m_strSeperator(strSeperator)
		, m_strSchema(strKeyValueSchema)
		, m_bValidity(false)
		, m_bFirst(true)
		, m_bUseHex(bUseHex)
	{
		m_bValidity = channel.CheckValidity(m_strErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	CKeyValueArraySerializer::~CKeyValueArraySerializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CKeyValueArraySerializer::BeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyValueArraySerializer::EndDictionary()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CKeyValueArraySerializer::BeginArray(std::tstring& strKey, const size_t tSize)
	{
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyValueArraySerializer::EndArray()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyValueArraySerializer::BeginObject(std::tstring& strKey)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyValueArraySerializer::EndObject()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyValueArraySerializer::BeginRoot()
	{
		m_bFirst = true;
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyValueArraySerializer::EndRoot()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline static void KeyValueArraySerializerMetaFunction(IChannel& channel, std::tstring strSeperator, std::tstring strSchema, std::tstring strKey, T* pValue, bool& bFirst, bool bUseHex)
	{
		std::tstring strOutput		= strSchema;
		std::tstring strValue		= bUseHex? HexFrom(*pValue) : StringFrom(*pValue);
		strOutput = Replace(strOutput, TEXT("[KEY]"),		strKey);
		strOutput = Replace(strOutput, TEXT("[VALUE]"),	strValue);

		if( bFirst )
			bFirst = false;
		else
			strOutput = strSeperator + strOutput;
		channel.Access((void*)strOutput.c_str(), strOutput.length() * sizeof(TCHAR));
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyValueArraySerializer::Sync(std::tstring& strKey, std::tstring* pValue)
	{
		KeyValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strSchema, strKey, pValue, m_bFirst, m_bUseHex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT & CKeyValueArraySerializer::Sync(std::tstring & strKey, std::ntstring * pValue)
	{
		KeyValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strSchema, strKey, pValue, m_bFirst, m_bUseHex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyValueArraySerializer::Sync(std::tstring& strKey, bool* pValue)
	{
		KeyValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strSchema, strKey, pValue, m_bFirst, m_bUseHex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyValueArraySerializer::Sync(std::tstring& strKey, char* pValue)
	{
		KeyValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strSchema, strKey, pValue, m_bFirst, m_bUseHex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyValueArraySerializer::Sync(std::tstring& strKey, short* pValue)
	{
		KeyValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strSchema, strKey, pValue, m_bFirst, m_bUseHex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyValueArraySerializer::Sync(std::tstring& strKey, int32_t* pValue)
	{
		KeyValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strSchema, strKey, pValue, m_bFirst, m_bUseHex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyValueArraySerializer::Sync(std::tstring& strKey, int64_t* pValue)
	{
		KeyValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strSchema, strKey, pValue, m_bFirst, m_bUseHex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyValueArraySerializer::Sync(std::tstring& strKey, BYTE* pValue)
	{
		KeyValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strSchema, strKey, pValue, m_bFirst, m_bUseHex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyValueArraySerializer::Sync(std::tstring& strKey, WORD* pValue)
	{
		KeyValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strSchema, strKey, pValue, m_bFirst, m_bUseHex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyValueArraySerializer::Sync(std::tstring& strKey, DWORD* pValue)
	{
		KeyValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strSchema, strKey, pValue, m_bFirst, m_bUseHex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyValueArraySerializer::Sync(std::tstring& strKey, QWORD* pValue)
	{
		KeyValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strSchema, strKey, pValue, m_bFirst, m_bUseHex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyValueArraySerializer::Sync(std::tstring& strKey, float* pValue)
	{
		KeyValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strSchema, strKey, pValue, m_bFirst, m_bUseHex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyValueArraySerializer::Sync(std::tstring& strKey, double* pValue)
	{
		KeyValueArraySerializerMetaFunction(m_Channel, m_strSeperator, m_strSchema, strKey, pValue, m_bFirst, m_bUseHex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyValueArraySerializer::Sync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		// Ignore
		return *this;
	}
}
