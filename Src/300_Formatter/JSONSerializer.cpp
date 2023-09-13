#include "stdafx.h"
#include "JSONSerializer.h"
#include "JSONFunctions.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	inline void AddOpeningToken(std::vector<sGroupingData>& vecObjectCountStack, core::IChannel& channel, TCHAR cToken)
	{
		channel.Access(&cToken, sizeof(TCHAR));

		TCHAR cDelToken;
		cDelToken = TEXT('\r');
		channel.Access(&cDelToken, sizeof(TCHAR));
		cDelToken = TEXT('\n');
		channel.Access(&cDelToken, sizeof(TCHAR));
	}

	//////////////////////////////////////////////////////////////////////////
	inline void AddClosingToken(std::vector<sGroupingData>& vecObjectCountStack, core::IChannel& channel, TCHAR cToken)
	{
		TCHAR cDelToken;
		cDelToken = TEXT('\r');
		channel.Access(&cDelToken, sizeof(TCHAR));
		cDelToken = TEXT('\n');
		channel.Access(&cDelToken, sizeof(TCHAR));

		size_t i;
		for(i=0; i<vecObjectCountStack.size(); i++)
		{
			cDelToken = TEXT('\t');
			channel.Access(&cDelToken, sizeof(TCHAR));
		}

		channel.Access(&cToken, sizeof(TCHAR));
	}

	//////////////////////////////////////////////////////////////////////////
	inline void InsertComma(std::vector<sGroupingData>& vecObjectCountStack, core::IChannel& channel)
	{
		if( vecObjectCountStack.empty() )
			return;

		sGroupingData& counter = vecObjectCountStack.back();
		if( counter.tSequence > 0 )
		{
			TCHAR cDelToken;
			cDelToken = TEXT(',');
			channel.Access(&cDelToken, sizeof(TCHAR));
			cDelToken = TEXT('\r');
			channel.Access(&cDelToken, sizeof(TCHAR));
			cDelToken = TEXT('\n');
			channel.Access(&cDelToken, sizeof(TCHAR));
		}

		size_t i;
		for(i=0; i<vecObjectCountStack.size(); i++)
		{
			TCHAR cDelToken = TEXT('\t');
			channel.Access(&cDelToken, sizeof(TCHAR));
		}
		counter.tSequence++;
	}

	//////////////////////////////////////////////////////////////////////////
	CJSONSerializer::CJSONSerializer(core::IChannel& channel)
		: IFormatter(channel)
		, m_vecObjectCountStack()
		, m_bValidity(false)
	{
		m_bValidity = channel.CheckValidity(m_strErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	CJSONSerializer::~CJSONSerializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CJSONSerializer::BeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		InsertComma(m_vecObjectCountStack, m_Channel);

		{	// Key
			std::tstring strToken;
			strToken = fmt_internal::ConvertToJsonString(strKey) + TEXT(":");
			m_Channel.Access((void*)strToken.c_str(), sizeof(TCHAR) * strToken.length());
		}

		AddOpeningToken(m_vecObjectCountStack, m_Channel, TEXT('{'));

		sGroupingData newObjectCountInfo(GT_DICTIONARY, tSize);
		m_vecObjectCountStack.push_back(newObjectCountInfo);
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CJSONSerializer::EndDictionary()
	{
		if( m_vecObjectCountStack.empty() )
			return;
		m_vecObjectCountStack.pop_back();

		AddClosingToken(m_vecObjectCountStack, m_Channel, TEXT('}'));
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CJSONSerializer::BeginArray(std::tstring& strKey, const size_t tSize)
	{
		InsertComma(m_vecObjectCountStack, m_Channel);

		{	// Key
			std::tstring strToken;
			strToken = fmt_internal::ConvertToJsonString(strKey) + TEXT(":");
			m_Channel.Access((void*)strToken.c_str(), sizeof(TCHAR) * strToken.length());
		}

		AddOpeningToken(m_vecObjectCountStack, m_Channel, TEXT('['));

		sGroupingData newObjectCountInfo(GT_ARRAY, tSize);
		m_vecObjectCountStack.push_back(newObjectCountInfo);
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CJSONSerializer::EndArray()
	{
		if( m_vecObjectCountStack.empty() )
			return;
		m_vecObjectCountStack.pop_back();

		AddClosingToken(m_vecObjectCountStack, m_Channel, TEXT(']'));
	}

	//////////////////////////////////////////////////////////////////////////
	void CJSONSerializer::BeginObject(std::tstring& strKey)
	{
		InsertComma(m_vecObjectCountStack, m_Channel);

		if( GT_ARRAY != m_vecObjectCountStack.back().nType )
		{	// Key
			std::tstring strToken;
			strToken = fmt_internal::ConvertToJsonString(strKey) + TEXT(":");
			m_Channel.Access((void*)strToken.c_str(), sizeof(TCHAR) * strToken.length());
		}
		AddOpeningToken(m_vecObjectCountStack, m_Channel, TEXT('{'));

		sGroupingData newObjectCountInfo(GT_OBJECT, 0x7FFFFFFF);
		m_vecObjectCountStack.push_back(newObjectCountInfo);
	}

	//////////////////////////////////////////////////////////////////////////
	void CJSONSerializer::EndObject()
	{
		if( m_vecObjectCountStack.empty() )
			return;
		m_vecObjectCountStack.pop_back();

		AddClosingToken(m_vecObjectCountStack, m_Channel, TEXT('}'));
	}

	//////////////////////////////////////////////////////////////////////////
	void CJSONSerializer::BeginRoot()
	{
		AddOpeningToken(m_vecObjectCountStack, m_Channel, TEXT('{'));

		sGroupingData newObjectCountInfo(GT_OBJECT, 0x7FFFFFFF);
		m_vecObjectCountStack.push_back(newObjectCountInfo);
	}

	//////////////////////////////////////////////////////////////////////////
	void CJSONSerializer::EndRoot()
	{
		if( m_vecObjectCountStack.empty() )
			return;
		m_vecObjectCountStack.pop_back();

		AddClosingToken(m_vecObjectCountStack, m_Channel, TEXT('}'));
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CJSONSerializer::Sync(std::tstring& strKey, std::tstring* pString)
	{
		InsertComma(m_vecObjectCountStack, m_Channel);

		if( GT_ARRAY != m_vecObjectCountStack.back().nType )
		{	// Key
			std::tstring strToken;
			strToken = fmt_internal::ConvertToJsonString(strKey) + TEXT(':');
			m_Channel.Access((void*)strToken.c_str(), sizeof(TCHAR) * strToken.length());
		}

		{	// Value
			std::tstring strToken;
			strToken = fmt_internal::ConvertToJsonString(*pString);
			m_Channel.Access((void*)strToken.c_str(), sizeof(TCHAR) * strToken.length());
		}

		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT & CJSONSerializer::Sync(std::tstring & strKey, std::ntstring * pValue)
	{
		std::tstring strTempString = TCSFromNTCS(*pValue);
		Sync(strKey, &strTempString);
		*pValue = NTCSFromTCS(strTempString);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline void JSONSerializerMetaSync(std::vector<sGroupingData>& m_vecObjectCountStack, core::IChannel& m_Channel, std::tstring& strKey, T& refValue)
	{
		InsertComma(m_vecObjectCountStack, m_Channel);

		// Key
		if( GT_ARRAY != m_vecObjectCountStack.back().nType )
		{
			std::tstring strToken;
			strToken = fmt_internal::ConvertToJsonString(strKey) + TEXT(':');
			m_Channel.Access((void*)strToken.c_str(), sizeof(TCHAR) * strToken.length());
		}

		// Value
		{
			std::tstring strToken;
			strToken = StringFrom(refValue);
			m_Channel.Access((void*)strToken.c_str(), sizeof(TCHAR) * strToken.length());
		}
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CJSONSerializer::Sync(std::tstring& strKey, bool* pValue)
	{
		JSONSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatterT& CJSONSerializer::Sync(std::tstring& strKey, char* pValue)
	{
		JSONSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatterT& CJSONSerializer::Sync(std::tstring& strKey, short* pValue)
	{
		JSONSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatterT& CJSONSerializer::Sync(std::tstring& strKey, int32_t* pValue)
	{
		JSONSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatterT& CJSONSerializer::Sync(std::tstring& strKey, int64_t* pValue)
	{
		JSONSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatterT& CJSONSerializer::Sync(std::tstring& strKey, BYTE* pValue)
	{
		JSONSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatterT& CJSONSerializer::Sync(std::tstring& strKey, WORD* pValue)
	{
		JSONSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatterT& CJSONSerializer::Sync(std::tstring& strKey, DWORD* pValue)
	{
		JSONSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatterT& CJSONSerializer::Sync(std::tstring& strKey, QWORD* pValue)
	{
		JSONSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatterT& CJSONSerializer::Sync(std::tstring& strKey, float* pValue)
	{
		JSONSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatterT& CJSONSerializer::Sync(std::tstring& strKey, double* pValue)
	{
		JSONSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatterT& CJSONSerializer::Sync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		// ignored, use UBJSON instead
		return *this;
	}
}
