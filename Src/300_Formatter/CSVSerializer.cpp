#include "stdafx.h"
#include "CSVSerializer.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CCSVSerializer::CCSVSerializer(core::IChannel& channel)
		: IFormatter(channel)
		, m_strErrMsg()
		, m_bValidity(false)
		, m_bFirst(true)
	{
		m_bValidity = channel.CheckValidity(m_strErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	CCSVSerializer::~CCSVSerializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CCSVSerializer::BeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVSerializer::EndDictionary()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CCSVSerializer::BeginArray(std::tstring& strKey, const size_t tSize)
	{
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVSerializer::BeginArrayItem(size_t tIndex, size_t tCount)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVSerializer::EndArrayItem(size_t tIndex, size_t tCount)
	{
		std::tstring strOutput = TEXT("\r\n");
		m_Channel.Access((void*)strOutput.c_str(), strOutput.length() * sizeof(TCHAR));

		m_bFirst = true;
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVSerializer::EndArray()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVSerializer::BeginObject(std::tstring& strKey)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVSerializer::EndObject()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVSerializer::BeginRoot()
	{
		m_bFirst = true;
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVSerializer::EndRoot()
	{
	}	

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline void __CSVSerializerMetaFunction(IChannel& channel, T* pValue, bool& bFirst)
	{
		std::tstring strValue = WrapupSpecialChar(StringFrom(*pValue), ',', '\"');

		if( bFirst )
			bFirst = false;
		else
			strValue = TEXT(",") + strValue;
		channel.Access((void*)strValue.c_str(), strValue.length() * sizeof(TCHAR));
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVSerializer::Sync(std::tstring& strKey, std::tstring* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT & CCSVSerializer::Sync(std::tstring & strKey, std::ntstring * pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVSerializer::Sync(std::tstring& strKey, bool* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVSerializer::Sync(std::tstring& strKey, char* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVSerializer::Sync(std::tstring& strKey, short* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVSerializer::Sync(std::tstring& strKey, int32_t* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVSerializer::Sync(std::tstring& strKey, int64_t* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVSerializer::Sync(std::tstring& strKey, BYTE* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVSerializer::Sync(std::tstring& strKey, WORD* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVSerializer::Sync(std::tstring& strKey, DWORD* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVSerializer::Sync(std::tstring& strKey, QWORD* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVSerializer::Sync(std::tstring& strKey, float* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVSerializer::Sync(std::tstring& strKey, double* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVSerializer::Sync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		// Ignore
		return *this;
	}
}
