#include "stdafx.h"
#include "YAMLDeserializer.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CYAMLDeserializer::CYAMLDeserializer(core::IChannel& channel)
		: CFormatterSuper(channel)
		, m_GroupingStack()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CYAMLDeserializer::~CYAMLDeserializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool CYAMLDeserializer::OnPrepare(IFormatterObject* pObject, std::tstring& strErrMsg)
	{
		if (!m_Channel.CheckValidity(strErrMsg))
			return false;

		CYAMLParser YAML(m_Channel);
		ECODE nRet = YAML.Parse(m_RootNode, strErrMsg);
		if (EC_SUCCESS != nRet)
		{
			Log_Error(TEXT("ParseUBJson failure, %d(%s)"), nRet, strErrMsg.c_str());
			return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	void CYAMLDeserializer::OnBeginRoot(std::tstring& strRootName)
	{
		strRootName = m_RootNode.strKey;

		sGroupingInfo newGroupingInfo(GT_ROOT, &m_RootNode);
		m_GroupingStack.push(newGroupingInfo);
	}

	//////////////////////////////////////////////////////////////////////////
	void CYAMLDeserializer::OnEndRoot()
	{
		if (m_GroupingStack.top().nType == GT_ROOT)
			m_GroupingStack.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CYAMLDeserializer::OnBeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		sGroupingInfo* pParent = &m_GroupingStack.top();
		for (ST_YAML_NODE& child : pParent->pNode->Children)
		{
			if (child.strKey != strKey)
				continue;

			m_GroupingStack.push(sGroupingInfo(GT_DICTIONARY, &child));
			return child.Children.size();
		}

		m_GroupingStack.push(sGroupingInfo(GT_DICTIONARY, pParent->pNode));
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	void CYAMLDeserializer::OnEndDictionary()
	{
		if (m_GroupingStack.top().nType == GT_DICTIONARY)
			m_GroupingStack.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CYAMLDeserializer::OnBeginArray(std::tstring& strKey, const size_t tSize)
	{
		sGroupingInfo* pParent = &m_GroupingStack.top();
		for (ST_YAML_NODE& child : pParent->pNode->Children)
		{
			if (child.strKey != strKey)
				continue;

			m_GroupingStack.push(sGroupingInfo(GT_ARRAY, &child));
			return child.Children.size();
		}

		m_GroupingStack.push(sGroupingInfo(GT_ARRAY, pParent->pNode));
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	void CYAMLDeserializer::OnBeginArrayItem(size_t tIndex, size_t tCount)
	{
		sGroupingInfo& topGroupInfo = m_GroupingStack.top();
		topGroupInfo.tReadPos = tIndex;
	}

	//////////////////////////////////////////////////////////////////////////
	void CYAMLDeserializer::OnEndArrayItem(size_t tIndex, size_t tCount)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CYAMLDeserializer::OnEndArray()
	{
		if (m_GroupingStack.top().nType == GT_ARRAY)
			m_GroupingStack.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	void CYAMLDeserializer::OnBeginObject(std::tstring& strKey)
	{
		sGroupingInfo* pParent = &m_GroupingStack.top();

		// find a array sequential token
		if (GT_ARRAY == pParent->nType)
		{
			size_t tIndex = pParent->tReadPos;
			pParent->tReadPos++;

			if (tIndex < pParent->pNode->Children.size())
			{
				strKey = pParent->pNode->Children[tIndex].strKey;
				m_GroupingStack.push(sGroupingInfo(GT_OBJECT, &pParent->pNode->Children[tIndex]));
				return;
			}
		}

		// find a array sequential token
		if (GT_DICTIONARY == pParent->nType)
		{
			size_t tIndex = pParent->tReadPos;
			pParent->tReadPos++;

			if (tIndex < pParent->pNode->Children.size())
			{
				strKey = pParent->pNode->Children[tIndex].strKey;
				m_GroupingStack.push(sGroupingInfo(GT_OBJECT, &pParent->pNode->Children[tIndex]));
				return;
			}
		}

		for (ST_YAML_NODE& child : pParent->pNode->Children)
		{
			if (child.strKey != strKey)
				continue;

			m_GroupingStack.push(sGroupingInfo(GT_OBJECT, &child));
			return;
		}

		m_GroupingStack.push(sGroupingInfo(GT_OBJECT, pParent->pNode));
	}

	//////////////////////////////////////////////////////////////////////////
	void CYAMLDeserializer::OnEndObject()
	{
		if (m_GroupingStack.top().nType == GT_OBJECT)
			m_GroupingStack.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CYAMLDeserializer::OnSync(std::tstring& strKey, std::tstring* pValue)
	{
		if (m_GroupingStack.empty())
			return *this;

		sGroupingInfo& topGroupingInfo = m_GroupingStack.top();
		if (topGroupingInfo.nType == GT_ARRAY)
		{
			if (topGroupingInfo.tReadPos < topGroupingInfo.pNode->Children.size())
			{
				ST_YAML_NODE& stNode = topGroupingInfo.pNode->Children[topGroupingInfo.tReadPos++];
				if (!stNode.Children.empty())
					*pValue = DecodeYamlString(stNode.Children[0].strValue);
			}
			return *this;
		}
		if (topGroupingInfo.nType == GT_DICTIONARY)
		{
			if (topGroupingInfo.tReadPos < topGroupingInfo.pNode->Children.size())
			{
				ST_YAML_NODE& stNode = topGroupingInfo.pNode->Children[topGroupingInfo.tReadPos++];
				strKey = stNode.strKey;
				*pValue = DecodeYamlString(stNode.strValue);
			}
			return *this;
		}
		if (topGroupingInfo.nType == GT_OBJECT)
		{
			for (ST_YAML_NODE& curNode: topGroupingInfo.pNode->Children)
			{
				if (curNode.strKey != strKey)
					continue;

				*pValue = DecodeYamlString(curNode.strValue);
				break;
			}
			return *this;
		}

		for (ST_YAML_NODE& curNode : topGroupingInfo.pNode->Children)
		{
			if (curNode.strKey != strKey)
				continue;

			*pValue = DecodeYamlString(curNode.strValue);
		}
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CYAMLDeserializer::OnSync(std::tstring& strKey, std::ntstring* pValue)
	{
		std::tstring strTempString = TCSFromNTCS(*pValue);
		OnSync(strKey, &strTempString);
		*pValue = NTCSFromTCS(strTempString);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline void YAMLDeserializerMetaSync(std::stack<CYAMLDeserializer::sGroupingInfo>& refGroupingStack, std::tstring& strKey, T& refValue)
	{
		if (refGroupingStack.empty())
			return;

		CYAMLDeserializer::sGroupingInfo& topGroupingInfo = refGroupingStack.top();

		if (topGroupingInfo.nType == GT_ARRAY
		|| topGroupingInfo.nType == GT_DICTIONARY)
		{
			if (topGroupingInfo.tReadPos < topGroupingInfo.pNode->Children.size())
			{
				const size_t tIndex = topGroupingInfo.tReadPos++;
				ST_YAML_NODE& CurNode = topGroupingInfo.pNode->Children[tIndex];

				strKey = CurNode.strKey;
				refValue = DecodeYamlString<T>(CurNode.strValue);
			}
		}
		else
		{
			for (ST_YAML_NODE& child : topGroupingInfo.pNode->Children)
			{
				if (strKey != child.strKey)
					continue;
				refValue = DecodeYamlString<T>(child.strValue);
			}
		}

		int a = 123;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CYAMLDeserializer::OnSync(std::tstring& strKey, bool* pValue)
	{
		YAMLDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CYAMLDeserializer::OnSync(std::tstring& strKey, char* pValue)
	{
		YAMLDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CYAMLDeserializer::OnSync(std::tstring& strKey, short* pValue)
	{
		YAMLDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CYAMLDeserializer::OnSync(std::tstring& strKey, int32_t* pValue)
	{
		YAMLDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CYAMLDeserializer::OnSync(std::tstring& strKey, int64_t* pValue)
	{
		YAMLDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CYAMLDeserializer::OnSync(std::tstring& strKey, BYTE* pValue)
	{
		YAMLDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CYAMLDeserializer::OnSync(std::tstring& strKey, WORD* pValue)
	{
		YAMLDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CYAMLDeserializer::OnSync(std::tstring& strKey, DWORD* pValue)
	{
		YAMLDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CYAMLDeserializer::OnSync(std::tstring& strKey, QWORD* pValue)
	{
		YAMLDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CYAMLDeserializer::OnSync(std::tstring& strKey, float* pValue)
	{
		YAMLDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CYAMLDeserializer::OnSync(std::tstring& strKey, double* pValue)
	{
		YAMLDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}

	core::IFormatter& CYAMLDeserializer::OnSync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		auto& GroupInfo = m_GroupingStack.top();

		for (ST_YAML_NODE& node : GroupInfo.pNode->Children)
		{
			if (strKey != node.strKey)
				continue;

			if (!node.strValue.empty())
			{
				pvecData->resize(node.strValue.size());
				memcpy(pvecData->data(), node.strValue.c_str(), node.strValue.size());
			}
			break;
		}
		return *this;
	}
}
