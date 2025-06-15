#include "stdafx.h"
#include "MarkerSerializer.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CMarkerSerializer::CMarkerSerializer(core::IChannel& channel, std::tstring strSchema)
		: CFormatterSuper(channel)
		, m_strSchema(strSchema)
		, m_setMarker()

	{
	}

	//////////////////////////////////////////////////////////////////////////
	CMarkerSerializer::~CMarkerSerializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool CMarkerSerializer::OnPrepare(IFormatterObject* pObject, std::tstring& strErrMsg)
	{
		if (!m_Channel.CheckValidity(strErrMsg))
			return false;

		size_t tCurPos = 0;
		while (tCurPos != std::tstring::npos)
		{
			size_t tStart = m_strSchema.find(TEXT("["), tCurPos);
			if (std::tstring::npos == tStart)
				break;

			size_t tEnd = m_strSchema.find(TEXT("]"), tStart);
			if (std::tstring::npos == tEnd)
				break;

			std::tstring strMarker = m_strSchema.substr(tStart + 1, tEnd - tStart - 1);
			m_setMarker.insert(strMarker);
			tCurPos = tEnd;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CMarkerSerializer::OnBeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		std::tstring strTraverseContext = m_stackTraverse.top() + TEXT(".") + strKey;
		m_stackTraverse.push(strTraverseContext);
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CMarkerSerializer::OnEndDictionary()
	{
		m_stackTraverse.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CMarkerSerializer::OnBeginArray(std::tstring& strKey, const size_t tSize)
	{
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CMarkerSerializer::OnEndArray()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CMarkerSerializer::OnBeginObject(std::tstring& strKey)
	{
		std::tstring strTraverseContext = m_stackTraverse.top() + TEXT(".") + strKey;
		m_stackTraverse.push(strTraverseContext);
	}

	//////////////////////////////////////////////////////////////////////////
	void CMarkerSerializer::OnEndObject()
	{
		m_stackTraverse.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	void CMarkerSerializer::OnBeginRoot(std::tstring& strRootName)
	{
		m_stackTraverse.push(TEXT("ROOT"));
	}

	//////////////////////////////////////////////////////////////////////////
	void CMarkerSerializer::OnEndRoot()
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
	core::IFormatter& CMarkerSerializer::OnSync(std::tstring& strKey, std::tstring* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CMarkerSerializer::OnSync(std::tstring & strKey, std::ntstring * pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CMarkerSerializer::OnSync(std::tstring& strKey, bool* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CMarkerSerializer::OnSync(std::tstring& strKey, char* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CMarkerSerializer::OnSync(std::tstring& strKey, short* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CMarkerSerializer::OnSync(std::tstring& strKey, int32_t* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CMarkerSerializer::OnSync(std::tstring& strKey, int64_t* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CMarkerSerializer::OnSync(std::tstring& strKey, BYTE* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CMarkerSerializer::OnSync(std::tstring& strKey, WORD* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CMarkerSerializer::OnSync(std::tstring& strKey, DWORD* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CMarkerSerializer::OnSync(std::tstring& strKey, QWORD* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CMarkerSerializer::OnSync(std::tstring& strKey, float* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CMarkerSerializer::OnSync(std::tstring& strKey, double* pValue)
	{
		HTMLSerializerMetaSync(m_Channel, m_strSchema, m_stackTraverse.top(), m_setMarker, strKey, *pValue);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CMarkerSerializer::OnSync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		// Ignore
		return *this;
	}
}
