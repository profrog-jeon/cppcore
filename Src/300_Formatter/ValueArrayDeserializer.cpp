#include "stdafx.h"
#include "ValueArrayDeserializer.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CValueArrayDeserializer::CValueArrayDeserializer(core::IChannel& channel, std::tstring strTokenDelimiter, std::tstring strQuotation, bool bSkipTitle)
		: CFormatterSuper(channel)
		, m_vecValueTable()
		, m_strTokenDelimiter(strTokenDelimiter)
		, m_strQuotation(strQuotation)
		, m_tArrayIndex(0)
		, m_tItemIndex(0)
		, m_bSkipTitle(bSkipTitle)
	{
	}
	//////////////////////////////////////////////////////////////////////////
	CValueArrayDeserializer::~CValueArrayDeserializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool CValueArrayDeserializer::OnPrepare(IFormatterObject* pObject, std::tstring& strErrMsg)
	{
		if (!m_Channel.CheckValidity(strErrMsg))
			return false;

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
		if (m_bSkipTitle)
			Tokenize(strContext, TEXT("\n"), nOffset);

		while(nOffset >= 0)
		{
			std::tstring strTempToken = Tokenize(strContext, TEXT("\n"), nOffset);
			std::tstring strLine = Trim(strTempToken);
			if( strLine.empty() )
				break;

			std::vector<std::tstring> vecToken;
			TokenToVectorByExactDelimiter(strLine, m_strTokenDelimiter, vecToken);

			m_vecValueTable.push_back(vecToken);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CValueArrayDeserializer::OnBeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		return m_vecValueTable.size();
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArrayDeserializer::OnEndDictionary()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CValueArrayDeserializer::OnBeginArray(std::tstring& strKey, const size_t tSize)
	{
		return m_vecValueTable.size();
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArrayDeserializer::OnEndArray()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArrayDeserializer::OnBeginArrayItem(size_t tIndex, size_t tCount)
	{
		m_tArrayIndex = tIndex;
		m_tItemIndex = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArrayDeserializer::OnEndArrayItem(size_t tIndex, size_t tCount)
	{
		m_tArrayIndex = tIndex;
		m_tItemIndex = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArrayDeserializer::OnBeginObject(std::tstring& strKey)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArrayDeserializer::OnEndObject()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArrayDeserializer::OnBeginRoot(std::tstring& strRootName)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CValueArrayDeserializer::OnEndRoot()
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
	core::IFormatter& CValueArrayDeserializer::OnSync(std::tstring& strKey, std::tstring* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArrayDeserializer::OnSync(std::tstring & strKey, std::ntstring * pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArrayDeserializer::OnSync(std::tstring& strKey, bool* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArrayDeserializer::OnSync(std::tstring& strKey, char* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArrayDeserializer::OnSync(std::tstring& strKey, short* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArrayDeserializer::OnSync(std::tstring& strKey, int32_t* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArrayDeserializer::OnSync(std::tstring& strKey, int64_t* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArrayDeserializer::OnSync(std::tstring& strKey, BYTE* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArrayDeserializer::OnSync(std::tstring& strKey, WORD* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArrayDeserializer::OnSync(std::tstring& strKey, DWORD* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArrayDeserializer::OnSync(std::tstring& strKey, QWORD* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArrayDeserializer::OnSync(std::tstring& strKey, float* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArrayDeserializer::OnSync(std::tstring& strKey, double* pValue)
	{
		__ValueArrayDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_strQuotation, m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CValueArrayDeserializer::OnSync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		// Ignore
		return *this;
	}
}
