#include "stdafx.h"
#include "CSVDeserializer.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CCSVDeserializer::CCSVDeserializer(core::IChannel& channel, bool bSkipTitle)
		: IFormatter(channel)
		, m_vecValueTable()
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
			tReadSize = m_Channel.Access((void*)strBuff.c_str(), tBuffLen * sizeof(TCHAR));
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
			TokenToVector(strLine, TEXT(','), TEXT('\"'), vecToken);

			m_vecValueTable.push_back(vecToken);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	CCSVDeserializer::~CCSVDeserializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CCSVDeserializer::BeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		return m_vecValueTable.size();
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVDeserializer::EndDictionary()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CCSVDeserializer::BeginArray(std::tstring& strKey, const size_t tSize)
	{
		return m_vecValueTable.size();
	}


	//////////////////////////////////////////////////////////////////////////
	void CCSVDeserializer::EndArray()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVDeserializer::BeginArrayItem(size_t tIndex, size_t tCount)
	{
		m_tArrayIndex = tIndex;
		m_tItemIndex = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVDeserializer::EndArrayItem(size_t tIndex, size_t tCount)
	{
		m_tArrayIndex = tIndex;
		m_tItemIndex = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVDeserializer::BeginObject(std::tstring& strKey)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVDeserializer::EndObject()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVDeserializer::BeginRoot()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVDeserializer::EndRoot()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	static inline void CSVDeserializerMetaFunction(const std::vector<std::tstring>& vecValue, size_t& refIndex, T* pValue)
	{
		if( refIndex >= vecValue.size() )
			return;

		*pValue = ValueFrom<T>(StripSpecialChar(vecValue[refIndex++], TEXT(','), TEXT('\"')));
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVDeserializer::Sync(std::tstring& strKey, std::tstring* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT & CCSVDeserializer::Sync(std::tstring & strKey, std::ntstring * pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVDeserializer::Sync(std::tstring& strKey, bool* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVDeserializer::Sync(std::tstring& strKey, char* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVDeserializer::Sync(std::tstring& strKey, short* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVDeserializer::Sync(std::tstring& strKey, int32_t* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVDeserializer::Sync(std::tstring& strKey, int64_t* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVDeserializer::Sync(std::tstring& strKey, BYTE* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVDeserializer::Sync(std::tstring& strKey, WORD* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVDeserializer::Sync(std::tstring& strKey, DWORD* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVDeserializer::Sync(std::tstring& strKey, QWORD* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVDeserializer::Sync(std::tstring& strKey, float* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVDeserializer::Sync(std::tstring& strKey, double* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CCSVDeserializer::Sync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		// Ignore
		return *this;
	}
}

