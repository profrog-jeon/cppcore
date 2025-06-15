#pragma once

#include <string>
#include <vector>
#include <stack>

#include "Interface.h"
#include "FmtTypes.h"
#include "IChannel.h"
#include "INIFormatter.h"

namespace fmt_internal
{
	class CINIDeserializer : public CFormatterSuper
	{
	private:
		std::stack<sGroupingData>	m_stkGroupData;
		CINIFormatter	m_INI;
		std::tstring	m_strRootSection;
		std::vector<std::tstring> m_vecKeys;
		CStringMap		m_mapArrayValues;

	public:
						CINIDeserializer(core::IChannel& channel, std::tstring strSection = TEXT(""));
						~CINIDeserializer(void);

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
	};
}
