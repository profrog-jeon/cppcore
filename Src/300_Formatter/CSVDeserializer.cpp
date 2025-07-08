#include "stdafx.h"
#include "CSVDeserializer.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CCSVDeserializer::CCSVDeserializer(core::IChannel& channel, bool bSkipTitle)
		: CFormatterSuper(channel)
		, m_vecValueTable()
		, m_tArrayIndex(0)
		, m_tItemIndex(0)
		, m_bSkipTitle(bSkipTitle)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CCSVDeserializer::~CCSVDeserializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool CCSVDeserializer::OnPrepare(IFormatterObject* pObject, std::tstring& strErrMsg)
	{
		bool bValidity = m_Channel.CheckValidity(strErrMsg);
		if( !bValidity )
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
		if(m_bSkipTitle)
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
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CCSVDeserializer::OnBeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		return m_vecValueTable.size();
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVDeserializer::OnEndDictionary()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CCSVDeserializer::OnBeginArray(std::tstring& strKey, const size_t tSize)
	{
		return m_vecValueTable.size();
	}


	//////////////////////////////////////////////////////////////////////////
	void CCSVDeserializer::OnEndArray()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVDeserializer::OnBeginArrayItem(size_t tIndex, size_t tCount)
	{
		m_tArrayIndex = tIndex;
		m_tItemIndex = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVDeserializer::OnEndArrayItem(size_t tIndex, size_t tCount)
	{
		m_tArrayIndex = tIndex;
		m_tItemIndex = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVDeserializer::OnBeginObject(std::tstring& strKey)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVDeserializer::OnEndObject()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVDeserializer::OnBeginRoot(std::tstring& strRootName)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CCSVDeserializer::OnEndRoot()
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
	core::IFormatter& CCSVDeserializer::OnSync(std::tstring& strKey, std::tstring* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVDeserializer::OnSync(std::tstring & strKey, std::ntstring * pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVDeserializer::OnSync(std::tstring& strKey, bool* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVDeserializer::OnSync(std::tstring& strKey, char* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVDeserializer::OnSync(std::tstring& strKey, short* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVDeserializer::OnSync(std::tstring& strKey, int32_t* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVDeserializer::OnSync(std::tstring& strKey, int64_t* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVDeserializer::OnSync(std::tstring& strKey, BYTE* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVDeserializer::OnSync(std::tstring& strKey, WORD* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVDeserializer::OnSync(std::tstring& strKey, DWORD* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVDeserializer::OnSync(std::tstring& strKey, QWORD* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVDeserializer::OnSync(std::tstring& strKey, float* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVDeserializer::OnSync(std::tstring& strKey, double* pValue)
	{
		CSVDeserializerMetaFunction(m_vecValueTable[m_tArrayIndex], m_tItemIndex, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CCSVDeserializer::OnSync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		// Ignore
		return *this;
	}
}

