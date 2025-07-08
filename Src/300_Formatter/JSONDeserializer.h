#pragma once

#include <string>
#include <stack>

#include "Interface.h"
#include "FmtTypes.h"
#include "IChannel.h"
#include "JSONFunctions.h"

namespace fmt_internal
{
	class CJSONDeserializer : public CFormatterSuper
	{
	public:
		struct sGroupingInfo
		{
			E_GROUPING_TYPE nType;
			size_t tReadPos;
			CJsonTokenVec vecChunk;
			sGroupingInfo(E_GROUPING_TYPE t)
				: nType(t), tReadPos(0), vecChunk()	{}
		};

	private:
		std::stack<sGroupingInfo>	m_GroupingStack;
		std::tstring				m_strContext;
		std::vector<std::tstring>	m_vecToken;
		std::vector<ST_JSON_CHUNK>	m_vecJsonToken;

	public:
						CJSONDeserializer(core::IChannel& channel);
						CJSONDeserializer(core::IChannel& channel, std::vector<std::tstring>& vecToken);
						~CJSONDeserializer(void);

	private:
		bool			OnPrepare(IFormatterObject* pObject, std::tstring& strErrMsg);
		
		size_t			OnBeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey);
		void			OnEndDictionary();

		size_t			OnBeginArray(std::tstring& strKey, const size_t tSize);
		void			OnBeginArrayItem(size_t tIndex, size_t tCount);
		void			OnEndArrayItem(size_t tIndex, size_t tCount);
		void			OnEndArray();

		void			OnBeginObject(std::tstring& strKey);
		void			OnEndObject();

		void			OnBeginRoot(std::tstring& strRootName);
		void			OnEndRoot();

		core::IFormatter& OnSync(std::tstring& strKey, std::tstring* pValue);
		core::IFormatter& OnSync(std::tstring& strKey, std::ntstring* pValue);
		core::IFormatter& OnSync(std::tstring& strKey, bool* pValue);
		core::IFormatter& OnSync(std::tstring& strKey, char* pValue);
		core::IFormatter& OnSync(std::tstring& strKey, short* pValue);
		core::IFormatter& OnSync(std::tstring& strKey, int32_t* pValue);
		core::IFormatter& OnSync(std::tstring& strKey, int64_t* pValue);
		core::IFormatter& OnSync(std::tstring& strKey, BYTE* pValue);
		core::IFormatter& OnSync(std::tstring& strKey, WORD* pValue);
		core::IFormatter& OnSync(std::tstring& strKey, DWORD* pValue);
		core::IFormatter& OnSync(std::tstring& strKey, QWORD* pValue);
		core::IFormatter& OnSync(std::tstring& strKey, float* pValue);
		core::IFormatter& OnSync(std::tstring& strKey, double* pValue);
		core::IFormatter& OnSync(std::tstring& strKey, std::vector<BYTE>* pvecData);
	};
}
