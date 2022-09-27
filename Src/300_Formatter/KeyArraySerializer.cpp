#include "stdafx.h"
#include "KeyArraySerializer.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CKeyArraySerializer::CKeyArraySerializer(core::IChannel& channel, std::tstring strSeperator, std::tstring strQuotator)
		: IFormatter(channel)
		, m_strErrMsg()
		, m_strSeperator(strSeperator)
		, m_strQuotator(strQuotator)
		, m_bValidity(false)
		, m_bWrite(true)
		, m_bFirst(true)
	{
		m_bValidity = channel.CheckValidity(m_strErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	CKeyArraySerializer::~CKeyArraySerializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CKeyArraySerializer::BeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyArraySerializer::EndDictionary()
	{
		m_bWrite = false;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CKeyArraySerializer::BeginArray(std::tstring& strKey, const size_t tSize)
	{
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyArraySerializer::BeginArrayItem(size_t tIndex, size_t tCount)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyArraySerializer::EndArrayItem(size_t tIndex, size_t tCount)
	{
		m_bWrite = false;
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyArraySerializer::EndArray()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyArraySerializer::BeginObject(std::tstring& strKey)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyArraySerializer::EndObject()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyArraySerializer::BeginRoot()
	{
		m_bFirst = true;
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyArraySerializer::EndRoot()
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
	core::IFormatterT& CKeyArraySerializer::Sync(std::tstring& strKey, std::tstring* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT & CKeyArraySerializer::Sync(std::tstring & strKey, std::ntstring * pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyArraySerializer::Sync(std::tstring& strKey, bool* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyArraySerializer::Sync(std::tstring& strKey, char* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyArraySerializer::Sync(std::tstring& strKey, short* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyArraySerializer::Sync(std::tstring& strKey, int32_t* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyArraySerializer::Sync(std::tstring& strKey, int64_t* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyArraySerializer::Sync(std::tstring& strKey, BYTE* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyArraySerializer::Sync(std::tstring& strKey, WORD* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyArraySerializer::Sync(std::tstring& strKey, DWORD* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyArraySerializer::Sync(std::tstring& strKey, QWORD* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyArraySerializer::Sync(std::tstring& strKey, float* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyArraySerializer::Sync(std::tstring& strKey, double* pValue)
	{
		__KeySerializerMetaFunction(m_Channel, m_strSeperator, m_strQuotator, strKey, m_bWrite, m_bFirst);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CKeyArraySerializer::Sync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		// Ignore
		return *this;
	}
}
