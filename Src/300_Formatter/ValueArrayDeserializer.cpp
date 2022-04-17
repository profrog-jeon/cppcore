#include "stdafx.h"
#include "ValueArrayDeserializer.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CValueArrayDeserializer::CValueArrayDeserializer(core::IChannel& channel, std::tstring strTokenDelimiter, std::tstring strQuotation, bool bSkipTitle)
		: IFormatter(channel)
		, m_vecValueTable()
		, m_strQuotation(strQuotation)
		, m_tArrayIndex(0)
		, m_tItemIndex(0)
		, m_strErrMsg()
		, m_bValidity(false)
	{
		m_bValidity = channel.CheckValidity(m_strErrMsg);
		if( !m_bValidity )
			return;

		std::tstring strContext;
		std::tstring strBuff;
		const size_t tBuffLen = 1024;
		size_t tReadSize = 0;
		while(1)
		{
			strBuff.resize(tBuffLen);
			tReadSize = m_Channel.OnAccess((void*)strBuff.c_str(), tBuffLen * sizeof(TCHAR));
			if( 0 == tReadSize )
				break;

			tReadSize /= sizeof(TCHAR);
			strBuff.resize(tReadSize);
			strContext += strBuff;
		}

		int nOffset = 0;
		if( bSkipTitle )
			Tokenize(strContext, TEXT("\n"), nOffset);

		while(nOffset >= 0)
		{
			std::tstring strTempToken = Tokenize(strContext, TEXT("\n"), nOffset);
			std::tstring strLine = Trim(strTempToken);
			if( strLine.empty() )
				break;

			std::vector<std::tstring> vecToken;
			TokenToVectorByExactDelimiter(strLine, strTokenDelimiter, vecToken);

			m_vecValueTable.push_back(vecToken);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	CValueArrayDeserializer::~CValueArrayDeserializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CValueArrayDeserializer::BeginDictionaryGrouping(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		return m_vecValueTable.size();
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArrayDeserializer::EndDictionaryGrouping()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CValueArrayDeserializer::BeginArrayGrouping(std::tstring& strKey, const size_t tSize)
	{
		return m_vecValueTable.size();
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArrayDeserializer::EndArrayGrouping()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArrayDeserializer::BeginArrayItem(size_t tIndex, size_t tCount)
	{
		m_tArrayIndex = tIndex;
		m_tItemIndex = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArrayDeserializer::EndArrayItem(size_t tIndex, size_t tCount)
	{
		m_tArrayIndex = tIndex;
		m_tItemIndex = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArrayDeserializer::BeginObjectGrouping(std::tstring& strKey)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArrayDeserializer::EndObjectGrouping()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArrayDeserializer::BeginRootGrouping()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArrayDeserializer::EndRootGrouping()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	inline void __ValueArrayDeserializerMetaFunction(const std::vector<std::tstring>& vecValue, std::tstring strQuotation, size_t& refIndex, T* pValue)
	{
		if( refIndex >= vecValue.size() )
			return;

		std::tstring strValue = Trim(vecValue[refIndex++].c_str(), strQuotation.c_str());
		*pValue = ValueFrom<T>(strValue);
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArrayDeserializer::Sync(std::tstring& strKey, std::tstring* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT & CValueArrayDeserializer::Sync(std::tstring & strKey, std::ntstring * pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArrayDeserializer::Sync(std::tstring& strKey, bool* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArrayDeserializer::Sync(std::tstring& strKey, char* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArrayDeserializer::Sync(std::tstring& strKey, short* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArrayDeserializer::Sync(std::tstring& strKey, int32_t* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArrayDeserializer::Sync(std::tstring& strKey, int64_t* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArrayDeserializer::Sync(std::tstring& strKey, BYTE* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArrayDeserializer::Sync(std::tstring& strKey, WORD* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArrayDeserializer::Sync(std::tstring& strKey, DWORD* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArrayDeserializer::Sync(std::tstring& strKey, QWORD* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArrayDeserializer::Sync(std::tstring& strKey, float* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CValueArrayDeserializer::Sync(std::tstring& strKey, double* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}
}
