#include "stdafx.h"
#include "KeyArraySerializer.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CKeyArraySerializer::CKeyArraySerializer(core::IChannel& channel, std::tstring strSeperator, std::tstring strQuotator)
		: CFormatterSuper(channel)
		, m_strSeperator(strSeperator)
		, m_strQuotator(strQuotator)
		, m_bWrite(true)
		, m_bFirst(true)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CKeyArraySerializer::~CKeyArraySerializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool CKeyArraySerializer::OnPrepare(IFormatterObject* pObject, std::tstring& strErrMsg)
	{
		return m_Channel.CheckValidity(strErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CKeyArraySerializer::OnBeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyArraySerializer::OnEndDictionary()
	{
		m_bWrite = false;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CKeyArraySerializer::OnBeginArray(std::tstring& strKey, const size_t tSize)
	{
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyArraySerializer::OnBeginArrayItem(size_t tIndex, size_t tCount)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyArraySerializer::OnEndArrayItem(size_t tIndex, size_t tCount)
	{
		m_bWrite = false;
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyArraySerializer::OnEndArray()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyArraySerializer::OnBeginObject(std::tstring& strKey)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyArraySerializer::OnEndObject()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyArraySerializer::OnBeginRoot(std::tstring& strRootName)
	{
		m_bFirst = true;
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyArraySerializer::OnEndRoot()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	inline void __KeySerializerMetaFunction(IChannel& channel, std::tstring strSeperator, std::tstring strQuotator, std::tstring strKey, bool bWrite, bool& bFirst)
	{
		if( !bWrite )
			return;

		std::tstring strOutput = strQuotator + strKey + strQuotator;
		if( bFirst )
			bFirst = false;
		else
			strOutput = strSeperator + strOutput;
		channel.Access((void*)strOutput.c_str(), strOutput.length() * sizeof(TCHAR));
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyArraySerializer::OnSync(std::tstring& strKey, std::tstring* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyArraySerializer::OnSync(std::tstring & strKey, std::ntstring * pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyArraySerializer::OnSync(std::tstring& strKey, bool* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyArraySerializer::OnSync(std::tstring& strKey, char* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyArraySerializer::OnSync(std::tstring& strKey, short* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyArraySerializer::OnSync(std::tstring& strKey, int32_t* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyArraySerializer::OnSync(std::tstring& strKey, int64_t* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyArraySerializer::OnSync(std::tstring& strKey, BYTE* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyArraySerializer::OnSync(std::tstring& strKey, WORD* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyArraySerializer::OnSync(std::tstring& strKey, DWORD* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyArraySerializer::OnSync(std::tstring& strKey, QWORD* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyArraySerializer::OnSync(std::tstring& strKey, float* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyArraySerializer::OnSync(std::tstring& strKey, double* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyArraySerializer::OnSync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		// Ignore
		return *this;
	}
}
