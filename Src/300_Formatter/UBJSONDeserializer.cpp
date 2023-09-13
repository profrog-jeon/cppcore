#include "stdafx.h"
#include "UBJSONDeserializer.h"
#include "JSONFunctions.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CUBJSONDeserializer::CUBJSONDeserializer(core::IChannel& channel)
		: core::IFormatterT(channel)
		, m_GroupingStack()
		, m_bValidity(false)
		, m_strContext()
		, m_vecJsonToken()
		, m_strErrMsg()
	{
		if( !(m_bValidity = channel.CheckValidity(m_strErrMsg)) )
			return;

		while(1)
		{
			const size_t tTokenSize = 1024;
			TCHAR szBuff[tTokenSize+1] = { 0, };
			size_t tReadSize = channel.Access(szBuff, sizeof(TCHAR) * tTokenSize);
			if( 0 == tReadSize )
				break;

			m_strContext += szBuff;
		}

		std::vector<std::tstring> vecToken;
		Scan(m_strContext, vecToken);
		m_bValidity = Parse(vecToken, m_vecJsonToken, m_strErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	CUBJSONDeserializer::CUBJSONDeserializer(core::IChannel& channel, std::vector<std::tstring>& vecChunk)
		: core::IFormatterT(channel)
		, m_GroupingStack()
		, m_bValidity(false)
		, m_strContext()
		, m_vecJsonToken()
		, m_strErrMsg()
	{
		m_bValidity = Parse(vecChunk, m_vecJsonToken, m_strErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	CUBJSONDeserializer::~CUBJSONDeserializer(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CUBJSONDeserializer::BeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey)
	{
		sGroupingInfo& topGroupInfo = m_GroupingStack.top();

		size_t tIndex = 0;
		size_t i;
		for(i=0; i<topGroupInfo.vecChunk.size(); i++)
		{
			if( topGroupInfo.vecChunk[i].strKey != strKey )
				continue;

			if( tIndex++ < topGroupInfo.tReadPos )
				continue;

			sGroupingInfo newGroupingInfo(GT_DICTIONARY);

			std::vector<sToken> vecJsonToken;
			m_bValidity = Parse(topGroupInfo.vecChunk[i].vecToken, vecJsonToken, m_strErrMsg);
			Build(vecJsonToken, newGroupingInfo.vecChunk);

			if( bAllowMultiKey )
			{
				size_t j;
				for(j=0; j<newGroupingInfo.vecChunk.size(); j++)
				{
					if( newGroupingInfo.vecChunk[j].vecToken.size() > 1 )
						ExpandArray(j, newGroupingInfo.vecChunk);
				}
			}

			m_GroupingStack.push(newGroupingInfo);

			return newGroupingInfo.vecChunk.size();
		}
		
		m_GroupingStack.push(sGroupingInfo(GT_DICTIONARY));
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	void CUBJSONDeserializer::EndDictionary()
	{
		if( m_GroupingStack.top().nType == GT_DICTIONARY )
			m_GroupingStack.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CUBJSONDeserializer::BeginArray(std::tstring& strKey, const size_t tSize)
	{
		sGroupingInfo& topGroupInfo = m_GroupingStack.top();

		size_t i;
		if( GT_DICTIONARY == topGroupInfo.nType )
		{
			i = topGroupInfo.tReadPos++;
			strKey = topGroupInfo.vecChunk[i].strKey;
		}

		for(i=0; i<topGroupInfo.vecChunk.size(); i++)
		{
			if( topGroupInfo.vecChunk[i].strKey != strKey )
				continue;

			break;
		}

		if( i >= topGroupInfo.vecChunk.size() )
		{
			m_GroupingStack.push(sGroupingInfo(GT_ARRAY));
			return 0;
		}

		sGroupingInfo newGroupingInfo(GT_ARRAY);

		// context has single item
		if( topGroupInfo.vecChunk[i].vecToken.size() == 1 )
		{
			newGroupingInfo.vecChunk.push_back(topGroupInfo.vecChunk[i]);
		}
		// context has multiple(array) item
		else
		{
			CTokenVec vecJsonToken;
			m_bValidity = Parse(topGroupInfo.vecChunk[i].vecToken, vecJsonToken, m_strErrMsg);
			Build(vecJsonToken, newGroupingInfo.vecChunk);

			size_t j;
			for(j=0; j<newGroupingInfo.vecChunk.size(); j++)
				newGroupingInfo.vecChunk[j].strKey = topGroupInfo.vecChunk[i].strKey;
		}

		m_GroupingStack.push(newGroupingInfo);

		return newGroupingInfo.vecChunk.size();
	}

	//////////////////////////////////////////////////////////////////////////
	void CUBJSONDeserializer::BeginArrayItem(size_t tIndex, size_t tCount)
	{
		sGroupingInfo& topGroupInfo = m_GroupingStack.top();
		topGroupInfo.tReadPos = tIndex;
	}

	//////////////////////////////////////////////////////////////////////////
	void CUBJSONDeserializer::EndArrayItem(size_t tIndex, size_t tCount)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void CUBJSONDeserializer::EndArray()
	{
		if( m_GroupingStack.top().nType == GT_ARRAY )
			m_GroupingStack.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	void CUBJSONDeserializer::BeginObject(std::tstring& strKey)
	{
		sGroupingInfo& topGroupInfo = m_GroupingStack.top();

		// find a array sequenctial token
		if( GT_ARRAY == topGroupInfo.nType )
		{
			size_t tIndex = topGroupInfo.tReadPos;
			topGroupInfo.tReadPos++;

			if( tIndex < topGroupInfo.vecChunk.size() )
			{
				sGroupingInfo newGroupingInfo(GT_OBJECT);
				CTokenVec vecJsonToken;
				m_bValidity = Parse(topGroupInfo.vecChunk[tIndex].vecToken, vecJsonToken, m_strErrMsg);
				Build(vecJsonToken, newGroupingInfo.vecChunk);

				m_GroupingStack.push(newGroupingInfo);
				return;
			}				
		}

		// find a array sequenctial token
		if( GT_DICTIONARY == topGroupInfo.nType )
		{
			size_t tIndex = topGroupInfo.tReadPos;
			topGroupInfo.tReadPos++;

			if( tIndex < topGroupInfo.vecChunk.size() )
			{
				sGroupingInfo newGroupingInfo(GT_OBJECT);
				CTokenVec vecJsonToken;
				m_bValidity = Parse(topGroupInfo.vecChunk[tIndex].vecToken, vecJsonToken, m_strErrMsg);
				Build(vecJsonToken, newGroupingInfo.vecChunk);

				strKey = topGroupInfo.vecChunk[tIndex].strKey;
				m_GroupingStack.push(newGroupingInfo);
				return;
			}				
		}

		// find a key matched token
		size_t i;
		for(i=0; i<topGroupInfo.vecChunk.size(); i++)
		{
			if( topGroupInfo.vecChunk[i].strKey != strKey )
				continue;

			sGroupingInfo newGroupingInfo(GT_OBJECT);
			CTokenVec vecJsonToken;
			m_bValidity = Parse(topGroupInfo.vecChunk[i].vecToken, vecJsonToken, m_strErrMsg);
			Build(vecJsonToken, newGroupingInfo.vecChunk);

			m_GroupingStack.push(newGroupingInfo);
			return;
		}

		// Add dummy group info
		m_GroupingStack.push(sGroupingInfo(GT_OBJECT));
	}

	//////////////////////////////////////////////////////////////////////////
	void CUBJSONDeserializer::EndObject()
	{
		if( m_GroupingStack.top().nType == GT_OBJECT )
			m_GroupingStack.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	void CUBJSONDeserializer::BeginRoot()
	{
		sGroupingInfo newGroupingInfo(GT_ROOT);
		Build(m_vecJsonToken, newGroupingInfo.vecChunk);
		m_GroupingStack.push(newGroupingInfo);
	}

	//////////////////////////////////////////////////////////////////////////
	void CUBJSONDeserializer::EndRoot()
	{
		if( m_GroupingStack.top().nType == GT_ROOT )
			m_GroupingStack.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	inline void MakeValueStringFromJsonChunk(std::tstring* pString, sChunk& jsonChunk)
	{
		if( jsonChunk.vecToken.size() == 1 )
		{
			*pString = RestoreFromJsonString(jsonChunk.vecToken[0]);
		}
		else
		{
			
			size_t i;
			for(i=0; i<jsonChunk.vecToken.size(); i++)
				*pString += jsonChunk.vecToken[i];
		}
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CUBJSONDeserializer::Sync(std::tstring& strKey, std::tstring* pValue)
	{
		if( !m_GroupingStack.empty() )
		{
			sGroupingInfo& topGroupingInfo = m_GroupingStack.top();
			if( topGroupingInfo.nType == GT_ARRAY )
			{
				if( topGroupingInfo.tReadPos < topGroupingInfo.vecChunk.size() )
				{
					sChunk& jsonChunk = topGroupingInfo.vecChunk[topGroupingInfo.tReadPos++];
					MakeValueStringFromJsonChunk(pValue, jsonChunk);
				}
				return *this;
			}
			if( topGroupingInfo.nType == GT_DICTIONARY )
			{
				if( topGroupingInfo.tReadPos < topGroupingInfo.vecChunk.size() )
				{
					sChunk& jsonChunk = topGroupingInfo.vecChunk[topGroupingInfo.tReadPos++];
					strKey = jsonChunk.strKey;
					MakeValueStringFromJsonChunk(pValue, jsonChunk);
				}
				return *this;
			}
			if( topGroupingInfo.nType == GT_OBJECT)
			{
				size_t i;
				for(i=0; i<topGroupingInfo.vecChunk.size(); i++)
				{
					sChunk& jsonChunk = topGroupingInfo.vecChunk[i];
					if( jsonChunk.strKey != strKey )
						continue;

					strKey = jsonChunk.strKey;
					MakeValueStringFromJsonChunk(pValue, jsonChunk);
					break;
				}
				return *this;
			}

			size_t i;
			for(i=0; i<topGroupingInfo.vecChunk.size(); i++)
			{
				if( topGroupingInfo.vecChunk[i].strKey != strKey )
					continue;

				MakeValueStringFromJsonChunk(pValue, topGroupingInfo.vecChunk[i]);
			}
		}

		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT & CUBJSONDeserializer::Sync(std::tstring & strKey, std::ntstring * pValue)
	{
		std::tstring strTempString = TCSFromNTCS(*pValue);
		Sync(strKey, &strTempString);
		*pValue = NTCSFromTCS(strTempString);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline void JSONDeserializerMetaSync(std::stack<CUBJSONDeserializer::sGroupingInfo>& refGroupingStack, std::tstring& strKey, T& refValue)
	{
		try
		{
			if( refGroupingStack.empty() )
				throw exception_format(TEXT("GroupStack is ZERO!"));

			CUBJSONDeserializer::sGroupingInfo& topGroupingInfo = refGroupingStack.top();

			if( topGroupingInfo.nType == CUBJSONDeserializer::GT_ARRAY
			||  topGroupingInfo.nType == CUBJSONDeserializer::GT_DICTIONARY )
			{
				if( topGroupingInfo.tReadPos < topGroupingInfo.vecChunk.size() )
				{
					sChunk& jsonChunk = topGroupingInfo.vecChunk[topGroupingInfo.tReadPos++];
					if( jsonChunk.vecToken.empty() )
						throw exception_format(TEXT("Empty chunk has been found!"));

					strKey = jsonChunk.strKey;
					{
						std::tstring strValue =  RestoreFromJsonString(jsonChunk.vecToken[0]);
						refValue = ValueFrom<T>(strValue);
					}		
				}
			}
			else
			{
				size_t i;
				for(i=0; i<topGroupingInfo.vecChunk.size(); i++)
				{
					sChunk& jsonChunk = topGroupingInfo.vecChunk[i];
					if( strKey != jsonChunk.strKey )
						continue;

					if( jsonChunk.vecToken.empty() )
						throw exception_format(TEXT("Empty chunk has been found!"));

					std::tstring strValue =  RestoreFromJsonString(jsonChunk.vecToken[0]);
					refValue = ValueFrom<T>(strValue);
				}
			}
		}
		catch(std::exception& e)
		{
			Log_Error("%s - %s", __FUNCTION__, e.what());
		}
	}

	//////////////////////////////////////////////////////////////////////////
	core::IFormatterT& CUBJSONDeserializer::Sync(std::tstring& strKey, bool* pValue)
	{
		JSONDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatterT& CUBJSONDeserializer::Sync(std::tstring& strKey, char* pValue)
	{
		JSONDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatterT& CUBJSONDeserializer::Sync(std::tstring& strKey, short* pValue)
	{
		JSONDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatterT& CUBJSONDeserializer::Sync(std::tstring& strKey, int32_t* pValue)
	{
		JSONDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatterT& CUBJSONDeserializer::Sync(std::tstring& strKey, int64_t* pValue)
	{
		JSONDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatterT& CUBJSONDeserializer::Sync(std::tstring& strKey, BYTE* pValue)
	{
		JSONDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatterT& CUBJSONDeserializer::Sync(std::tstring& strKey, WORD* pValue)
	{
		JSONDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatterT& CUBJSONDeserializer::Sync(std::tstring& strKey, DWORD* pValue)
	{
		JSONDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatterT& CUBJSONDeserializer::Sync(std::tstring& strKey, QWORD* pValue)
	{
		JSONDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatterT& CUBJSONDeserializer::Sync(std::tstring& strKey, float* pValue)
	{
		JSONDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}
	core::IFormatterT& CUBJSONDeserializer::Sync(std::tstring& strKey, double* pValue)
	{
		JSONDeserializerMetaSync(m_GroupingStack, strKey, *pValue);
		return *this;
	}

	core::IFormatterT& CUBJSONDeserializer::Sync(std::tstring& strKey, std::vector<BYTE>* pvecData)
	{
		return *this;
	}
}
