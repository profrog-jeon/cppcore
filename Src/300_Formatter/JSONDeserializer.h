#pragma once

#include <string>
#include <stack>

#include "Interface.h"
#include "Pair.h"
#include "FmtTypes.h"
#include "IChannel.h"
#include "JSONFunctions.h"

namespace fmt_internal
{
	class CJSONDeserializer : public core::CFormatterSuper
	{
	public:
		struct sGroupingInfo
		{
			E_GROUPING_TYPE nType;
			size_t tReadPos;
			CChunkVec vecChunk;
			sGroupingInfo(E_GROUPING_TYPE t)
				: nType(t), tReadPos(0), vecChunk()	{}
		};

	private:
		std::stack<sGroupingInfo>	m_GroupingStack;
		bool						m_bValidity;
		bool						m_bReserved[7];
		std::tstring				m_strContext;
		std::vector<ST_JSON_CHUNK>			m_vecJsonToken;
		std::tstring				m_strErrMsg;

	public:
						CJSONDeserializer(core::IChannel& channel);
						CJSONDeserializer(core::IChannel& channel, std::vector<std::tstring>& vecChunk);
						~CJSONDeserializer(void);

		bool			CheckValidity(std::tstring* pStrErrMsg)		{	if( pStrErrMsg )	*pStrErrMsg = m_strErrMsg;	return m_bValidity;		}

	private:
		size_t			BeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey);
		void			EndDictionary();

		size_t			BeginArray(std::tstring& strKey, const size_t tSize);
		void			BeginArrayItem(size_t tIndex, size_t tCount);
		void			EndArrayItem(size_t tIndex, size_t tCount);
		void			EndArray();

		void			BeginObject(std::tstring& strKey);
		void			EndObject();

		void			BeginRoot();
		void			EndRoot();

		core::IFormatter& Sync(std::tstring& strKey, std::tstring* pValue);
		core::IFormatter& Sync(std::tstring& strKey, std::ntstring* pValue);
		core::IFormatter& Sync(std::tstring& strKey, bool* pValue);
		core::IFormatter& Sync(std::tstring& strKey, char* pValue);
		core::IFormatter& Sync(std::tstring& strKey, short* pValue);
		core::IFormatter& Sync(std::tstring& strKey, int32_t* pValue);
		core::IFormatter& Sync(std::tstring& strKey, int64_t* pValue);
		core::IFormatter& Sync(std::tstring& strKey, BYTE* pValue);
		core::IFormatter& Sync(std::tstring& strKey, WORD* pValue);
		core::IFormatter& Sync(std::tstring& strKey, DWORD* pValue);
		core::IFormatter& Sync(std::tstring& strKey, QWORD* pValue);
		core::IFormatter& Sync(std::tstring& strKey, float* pValue);
		core::IFormatter& Sync(std::tstring& strKey, double* pValue);
		core::IFormatter& Sync(std::tstring& strKey, std::vector<BYTE>* pvecData);
	};
}
