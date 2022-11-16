#include "stdafx.h"
#include "INISerializer.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CINISerializer::CINISerializer(core::IChannel& channel)
		: IFormatter(channel)
		, m_stkSection()
		, m_INI()
		, m_strErrMsg()
		, m_dwArrayIndex(0)
		, m_bInArray(false)
		, m_bValidity(false)
	{
		m_bValidity = channel.CheckValidity(m_strErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	CINISerializer::~CINISerializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CINISerializer::BeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		m_stkSection.push(strKey);
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CINISerializer::EndDictionary()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CINISerializer::BeginArray(std::tstring& strKey, const size_t tSize)
	{
		m_bInArray = true;
		m_dwArrayIndex = 0;
		m_stkSection.push(strKey);
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CINISerializer::EndArray()
	{
		m_bInArray = false;
		m_stkSection.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	void CINISerializer::BeginObject(std::tstring& strKey)
	{
		m_stkSection.push(strKey);
	}

	//////////////////////////////////////////////////////////////////////////
	void CINISerializer::EndObject()
	{
		m_stkSection.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	void CINISerializer::BeginRoot()
	{
		m_stkSection.push(TEXT("DEFAULT"));
	}

	//////////////////////////////////////////////////////////////////////////
	void CINISerializer::EndRoot()
	{
		m_stkSection.pop();

		ECODE errRet = m_INI.Flush(m_Channel);
		if( EC_SUCCESS != errRet )
		{
			m_strErrMsg = Format(TEXT("INI.Flush operating failure, %d"), errRet);
			m_bValidity = false;
		}
	}


	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline void __INISerializerMetaFunction(CINIFormatter& ini, std::tstring strSection, std::tstring strKey, T* pValue, bool bArray, DWORD& dwArrayIndex)
	{
		if( bArray )
			strKey = Format(TEXT("INDEX%04u"), dwArrayIndex++);

		std::tstring strValue = StringFrom(*pValue);
		ini.SetValue(strSection.c_str(), strKey.c_str(), strValue.c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CINISerializer::Sync(std::tstring& strKey, std::tstring* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT & CINISerializer::Sync(std::tstring & strKey, std::ntstring * pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CINISerializer::Sync(std::tstring& strKey, bool* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CINISerializer::Sync(std::tstring& strKey, char* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CINISerializer::Sync(std::tstring& strKey, short* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CINISerializer::Sync(std::tstring& strKey, int32_t* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CINISerializer::Sync(std::tstring& strKey, int64_t* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CINISerializer::Sync(std::tstring& strKey, BYTE* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CINISerializer::Sync(std::tstring& strKey, WORD* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CINISerializer::Sync(std::tstring& strKey, DWORD* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CINISerializer::Sync(std::tstring& strKey, QWORD* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CINISerializer::Sync(std::tstring& strKey, float* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CINISerializer::Sync(std::tstring& strKey, double* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CINISerializer::Sync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		// Ignore
		return *this;
	}
}

