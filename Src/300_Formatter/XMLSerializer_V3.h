#pragma once

#include <string>
#include <vector>

#include "Interface.h"
#include "FmtTypes.h"
#include "IChannel.h"

namespace fmt_internal
{
	class CXMLSerializer_V3 : public CFormatterSuper
	{
		E_BOM_TYPE		m_nBOM;

		struct ST_INTERNAL_TREE_DATA
		{
			std::tstring strKey;
			std::tstring strValue;
			std::tstring strAttr;

			ST_INTERNAL_TREE_DATA(void) {}
			ST_INTERNAL_TREE_DATA(std::tstring key) : strKey(key) {}
		};

		std::stack<ST_INTERNAL_TREE_DATA> m_stkTraverse;
		std::stack<std::tstring> m_stkContext;

		std::tstring	m_strRootTag;
		std::tstring	m_strAttrContext;

	public:
		CXMLSerializer_V3(core::IChannel& channel, E_BOM_TYPE nBOM, std::tstring strRoot);
		~CXMLSerializer_V3(void);

	private:
		bool			OnPrepare(IFormatterObject* pObject, std::tstring& strErrMsg);

		size_t			OnBeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey);
		void			OnEndDictionary();

		size_t			OnBeginArray(std::tstring& strKey, const size_t tSize);
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

	private:
		std::tstring BuildXmlContext(size_t tIndent, const ST_INTERNAL_TREE_DATA& stData);
	};
}

