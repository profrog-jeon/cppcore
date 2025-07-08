#include "stdafx.h"
#include "DBValueArraySerializer.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CDBValueArraySerializer::CDBValueArraySerializer(core::IChannel& channel)
		: CFormatterSuper(channel)
		, m_bFirst(true)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CDBValueArraySerializer::~CDBValueArraySerializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool CDBValueArraySerializer::OnPrepare(IFormatterObject* pObject, std::tstring& strErrMsg)
	{
		return m_Channel.CheckValidity(strErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CDBValueArraySerializer::OnBeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CDBValueArraySerializer::OnEndDictionary()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CDBValueArraySerializer::OnBeginArray(std::tstring& strKey, const size_t tSize)
	{
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CDBValueArraySerializer::OnBeginArrayItem(size_t tIndex, size_t tCount)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CDBValueArraySerializer::OnEndArrayItem(size_t tIndex, size_t tCount)
	{
		std::tstring strOutput = TEXT("\n");
		m_Channel.Access((void*)strOutput.c_str(), strOutput.length() * sizeof(TCHAR));

		m_bFirst = true;
	}

	//////////////////////////////////////////////////////////////////////////
	void CDBValueArraySerializer::OnEndArray()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CDBValueArraySerializer::OnBeginObject(std::tstring& strKey)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CDBValueArraySerializer::OnEndObject()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CDBValueArraySerializer::OnBeginRoot(std::tstring& strRootName)
	{
		m_bFirst = true;
	}

	//////////////////////////////////////////////////////////////////////////
	void CDBValueArraySerializer::OnEndRoot()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline static void DBValueArraySerializerMetaFunction(IChannel& channel, T* pValue, bool& bFirst)
	{
		std::tstring strValue = StringFrom(*pValue);
		std::tstring strOutput = strValue;
		if( bFirst )
			bFirst = false;
		else
			strOutput = TEXT(", ") + strOutput;
		channel.Access((void*)strOutput.c_str(), strOutput.length() * sizeof(TCHAR));
	}

	//////////////////////////////////////////////////////////////////////////
	inline static void DBValueArraySerializerMetaFunction(IChannel& channel, std::tstring* pValue, bool& bFirst)
	{
		std::tstring strValue = StringFrom(*pValue);
		std::tstring strOutput = TEXT("\'") + strValue + TEXT("\'");
		if (bFirst)
			bFirst = false;
		else
			strOutput = TEXT(", ") + strOutput;
		channel.Access((void*)strOutput.c_str(), strOutput.length() * sizeof(TCHAR));
	}

	//////////////////////////////////////////////////////////////////////////
	inline static void DBValueArraySerializerMetaFunction(IChannel& channel, std::ntstring* pValue, bool& bFirst)
	{
		std::tstring strValue = StringFrom(*pValue);
		std::tstring strOutput = TEXT("\'") + strValue + TEXT("\'");
		if (bFirst)
			bFirst = false;
		else
			strOutput = TEXT(", ") + strOutput;
		channel.Access((void*)strOutput.c_str(), strOutput.length() * sizeof(TCHAR));
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CDBValueArraySerializer::OnSync(std::tstring& strKey, std::tstring* pValue)
	{
		DBValueArraySerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CDBValueArraySerializer::OnSync(std::tstring & strKey, std::ntstring * pValue)
	{
		DBValueArraySerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CDBValueArraySerializer::OnSync(std::tstring& strKey, bool* pValue)
	{
		DBValueArraySerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CDBValueArraySerializer::OnSync(std::tstring& strKey, char* pValue)
	{
		DBValueArraySerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CDBValueArraySerializer::OnSync(std::tstring& strKey, short* pValue)
	{
		DBValueArraySerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CDBValueArraySerializer::OnSync(std::tstring& strKey, int32_t* pValue)
	{
		DBValueArraySerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CDBValueArraySerializer::OnSync(std::tstring& strKey, int64_t* pValue)
	{
		DBValueArraySerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CDBValueArraySerializer::OnSync(std::tstring& strKey, BYTE* pValue)
	{
		DBValueArraySerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CDBValueArraySerializer::OnSync(std::tstring& strKey, WORD* pValue)
	{
		DBValueArraySerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CDBValueArraySerializer::OnSync(std::tstring& strKey, DWORD* pValue)
	{
		DBValueArraySerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CDBValueArraySerializer::OnSync(std::tstring& strKey, QWORD* pValue)
	{
		DBValueArraySerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CDBValueArraySerializer::OnSync(std::tstring& strKey, float* pValue)
	{
		DBValueArraySerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CDBValueArraySerializer::OnSync(std::tstring& strKey, double* pValue)
	{
		DBValueArraySerializerMetaFunction(m_Channel, pValue, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CDBValueArraySerializer::OnSync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		// Ignore
		return *this;
	}
}
