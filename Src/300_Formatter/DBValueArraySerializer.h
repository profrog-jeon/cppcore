#pragma once

#include <string>
#include <vector>

#include "Interface.h"
#include "FmtTypes.h"
#include "IChannel.h"

namespace fmt_internal
{
	class CDBValueArraySerializer : public CFormatterSuper
	{
	private:
		bool			m_bFirst;
		bool			m_bReserved[7];

	public:
						CDBValueArraySerializer(core::IChannel& channel);
						~CDBValueArraySerializer(void);

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
