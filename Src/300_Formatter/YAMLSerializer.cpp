#include "stdafx.h"
#include "YAMLSerializer.h"
#include "YAMLParser.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CYAMLSerializer::CYAMLSerializer(core::IChannel& channel)
		: CFormatterSuper(channel)
		, m_vecObjectCountStack()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CYAMLSerializer::~CYAMLSerializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool CYAMLSerializer::OnPrepare(IFormatterObject* pObject, std::tstring& strErrMsg)
	{
		return m_Channel.CheckValidity(strErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	inline void InsertIndentation(size_t tLevel, core::IChannel& m_Channel)
	{
		static const size_t s_tIndentLength = 2;
		static LPCTSTR s_pszIndent = TEXT("  ");
		for (size_t i = 0; i < tLevel; i++)
			m_Channel.Access((void*)s_pszIndent, sizeof(TCHAR) * s_tIndentLength);
	}

	//////////////////////////////////////////////////////////////////////////
	void CYAMLSerializer::OnBeginRoot(std::tstring& strRootName)
	{
		std::tstring strToken = strRootName + TEXT(":\n");
		m_Channel.Access((void*)strToken.c_str(), sizeof(TCHAR) * strToken.length());

		sGroupingData newObjectCountInfo(GT_OBJECT, 0x7FFFFFFF);
		m_vecObjectCountStack.push_back(newObjectCountInfo);
	}

	//////////////////////////////////////////////////////////////////////////
	void CYAMLSerializer::OnEndRoot()
	{
		if (m_vecObjectCountStack.empty())
			return;
			m_vecObjectCountStack.pop_back();
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CYAMLSerializer::OnBeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		InsertIndentation(m_vecObjectCountStack.size(), m_Channel);

		std::tstring strToken = strKey + TEXT(":\n");
		if (0 == tSize)
			strToken = strKey + TEXT(": {}\n");
		else
			strToken = strKey + TEXT(":\n");
		m_Channel.Access((void*)strToken.c_str(), sizeof(TCHAR) * strToken.length());

		sGroupingData newObjectCountInfo(GT_DICTIONARY, tSize);
		m_vecObjectCountStack.push_back(newObjectCountInfo);
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CYAMLSerializer::OnEndDictionary()
	{
		if (m_vecObjectCountStack.empty())
			return;
		m_vecObjectCountStack.pop_back();
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CYAMLSerializer::OnBeginArray(std::tstring& strKey, const size_t tSize)
	{
		InsertIndentation(m_vecObjectCountStack.size(), m_Channel);

		std::tstring strToken = strKey + TEXT(":\n");
		if (0 == tSize)
			strToken = strKey + TEXT(": []\n");
		else
			strToken = strKey + TEXT(":\n");
		m_Channel.Access((void*)strToken.c_str(), sizeof(TCHAR) * strToken.length());

		sGroupingData newObjectCountInfo(GT_ARRAY, tSize);
		m_vecObjectCountStack.push_back(newObjectCountInfo);
		return tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	void CYAMLSerializer::OnEndArray()
	{
		if (m_vecObjectCountStack.empty())
			return;
		m_vecObjectCountStack.pop_back();
	}

	//////////////////////////////////////////////////////////////////////////
	void CYAMLSerializer::OnBeginObject(std::tstring& strKey)
	{
		sGroupingData& topGroupData = m_vecObjectCountStack.back();
		if (topGroupData.nType == GT_ARRAY)
		{
			// array 항목의 dictionary에서는 key가 의미없음
			// ex)  routes:
			//        - to: 0.0.0.0/0
			//          via: 1.2.3.4
		}
		else
		{
			InsertIndentation(m_vecObjectCountStack.size(), m_Channel);

			std::tstring strToken = strKey + TEXT(":\n");
			m_Channel.Access((void*)strToken.c_str(), sizeof(TCHAR) * strToken.length());
		}

		sGroupingData newObjectCountInfo(GT_OBJECT, 0x7FFFFFFF);
		m_vecObjectCountStack.push_back(newObjectCountInfo);
	}

	//////////////////////////////////////////////////////////////////////////
	void CYAMLSerializer::OnEndObject()
	{
		if (m_vecObjectCountStack.empty())
			return;
		m_vecObjectCountStack.pop_back();
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CYAMLSerializer::OnSync(std::tstring& strKey, std::ntstring* pValue)
	{
		std::tstring strTempString = TCSFromNTCS(*pValue);
		OnSync(strKey, &strTempString);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline void YAMLSerializerMetaSync(std::vector<sGroupingData>& m_vecObjectCountStack, core::IChannel& m_Channel, std::tstring& strKey, T& refValue)
	{
		const size_t tStackCount = m_vecObjectCountStack.size();
		sGroupingData* pGrandparentGroupData = NULL;
		if (2 <= tStackCount)
			pGrandparentGroupData = &m_vecObjectCountStack[tStackCount - 2];

		sGroupingData& parentGroupData = m_vecObjectCountStack.back();
		if (parentGroupData.nType == GT_ARRAY)
		{
			InsertIndentation(m_vecObjectCountStack.size(), m_Channel);

			std::tstring strToken = TEXT("- ") + EncodeYamlString(refValue) + TEXT("\n");
			m_Channel.Access((void*)strToken.c_str(), sizeof(TCHAR) * strToken.length());
		}
		else if (parentGroupData.nType == GT_OBJECT)
		{
			if (pGrandparentGroupData && 
				pGrandparentGroupData->nType == GT_ARRAY && 
				parentGroupData.tSequence++ == 0)
			{
				// 첫번째 item 항목은 "- "을 넣어줘야 함
				InsertIndentation(m_vecObjectCountStack.size() - 1, m_Channel);
				std::tstring strToken = TEXT("- ");
				m_Channel.Access((void*)strToken.c_str(), sizeof(TCHAR) * strToken.length());
			}
			else
				InsertIndentation(m_vecObjectCountStack.size(), m_Channel);

			std::tstring strToken = strKey + TEXT(": ") + EncodeYamlString(refValue) + TEXT("\n");
			m_Channel.Access((void*)strToken.c_str(), sizeof(TCHAR) * strToken.length());
		}
		else
		{
			InsertIndentation(m_vecObjectCountStack.size(), m_Channel);

			std::tstring strToken = strKey + TEXT(": ") + EncodeYamlString(refValue) + TEXT("\n");
			m_Channel.Access((void*)strToken.c_str(), sizeof(TCHAR) * strToken.length());
		}
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CYAMLSerializer::OnSync(std::tstring& strKey, std::tstring* pString)
	{
		YAMLSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pString);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CYAMLSerializer::OnSync(std::tstring& strKey, bool* pValue)
	{
		YAMLSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CYAMLSerializer::OnSync(std::tstring& strKey, char* pValue)
	{
		YAMLSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CYAMLSerializer::OnSync(std::tstring& strKey, short* pValue)
	{
		YAMLSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CYAMLSerializer::OnSync(std::tstring& strKey, int32_t* pValue)
	{
		YAMLSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CYAMLSerializer::OnSync(std::tstring& strKey, int64_t* pValue)
	{
		YAMLSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CYAMLSerializer::OnSync(std::tstring& strKey, BYTE* pValue)
	{
		YAMLSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CYAMLSerializer::OnSync(std::tstring& strKey, WORD* pValue)
	{
		YAMLSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CYAMLSerializer::OnSync(std::tstring& strKey, DWORD* pValue)
	{
		YAMLSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CYAMLSerializer::OnSync(std::tstring& strKey, QWORD* pValue)
	{
		YAMLSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CYAMLSerializer::OnSync(std::tstring& strKey, float* pValue)
	{
		YAMLSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CYAMLSerializer::OnSync(std::tstring& strKey, double* pValue)
	{
		YAMLSerializerMetaSync(m_vecObjectCountStack, m_Channel, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CYAMLSerializer::OnSync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		if (GT_ARRAY != m_vecObjectCountStack.back().nType)
		{
		}

		return *this;
	}
}
