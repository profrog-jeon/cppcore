#include "stdafx.h"
#include "UBJSONSerializer.h"
#include "UBJSONFunctions.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CUBJSONSerializer::CUBJSONSerializer(core::IChannel& channel)
		: CFormatterSuper(channel)
		, m_vecObjectCountStack()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CUBJSONSerializer::~CUBJSONSerializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool CUBJSONSerializer::OnPrepare(IFormatterObject* pObject, std::tstring& strErrMsg)
	{
		return m_Channel.CheckValidity(strErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CUBJSONSerializer::OnBeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		WriteUBJsonKey(m_Channel, strKey);
		WriteUBJsonConst(m_Channel, "{");

		sGroupingData newObjectCountInfo(GT_DICTIONARY, tSize);
		m_vecObjectCountStack.push_back(newObjectCountInfo);
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CUBJSONSerializer::OnEndDictionary()
	{
		if( m_vecObjectCountStack.empty() )
			return;
		m_vecObjectCountStack.pop_back();

		WriteUBJsonConst(m_Channel, "}");
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CUBJSONSerializer::OnBeginArray(std::tstring& strKey, const size_t tSize)
	{
		WriteUBJsonKey(m_Channel, strKey);
		WriteUBJsonConst(m_Channel, "[");

		sGroupingData newObjectCountInfo(GT_ARRAY, tSize);
		m_vecObjectCountStack.push_back(newObjectCountInfo);
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CUBJSONSerializer::OnEndArray()
	{
		if( m_vecObjectCountStack.empty() )
			return;
		m_vecObjectCountStack.pop_back();

		WriteUBJsonConst(m_Channel, "]");
	}

	//////////////////////////////////////////////////////////////////////////
	void CUBJSONSerializer::OnBeginObject(std::tstring& strKey)
	{
		if( GT_ARRAY != m_vecObjectCountStack.back().nType )
		{
			WriteUBJsonKey(m_Channel, strKey);
		}
		WriteUBJsonConst(m_Channel, "{");

		sGroupingData newObjectCountInfo(GT_OBJECT, 0x7FFFFFFF);
		m_vecObjectCountStack.push_back(newObjectCountInfo);
	}

	//////////////////////////////////////////////////////////////////////////
	void CUBJSONSerializer::OnEndObject()
	{
		if( m_vecObjectCountStack.empty() )
			return;
		m_vecObjectCountStack.pop_back();

		WriteUBJsonConst(m_Channel, "}");
	}

	//////////////////////////////////////////////////////////////////////////
	void CUBJSONSerializer::OnBeginRoot(std::tstring& strRootName)
	{
		WriteUBJsonConst(m_Channel, "{");

		sGroupingData newObjectCountInfo(GT_OBJECT, 0x7FFFFFFF);
		m_vecObjectCountStack.push_back(newObjectCountInfo);
	}

	//////////////////////////////////////////////////////////////////////////
	void CUBJSONSerializer::OnEndRoot()
	{
		if( m_vecObjectCountStack.empty() )
			return;
		m_vecObjectCountStack.pop_back();

		WriteUBJsonConst(m_Channel, "}");
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CUBJSONSerializer::OnSync(std::tstring& strKey, std::tstring* pString)
	{
		if( GT_ARRAY != m_vecObjectCountStack.back().nType )
		{
			WriteUBJsonKey(m_Channel, strKey);
		}

		WriteUBJsonValue(m_Channel, *pString);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CUBJSONSerializer::OnSync(std::tstring & strKey, std::ntstring * pValue)
	{
		std::tstring strTempString = TCSFromNTCS(*pValue);
		OnSync(strKey, &strTempString);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline void UBJSONSerializerMetaSync(std::vector<sGroupingData>& m_vecObjectCountStack, core::IChannel& m_Channel, std::tstring& strKey, T& refValue)
	{
		if( GT_ARRAY != m_vecObjectCountStack.back().nType )
		{
			WriteUBJsonKey(m_Channel, strKey);
		}

		WriteUBJsonValue(m_Channel, refValue);
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CUBJSONSerializer::OnSync(std::tstring& strKey, bool* pValue)
	{
		UBJSONSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CUBJSONSerializer::OnSync(std::tstring& strKey, char* pValue)
	{
		UBJSONSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CUBJSONSerializer::OnSync(std::tstring& strKey, short* pValue)
	{
		UBJSONSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CUBJSONSerializer::OnSync(std::tstring& strKey, int32_t* pValue)
	{
		UBJSONSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CUBJSONSerializer::OnSync(std::tstring& strKey, int64_t* pValue)
	{
		UBJSONSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CUBJSONSerializer::OnSync(std::tstring& strKey, BYTE* pValue)
	{
		UBJSONSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CUBJSONSerializer::OnSync(std::tstring& strKey, WORD* pValue)
	{
		UBJSONSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CUBJSONSerializer::OnSync(std::tstring& strKey, DWORD* pValue)
	{
		UBJSONSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CUBJSONSerializer::OnSync(std::tstring& strKey, QWORD* pValue)
	{
		UBJSONSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CUBJSONSerializer::OnSync(std::tstring& strKey, float* pValue)
	{
		UBJSONSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CUBJSONSerializer::OnSync(std::tstring& strKey, double* pValue)
	{
		UBJSONSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CUBJSONSerializer::OnSync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		if (GT_ARRAY != m_vecObjectCountStack.back().nType)
		{
			WriteUBJsonKey(m_Channel, strKey);
		}

		WriteUBJsonValue(m_Channel, *pvecData);
		return *this;
	}
}
