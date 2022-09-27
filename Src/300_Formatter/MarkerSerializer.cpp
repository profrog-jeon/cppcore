#include "stdafx.h"
#include "MarkerSerializer.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////	
	inline void __MarkerSerializer_ExtractTagMarker(const std::tstring& strContext, CStringSet& setMarker)
	{
		size_t tCurPos = 0;
		while(tCurPos != std::tstring::npos)
		{
			size_t tStart = strContext.find(TEXT("["), tCurPos);
			if( std::tstring::npos == tStart )
				break;

			size_t tEnd = strContext.find(TEXT("]"), tStart);
			if( std::tstring::npos == tEnd )
				break;

			std::tstring strMarker = strContext.substr(tStart + 1, tEnd - tStart - 1);
			setMarker.insert(strMarker);
			tCurPos = tEnd;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	CMarkerSerializer::CMarkerSerializer(core::IChannel& channel, std::tstring strSchema)
		: IFormatter(channel)
		, m_strSchema(strSchema)
		, m_bValidity(false)
		, m_setMarker()
	{
		m_bValidity = channel.CheckValidity(m_strErrMsg);
		__MarkerSerializer_ExtractTagMarker(m_strSchema, m_setMarker);
	}

	//////////////////////////////////////////////////////////////////////////
	CMarkerSerializer::~CMarkerSerializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CMarkerSerializer::BeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		std::tstring strTraverseContext = m_stackTraverse.top() + TEXT(".") + strKey;
		m_stackTraverse.push(strTraverseContext);
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CMarkerSerializer::EndDictionary()
	{
		m_stackTraverse.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CMarkerSerializer::BeginArray(std::tstring& strKey, const size_t tSize)
	{
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CMarkerSerializer::EndArray()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CMarkerSerializer::BeginObject(std::tstring& strKey)
	{
		std::tstring strTraverseContext = m_stackTraverse.top() + TEXT(".") + strKey;
		m_stackTraverse.push(strTraverseContext);
	}

	//////////////////////////////////////////////////////////////////////////
	void CMarkerSerializer::EndObject()
	{
		m_stackTraverse.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	void CMarkerSerializer::BeginRoot()
	{
		m_stackTraverse.push(TEXT("ROOT"));
	}

	//////////////////////////////////////////////////////////////////////////
	void CMarkerSerializer::EndRoot()
	{
		m_stackTraverse.pop();

		size_t tTotalSent = 0;
		size_t tLength = m_strSchema.length() * sizeof(TCHAR);
		LPBYTE pData = (LPBYTE)m_strSchema.c_str();
		while(tTotalSent < tLength)
		{
			size_t tSent = m_Channel.Access(pData + tTotalSent, tLength - tTotalSent);
			tTotalSent += tSent;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline void HTMLSerializerMetaSync(core::IChannel& m_Channel, std::tstring& strSchema, std::tstring strTraverse, CStringSet& m_setMarker, std::tstring& strKey, T& refValue)
	{
		strTraverse += TEXT(".") + strKey;
		
		CStringSetIt iter;
		for(iter=m_setMarker.begin(); iter!=m_setMarker.end(); iter++)
		{
			std::tstring strSrc = TEXT(".") + *iter;

			size_t tLength = strTraverse.length();
			if( tLength < strSrc.length() )
				continue;
			
			std::tstring strDest = strTraverse.substr(tLength - strSrc.length());
			if( strDest != strSrc )
				continue;

			std::tstring strMarker = TEXT("[") + (*iter) + TEXT("]");
			std::tstring strValue = StringFrom(refValue);
			strSchema = Replace(strSchema, strMarker, strValue);
			m_setMarker.erase(iter);
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CMarkerSerializer::Sync(std::tstring& strKey, std::tstring* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT & CMarkerSerializer::Sync(std::tstring & strKey, std::ntstring * pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CMarkerSerializer::Sync(std::tstring& strKey, bool* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CMarkerSerializer::Sync(std::tstring& strKey, char* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CMarkerSerializer::Sync(std::tstring& strKey, short* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CMarkerSerializer::Sync(std::tstring& strKey, int32_t* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CMarkerSerializer::Sync(std::tstring& strKey, int64_t* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CMarkerSerializer::Sync(std::tstring& strKey, BYTE* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CMarkerSerializer::Sync(std::tstring& strKey, WORD* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CMarkerSerializer::Sync(std::tstring& strKey, DWORD* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CMarkerSerializer::Sync(std::tstring& strKey, QWORD* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CMarkerSerializer::Sync(std::tstring& strKey, float* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CMarkerSerializer::Sync(std::tstring& strKey, double* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CMarkerSerializer::Sync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		// Ignore
		return *this;
	}
}
