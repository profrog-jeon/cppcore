#include "stdafx.h"
#include "UBJSONDeserializer.h"
#include "UBJSONFunctions.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CUBJSONDeserializer::CUBJSONDeserializer(core::IChannel& channel)
		: CFormatterSuper(channel)
		, m_GroupingStack()

	{
	}

	//////////////////////////////////////////////////////////////////////////
	CUBJSONDeserializer::~CUBJSONDeserializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool CUBJSONDeserializer::OnPrepare(IFormatterObject* pObject, std::tstring& strErrMsg)
	{
		if (!m_Channel.CheckValidity(strErrMsg))
			return false;

		ECODE nRet = ParseUBJson(m_Channel, m_RootNode, strErrMsg);
		if (EC_SUCCESS != nRet)
		{
			Log_Error(TEXT("ParseUBJson failure, %d(%s)"), nRet, strErrMsg.c_str());
			return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	void CUBJSONDeserializer::OnBeginRoot(std::tstring& strRootName)
	{
		sGroupingInfo newGroupingInfo(GT_ROOT, &m_RootNode);
		m_GroupingStack.push(newGroupingInfo);
	}

	//////////////////////////////////////////////////////////////////////////
	void CUBJSONDeserializer::OnEndRoot()
	{
		if (m_GroupingStack.top().nType == GT_ROOT)
			m_GroupingStack.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CUBJSONDeserializer::OnBeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		std::string strKeyU8 = UTF8FromTCS(strKey);

		sGroupingInfo* pParent = &m_GroupingStack.top();
		for (ST_UBJ_NODE& child : pParent->pNode->Children)
		{
			if (child.strKey != strKeyU8)
				continue;

			m_GroupingStack.push(sGroupingInfo(GT_DICTIONARY, &child));
			return child.Children.size();
		}

		m_GroupingStack.push(sGroupingInfo(GT_DICTIONARY, pParent->pNode));
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	void CUBJSONDeserializer::OnEndDictionary()
	{
		if( m_GroupingStack.top().nType == GT_DICTIONARY )
			m_GroupingStack.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CUBJSONDeserializer::OnBeginArray(std::tstring& strKey, const size_t tSize)
	{
		std::string strKeyU8 = UTF8FromTCS(strKey);

		sGroupingInfo* pParent = &m_GroupingStack.top();
		for (ST_UBJ_NODE& child : pParent->pNode->Children)
		{
			if (child.strKey != strKeyU8)
				continue;

			m_GroupingStack.push(sGroupingInfo(GT_ARRAY, &child));
			return child.Children.size();
		}

		m_GroupingStack.push(sGroupingInfo(GT_ARRAY, pParent->pNode));
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	void CUBJSONDeserializer::OnBeginArrayItem(size_t tIndex, size_t tCount)
	{
		sGroupingInfo& topGroupInfo = m_GroupingStack.top();
		topGroupInfo.tReadPos = tIndex;
	}

	//////////////////////////////////////////////////////////////////////////
	void CUBJSONDeserializer::OnEndArrayItem(size_t tIndex, size_t tCount)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CUBJSONDeserializer::OnEndArray()
	{
		if( m_GroupingStack.top().nType == GT_ARRAY )
			m_GroupingStack.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	void CUBJSONDeserializer::OnBeginObject(std::tstring& strKey)
	{
		std::string strKeyU8 = UTF8FromTCS(strKey);

		sGroupingInfo* pParent = &m_GroupingStack.top();

		// find a array sequential token
		if (GT_ARRAY == pParent->nType)
		{
			size_t tIndex = pParent->tReadPos;
			pParent->tReadPos++;

			if (tIndex < pParent->pNode->Children.size())
			{
				strKey = TCSFromUTF8(pParent->pNode->Children[tIndex].strKey);
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
				strKey = TCSFromUTF8(pParent->pNode->Children[tIndex].strKey);
				m_GroupingStack.push(sGroupingInfo(GT_OBJECT, &pParent->pNode->Children[tIndex]));
				return;
			}
		}

		for (ST_UBJ_NODE& child : pParent->pNode->Children)
		{
			if (child.strKey != strKeyU8)
				continue;

			m_GroupingStack.push(sGroupingInfo(GT_OBJECT, &child));
			return;
		}

		m_GroupingStack.push(sGroupingInfo(GT_OBJECT, pParent->pNode));
	}

	//////////////////////////////////////////////////////////////////////////
	void CUBJSONDeserializer::OnEndObject()
	{
		if( m_GroupingStack.top().nType == GT_OBJECT )
			m_GroupingStack.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CUBJSONDeserializer::OnSync(std::tstring& strKey, std::tstring* pValue)
	{
		std::string strKeyU8 = UTF8FromTCS(strKey);

		if (m_GroupingStack.empty())
			return *this;

		sGroupingInfo& topGroupingInfo = m_GroupingStack.top();
		if( topGroupingInfo.nType == GT_ARRAY )
		{
			if( topGroupingInfo.tReadPos < topGroupingInfo.pNode->Children.size() )
			{
				ST_UBJ_NODE& stNode = topGroupingInfo.pNode->Children[topGroupingInfo.tReadPos++];
				strKey = TCSFromUTF8(stNode.strKey);
				*pValue = TCSFromUTF8(stNode.strValue);
			}
			return *this;
		}
		if( topGroupingInfo.nType == GT_DICTIONARY )
		{
			if (topGroupingInfo.tReadPos < topGroupingInfo.pNode->Children.size())
			{
				ST_UBJ_NODE& stNode = topGroupingInfo.pNode->Children[topGroupingInfo.tReadPos++];
				strKey = TCSFromUTF8(stNode.strKey);
				*pValue = TCSFromUTF8(stNode.strValue);
			}
			return *this;
		}
		if( topGroupingInfo.nType == GT_OBJECT)
		{
			if (topGroupingInfo.tReadPos < topGroupingInfo.pNode->Children.size())
			{
				ST_UBJ_NODE& stNode = topGroupingInfo.pNode->Children[topGroupingInfo.tReadPos++];
				strKey = TCSFromUTF8(stNode.strKey);
				*pValue = TCSFromUTF8(stNode.strValue);
			}

			size_t i;
			for(i=0; i< topGroupingInfo.pNode->Children.size(); i++)
			{
				ST_UBJ_NODE& curNode = topGroupingInfo.pNode->Children[i];
				if(curNode.strKey != strKeyU8)
					continue;

				strKey = TCSFromUTF8(curNode.strKey);
				*pValue = TCSFromUTF8(curNode.strValue);
				break;
			}
			return *this;
		}

		size_t i;
		for (i = 0; i < topGroupingInfo.pNode->Children.size(); i++)
		{
			ST_UBJ_NODE& curNode = topGroupingInfo.pNode->Children[i];
			if (curNode.strKey != strKeyU8)
				continue;

			*pValue = TCSFromUTF8(curNode.strValue);
		}
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CUBJSONDeserializer::OnSync(std::tstring & strKey, std::ntstring * pValue)
	{
		std::tstring strTempString = TCSFromNTCS(*pValue);
		OnSync(strKey, &strTempString);
		*pValue = NTCSFromTCS(strTempString);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline void UBJSONDeserializerMetaSync(std::stack<CUBJSONDeserializer::sGroupingInfo>& refGroupingStack, std::tstring& strKey, T& refValue)
	{
		if( refGroupingStack.empty() )
			return;

		CUBJSONDeserializer::sGroupingInfo& topGroupingInfo = refGroupingStack.top();

		if( topGroupingInfo.nType == GT_ARRAY
		||  topGroupingInfo.nType == GT_DICTIONARY )
		{
			if( topGroupingInfo.tReadPos < topGroupingInfo.pNode->Children.size() )
			{
				ST_UBJ_NODE& stNode = topGroupingInfo.pNode->Children[topGroupingInfo.tReadPos++];
				strKey = TCSFromUTF8(stNode.strKey);
				refValue = ValueFrom<T>(TCSFromUTF8(stNode.strValue));
			}
		}
		else
		{
			std::string strKeyU8 = UTF8FromTCS(strKey);
			
			size_t i;
			for(i=0; i<topGroupingInfo.pNode->Children.size(); i++)
			{
				ST_UBJ_NODE& stNode = topGroupingInfo.pNode->Children[i];
				if(strKeyU8 != stNode.strKey )
					continue;

				refValue = ValueFrom<T>(TCSFromUTF8(stNode.strValue));
				return;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatter& CUBJSONDeserializer::OnSync(std::tstring& strKey, bool* pValue)
	{
		UBJSONDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CUBJSONDeserializer::OnSync(std::tstring& strKey, char* pValue)
	{
		UBJSONDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CUBJSONDeserializer::OnSync(std::tstring& strKey, short* pValue)
	{
		UBJSONDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CUBJSONDeserializer::OnSync(std::tstring& strKey, int32_t* pValue)
	{
		UBJSONDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CUBJSONDeserializer::OnSync(std::tstring& strKey, int64_t* pValue)
	{
		UBJSONDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CUBJSONDeserializer::OnSync(std::tstring& strKey, BYTE* pValue)
	{
		UBJSONDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CUBJSONDeserializer::OnSync(std::tstring& strKey, WORD* pValue)
	{
		UBJSONDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CUBJSONDeserializer::OnSync(std::tstring& strKey, DWORD* pValue)
	{
		UBJSONDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CUBJSONDeserializer::OnSync(std::tstring& strKey, QWORD* pValue)
	{
		UBJSONDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CUBJSONDeserializer::OnSync(std::tstring& strKey, float* pValue)
	{
		UBJSONDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatter& CUBJSONDeserializer::OnSync(std::tstring& strKey, double* pValue)
	{
		UBJSONDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}

	core::IFormatter& CUBJSONDeserializer::OnSync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		auto& GroupInfo = m_GroupingStack.top();

		std::string strKeyU8 = UTF8FromTCS(strKey);
		for (ST_UBJ_NODE& node : GroupInfo.pNode->Children)
		{
			if (strKeyU8 != node.strKey)
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
