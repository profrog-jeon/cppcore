#include "stdafx.h"
#include "INISerializer.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CINISerializer::CINISerializer(core::IChannel& channel)
		: CFormatterSuper(channel)
		, m_stkSection()
		, m_INI()
		, m_dwArrayIndex(0)
		, m_bInArray(false)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CINISerializer::~CINISerializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool CINISerializer::OnPrepare(IFormatterObject* pObject, std::tstring& strErrMsg)
	{
		return m_Channel.CheckValidity(strErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CINISerializer::OnBeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		m_stkSection.push(strKey);
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CINISerializer::OnEndDictionary()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CINISerializer::OnBeginArray(std::tstring& strKey, const size_t tSize)
	{
		m_bInArray = true;
		m_dwArrayIndex = 0;
		m_stkSection.push(strKey);
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CINISerializer::OnEndArray()
	{
		m_bInArray = false;
		m_stkSection.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	void CINISerializer::OnBeginObject(std::tstring& strKey)
	{
		m_stkSection.push(strKey);
	}

	//////////////////////////////////////////////////////////////////////////
	void CINISerializer::OnEndObject()
	{
		m_stkSection.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	void CINISerializer::OnBeginRoot(std::tstring& strRootName)
	{
		m_stkSection.push(TEXT("DEFAULT"));
	}

	//////////////////////////////////////////////////////////////////////////
	void CINISerializer::OnEndRoot()
	{
		m_stkSection.pop();

		ECODE errRet = m_INI.Flush(m_Channel);
		if( EC_SUCCESS != errRet )
		{
			Log_Error(TEXT("INI.Flush operating failure, %d"), errRet);
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
	core::IFormatter& CINISerializer::OnSync(std::tstring& strKey, std::tstring* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINISerializer::OnSync(std::tstring & strKey, std::ntstring * pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINISerializer::OnSync(std::tstring& strKey, bool* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINISerializer::OnSync(std::tstring& strKey, char* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINISerializer::OnSync(std::tstring& strKey, short* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINISerializer::OnSync(std::tstring& strKey, int32_t* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINISerializer::OnSync(std::tstring& strKey, int64_t* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINISerializer::OnSync(std::tstring& strKey, BYTE* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINISerializer::OnSync(std::tstring& strKey, WORD* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINISerializer::OnSync(std::tstring& strKey, DWORD* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINISerializer::OnSync(std::tstring& strKey, QWORD* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINISerializer::OnSync(std::tstring& strKey, float* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINISerializer::OnSync(std::tstring& strKey, double* pValue)
	{
		__INISerializerMetaFunction(m_INI, m_stkSection.top(), strKey, pValue, m_bInArray, m_dwArrayIndex);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINISerializer::OnSync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		// Ignore
		return *this;
	}
}

