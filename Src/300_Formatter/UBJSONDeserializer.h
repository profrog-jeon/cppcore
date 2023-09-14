#pragma once

#include <string>
#include <stack>

#include "Interface.h"
#include "Pair.h"
#include "FmtTypes.h"
#include "IChannel.h"
#include "UBJSONFunctions.h"

namespace fmt_internal
{
	class CUBJSONDeserializer : public core::CFormatterSuper
	{
	public:
		struct sGroupingInfo
		{
			E_GROUPING_TYPE nType;
			ST_UBJ_NODE* pNode;
			size_t tReadPos;
			sGroupingInfo(E_GROUPING_TYPE t, ST_UBJ_NODE* pUBJNode)
				: nType(t), pNode(pUBJNode), tReadPos(0) {}
		};

		ST_UBJ_NODE					m_RootNode;
		std::tstring				m_strErrMsg;
		std::stack<sGroupingInfo>	m_GroupingStack;
		bool						m_bValidity;
		bool						m_bReserved[7];

	public:
		CUBJSONDeserializer(core::IChannel& channel);
		~CUBJSONDeserializer(void);

		bool			CheckValidity(std::tstring* pStrErrMsg) { if (pStrErrMsg)	*pStrErrMsg = m_strErrMsg;	return m_bValidity; }

	private:
		void			BeginRoot();
		void			EndRoot();

		size_t			BeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey);
		void			EndDictionary();

		size_t			BeginArray(std::tstring& strKey, const size_t tSize);
		void			BeginArrayItem(size_t tIndex, size_t tCount);
		void			EndArrayItem(size_t tIndex, size_t tCount);
		void			EndArray();

		void			BeginObject(std::tstring& strKey);
		void			EndObject();

		core::IFormatter& Sync(std::tstring& strKey, std::wstring* pValue);
		core::IFormatter& Sync(std::tstring& strKey, std::string* pValue);
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
