#pragma once

#include <string>
#include <vector>

#include "Interface.h"
#include "Pair.h"
#include "FmtTypes.h"
#include "IChannel.h"

namespace fmt_internal
{
	class CXMLSerializer_V3 : public core::IFormatterT
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

		bool			CheckValidity(std::tstring* pStrErrMsg)	{	return true;	}

	private:
		size_t			BeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey);
		void			EndDictionary();

		size_t			BeginArray(std::tstring& strKey, const size_t tSize);
		void			EndArray();

		void			BeginObject(std::tstring& strKey);
		void			EndObject();

		void			BeginRoot();
		void			EndRoot();

		core::IFormatterT& Sync(std::tstring& strKey, std::tstring* pValue);
		core::IFormatterT& Sync(std::tstring& strKey, std::ntstring* pValue);
		core::IFormatterT& Sync(std::tstring& strKey, bool* pValue);
		core::IFormatterT& Sync(std::tstring& strKey, char* pValue);
		core::IFormatterT& Sync(std::tstring& strKey, short* pValue);
		core::IFormatterT& Sync(std::tstring& strKey, int32_t* pValue);
		core::IFormatterT& Sync(std::tstring& strKey, int64_t* pValue);
		core::IFormatterT& Sync(std::tstring& strKey, BYTE* pValue);
		core::IFormatterT& Sync(std::tstring& strKey, WORD* pValue);
		core::IFormatterT& Sync(std::tstring& strKey, DWORD* pValue);
		core::IFormatterT& Sync(std::tstring& strKey, QWORD* pValue);
		core::IFormatterT& Sync(std::tstring& strKey, float* pValue);
		core::IFormatterT& Sync(std::tstring& strKey, double* pValue);
		core::IFormatterT& Sync(std::tstring& strKey, std::vector<BYTE>* pvecData);

	private:
		std::tstring BuildXmlContext(size_t tIndent, const ST_INTERNAL_TREE_DATA& stData);
	};
}

