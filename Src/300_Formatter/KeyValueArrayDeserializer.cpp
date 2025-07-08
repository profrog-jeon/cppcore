#include "stdafx.h"
#include "KeyValueArrayDeserializer.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CKeyValueArrayDeserializer::CKeyValueArrayDeserializer(core::IChannel& channel, std::tstring strTokenDelimiter, std::tstring strSpliter)
		: CFormatterSuper(channel)
		, m_strTokenDelimiter(strTokenDelimiter)
		, m_strSplitter(strSpliter)
		, m_mapValue()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CKeyValueArrayDeserializer::~CKeyValueArrayDeserializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool CKeyValueArrayDeserializer::OnPrepare(IFormatterObject* pObject, std::tstring& strErrMsg)
	{
		if( !m_Channel.CheckValidity(strErrMsg))
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

		std::vector<std::tstring> vecToken;
		TokenToVectorByExactDelimiter(strContext, m_strTokenDelimiter, vecToken);

		size_t i;
		for(i=0; i<vecToken.size(); i++)
		{
			std::tstring strValue;
			std::tstring strKey = Split(vecToken[i], m_strSplitter, &strValue);
			m_mapValue.insert(std::make_pair(strKey, strValue));
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CKeyValueArrayDeserializer::OnBeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyValueArrayDeserializer::OnEndDictionary()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CKeyValueArrayDeserializer::OnBeginArray(std::tstring& strKey, const size_t tSize)
	{
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyValueArrayDeserializer::OnEndArray()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyValueArrayDeserializer::OnBeginObject(std::tstring& strKey)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyValueArrayDeserializer::OnEndObject()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyValueArrayDeserializer::OnBeginRoot(std::tstring& strRootName)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CKeyValueArrayDeserializer::OnEndRoot()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	static inline void __KeyValueArrayDeserializerMetaFunction(const std::map<std::tstring, std::tstring>& mapValue, std::tstring strKey, T* pValue)
	{
		std::map<std::tstring, std::tstring>::const_iterator iter = mapValue.find(strKey);
		if( iter == mapValue.end() )
			return;

		*pValue = ValueFrom<T>(iter->second);
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyValueArrayDeserializer::OnSync(std::tstring& strKey, std::tstring* pValue)
	{
		__KeyValueArrayDeserializerMetaFunction(m_mapValue, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyValueArrayDeserializer::OnSync(std::tstring & strKey, std::ntstring * pValue)
	{
		__KeyValueArrayDeserializerMetaFunction(m_mapValue, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyValueArrayDeserializer::OnSync(std::tstring& strKey, bool* pValue)
	{
		__KeyValueArrayDeserializerMetaFunction(m_mapValue, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyValueArrayDeserializer::OnSync(std::tstring& strKey, char* pValue)
	{
		__KeyValueArrayDeserializerMetaFunction(m_mapValue, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyValueArrayDeserializer::OnSync(std::tstring& strKey, short* pValue)
	{
		__KeyValueArrayDeserializerMetaFunction(m_mapValue, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyValueArrayDeserializer::OnSync(std::tstring& strKey, int32_t* pValue)
	{
		__KeyValueArrayDeserializerMetaFunction(m_mapValue, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyValueArrayDeserializer::OnSync(std::tstring& strKey, int64_t* pValue)
	{
		__KeyValueArrayDeserializerMetaFunction(m_mapValue, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyValueArrayDeserializer::OnSync(std::tstring& strKey, BYTE* pValue)
	{
		__KeyValueArrayDeserializerMetaFunction(m_mapValue, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyValueArrayDeserializer::OnSync(std::tstring& strKey, WORD* pValue)
	{
		__KeyValueArrayDeserializerMetaFunction(m_mapValue, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyValueArrayDeserializer::OnSync(std::tstring& strKey, DWORD* pValue)
	{
		__KeyValueArrayDeserializerMetaFunction(m_mapValue, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyValueArrayDeserializer::OnSync(std::tstring& strKey, QWORD* pValue)
	{
		__KeyValueArrayDeserializerMetaFunction(m_mapValue, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyValueArrayDeserializer::OnSync(std::tstring& strKey, float* pValue)
	{
		__KeyValueArrayDeserializerMetaFunction(m_mapValue, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyValueArrayDeserializer::OnSync(std::tstring& strKey, double* pValue)
	{
		__KeyValueArrayDeserializerMetaFunction(m_mapValue, strKey, pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CKeyValueArrayDeserializer::OnSync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		// Ignore
		return *this;
	}
}
