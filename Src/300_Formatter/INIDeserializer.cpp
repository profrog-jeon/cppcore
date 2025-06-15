#include "stdafx.h"
#include "INIDeserializer.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CINIDeserializer::CINIDeserializer(core::IChannel& channel, std::tstring strSection)
		: CFormatterSuper(channel)
		, m_stkGroupData()
		, m_INI()
		, m_strRootSection(strSection)
		, m_mapArrayValues()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CINIDeserializer::~CINIDeserializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool CINIDeserializer::OnPrepare(IFormatterObject* pObject, std::tstring& strErrMsg)
	{
		try
		{
			if( !m_Channel.CheckValidity(strErrMsg) )
				throw exception_format(TEXT("%s"), strErrMsg.c_str());

			std::tstring strContext;
			while(1)
			{
				const size_t tTokenSize = 512;
				TCHAR szBuff[tTokenSize+1] = { 0, };
				size_t tReadSize = m_Channel.Access(szBuff, sizeof(TCHAR) * tTokenSize);
				if( 0 == tReadSize )
					break;

				strContext += szBuff;
			}

			ECODE errRet = m_INI.Open(strContext);
			if( EC_SUCCESS != errRet )
			{
				strErrMsg = Format(TEXT("INI Parsing failure, CINIDeserializer(), %d"), errRet);
				throw exception_format(TEXT("%s"), strErrMsg.c_str());
			}
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			return false;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CINIDeserializer::OnBeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		m_mapArrayValues.clear();

		if( strKey.empty() )
		{
			m_vecKeys.clear();
			m_INI.GetSectionNames(m_vecKeys);
			m_stkGroupData.push(sGroupingData(strKey, GT_DICTIONARY, m_vecKeys.size()));
			return m_vecKeys.size();
		}
		else
		{
			m_INI.GetSectionValues(strKey.c_str(), m_mapArrayValues);
			m_stkGroupData.push(sGroupingData(strKey, GT_DICTIONARY, m_mapArrayValues.size()));
			return m_mapArrayValues.size();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void CINIDeserializer::OnEndDictionary()
	{
		m_stkGroupData.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CINIDeserializer::OnBeginArray(std::tstring& strKey, const size_t tSize)
	{
		m_mapArrayValues.clear();
		m_INI.GetSectionValues(strKey.c_str(), m_mapArrayValues);

		m_stkGroupData.push(sGroupingData(strKey, GT_ARRAY, m_mapArrayValues.size()));
		return m_mapArrayValues.size();
	}

	//////////////////////////////////////////////////////////////////////////
	void CINIDeserializer::OnEndArray()
	{
		m_stkGroupData.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	void CINIDeserializer::OnBeginObject(std::tstring& strKey)
	{
		sGroupingData& refTopGroupingData = m_stkGroupData.top();
		if( GT_DICTIONARY == refTopGroupingData.nType )
		{
			strKey = m_vecKeys[refTopGroupingData.tSequence++];
			m_stkGroupData.push(sGroupingData(strKey, GT_OBJECT, m_mapArrayValues.size()));
		}
		else
			m_stkGroupData.push(sGroupingData(strKey, GT_OBJECT, m_mapArrayValues.size()));
	}

	//////////////////////////////////////////////////////////////////////////
	void CINIDeserializer::OnEndObject()
	{
		m_stkGroupData.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	void CINIDeserializer::OnBeginRoot(std::tstring& strRootName)
	{
		m_stkGroupData.push(sGroupingData(m_strRootSection.empty()? TEXT("DEFAULT") : m_strRootSection, GT_ROOT, 0));
	}

	//////////////////////////////////////////////////////////////////////////
	void CINIDeserializer::OnEndRoot()
	{
		m_stkGroupData.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline void __INIDeserializerMetaFunction(CINIFormatter& INI, sGroupingData& GroupingData, std::tstring& strKey, T* pValue, CStringMap& mapArrayValues)
	{
		if( !mapArrayValues.empty() )
		{
			strKey = mapArrayValues.begin()->first;
			std::tstring strValue = mapArrayValues.begin()->second;
			*pValue = ValueFrom<T>(strValue);
			mapArrayValues.erase(mapArrayValues.begin());
			return;
		}

		std::tstring strValue;
		if( EC_SUCCESS != INI.GetValue(GroupingData.strName.c_str(), strKey.c_str(), strValue) )
			return;

		*pValue = ValueFrom<T>(strValue);
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINIDeserializer::OnSync(std::tstring& strKey, std::tstring* pValue)
	{
		__INIDeserializerMetaFunction(m_INI, m_stkGroupData.top(), strKey, pValue, m_mapArrayValues);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINIDeserializer::OnSync(std::tstring & strKey, std::ntstring * pValue)
	{
		__INIDeserializerMetaFunction(m_INI, m_stkGroupData.top(), strKey, pValue, m_mapArrayValues);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINIDeserializer::OnSync(std::tstring& strKey, bool* pValue)
	{
		__INIDeserializerMetaFunction(m_INI, m_stkGroupData.top(), strKey, pValue, m_mapArrayValues);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINIDeserializer::OnSync(std::tstring& strKey, char* pValue)
	{
		__INIDeserializerMetaFunction(m_INI, m_stkGroupData.top(), strKey, pValue, m_mapArrayValues);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINIDeserializer::OnSync(std::tstring& strKey, short* pValue)
	{
		__INIDeserializerMetaFunction(m_INI, m_stkGroupData.top(), strKey, pValue, m_mapArrayValues);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINIDeserializer::OnSync(std::tstring& strKey, int32_t* pValue)
	{
		__INIDeserializerMetaFunction(m_INI, m_stkGroupData.top(), strKey, pValue, m_mapArrayValues);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINIDeserializer::OnSync(std::tstring& strKey, int64_t* pValue)
	{
		__INIDeserializerMetaFunction(m_INI, m_stkGroupData.top(), strKey, pValue, m_mapArrayValues);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINIDeserializer::OnSync(std::tstring& strKey, BYTE* pValue)
	{
		__INIDeserializerMetaFunction(m_INI, m_stkGroupData.top(), strKey, pValue, m_mapArrayValues);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINIDeserializer::OnSync(std::tstring& strKey, WORD* pValue)
	{
		__INIDeserializerMetaFunction(m_INI, m_stkGroupData.top(), strKey, pValue, m_mapArrayValues);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINIDeserializer::OnSync(std::tstring& strKey, DWORD* pValue)
	{
		__INIDeserializerMetaFunction(m_INI, m_stkGroupData.top(), strKey, pValue, m_mapArrayValues);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINIDeserializer::OnSync(std::tstring& strKey, QWORD* pValue)
	{
		__INIDeserializerMetaFunction(m_INI, m_stkGroupData.top(), strKey, pValue, m_mapArrayValues);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINIDeserializer::OnSync(std::tstring& strKey, float* pValue)
	{
		__INIDeserializerMetaFunction(m_INI, m_stkGroupData.top(), strKey, pValue, m_mapArrayValues);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINIDeserializer::OnSync(std::tstring& strKey, double* pValue)
	{
		__INIDeserializerMetaFunction(m_INI, m_stkGroupData.top(), strKey, pValue, m_mapArrayValues);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CINIDeserializer::OnSync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		// Ignore
		return *this;
	}
}
