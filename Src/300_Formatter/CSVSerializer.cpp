#include "stdafx.h"
#include "CSVSerializer.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CCSVSerializer::CCSVSerializer(core::IChannel& channel)
		: CFormatterSuper(channel)
		, m_bFirst(true)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CCSVSerializer::~CCSVSerializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool CCSVSerializer::OnPrepare(IFormatterObject* pObject, std::tstring& strErrMsg)
	{
		return m_Channel.CheckValidity(strErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CCSVSerializer::OnBeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVSerializer::OnEndDictionary()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CCSVSerializer::OnBeginArray(std::tstring& strKey, const size_t tSize)
	{
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVSerializer::OnBeginArrayItem(size_t tIndex, size_t tCount)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVSerializer::OnEndArrayItem(size_t tIndex, size_t tCount)
	{
		std::tstring strOutput = TEXT("\r\n");
		m_Channel.Access((void*)strOutput.c_str(), strOutput.length() * sizeof(TCHAR));

		m_bFirst = true;
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVSerializer::OnEndArray()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVSerializer::OnBeginObject(std::tstring& strKey)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVSerializer::OnEndObject()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVSerializer::OnBeginRoot(std::tstring& strRootName)
	{
		m_bFirst = true;
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVSerializer::OnEndRoot()
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
	core::IFormatter& CCSVSerializer::OnSync(std::tstring& strKey, std::tstring* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVSerializer::OnSync(std::tstring & strKey, std::ntstring * pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVSerializer::OnSync(std::tstring& strKey, bool* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVSerializer::OnSync(std::tstring& strKey, char* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVSerializer::OnSync(std::tstring& strKey, short* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVSerializer::OnSync(std::tstring& strKey, int32_t* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVSerializer::OnSync(std::tstring& strKey, int64_t* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVSerializer::OnSync(std::tstring& strKey, BYTE* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVSerializer::OnSync(std::tstring& strKey, WORD* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVSerializer::OnSync(std::tstring& strKey, DWORD* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVSerializer::OnSync(std::tstring& strKey, QWORD* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVSerializer::OnSync(std::tstring& strKey, float* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVSerializer::OnSync(std::tstring& strKey, double* pValue)
	{
		__CSVSerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVSerializer::OnSync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		// Ignore
		return *this;
	}
}
