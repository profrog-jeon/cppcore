#pragma once

#include <string>
#include <vector>

#include "Interface.h"
#include "FmtTypes.h"
#include "IChannel.h"
#include "HelperFunc.h"

namespace fmt_internal
{
	class CValueArrayDeserializer : public CFormatterSuper
	{
	private:
		std::vector< std::vector<std::tstring> >	m_vecValueTable;
		std::tstring	m_strTokenDelimiter;
		std::tstring	m_strQuotation;
		size_t			m_tArrayIndex;
		size_t			m_tItemIndex;
		bool m_bSkipTitle;

	public:
		CValueArrayDeserializer(core::IChannel& channel, std::tstring strTokenDelimiter, std::tstring strQuotation, bool bSkipTitle = false);
		~CValueArrayDeserializer(void);

	private:
		bool			OnPrepare(IFormatterObject* pObject, std::tstring& strErrMsg);

		size_t			OnBeginDictionary(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey);
		void			OnEndDictionary();

		size_t			OnBeginArray(std::tstring& strKey, const size_t tSize);
		void			OnEndArray();

		void			OnBeginArrayItem(size_t tIndex, size_t tCount);
		void			OnEndArrayItem(size_t tIndex, size_t tCount);

		void			OnBeginObject(std::tstring& strKey);
		void			OnEndObject();

		void			OnBeginRoot(std::tstring& strRootName);
		void			OnEndRoot();

		core::IFormatter& OnSync(std::tstring& strKey, std::tstring* pString);
		core::IFormatter& OnSync(std::tstring& strKey, std::ntstring* pString);
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
