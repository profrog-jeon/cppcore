#pragma once

#include <string>
#include <vector>
#include <vector>

#include "Interface.h"
#include "FmtTypes.h"
#include "IChannel.h"
#include "YAMLParser.h"

namespace fmt_internal
{
	class CYAMLDeserializer : public CFormatterSuper
	{
	public:
		struct sGroupingInfo
		{
			E_GROUPING_TYPE nType;
			ST_YAML_NODE* pNode;
			size_t tReadPos;
			sGroupingInfo(E_GROUPING_TYPE t, ST_YAML_NODE* pUBJNode)
				: nType(t), pNode(pUBJNode), tReadPos(0) {
			}
		};

		ST_YAML_NODE				m_RootNode;
		std::stack<sGroupingInfo>	m_GroupingStack;

	public:
		CYAMLDeserializer(core::IChannel& channel);
		~CYAMLDeserializer(void);

	private:
		bool			OnPrepare(IFormatterObject* pObject, std::tstring& strErrMsg);

		void			OnBeginRoot(std::tstring& strRootName);
		void			OnEndRoot();

		size_t			OnBeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey);
		void			OnEndDictionary();

		size_t			OnBeginArray(std::tstring& strKey, const size_t tSize);
		void			OnBeginArrayItem(size_t tIndex, size_t tCount);
		void			OnEndArrayItem(size_t tIndex, size_t tCount);
		void			OnEndArray();

		void			OnBeginObject(std::tstring& strKey);
		void			OnEndObject();

		core::IFormatter& OnSync(std::tstring& strKey, std::wstring* pValue);
		core::IFormatter& OnSync(std::tstring& strKey, std::string* pValue);
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
