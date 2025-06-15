#pragma once

#pragma once

#include <string>
#include <vector>
#include <stack>
#include <set>

#include "Interface.h"
#include "FmtTypes.h"
#include "IChannel.h"
#include "JSONFunctions.h"
#include "HelperFunc.h"

namespace fmt_internal
{
	typedef std::set<std::tstring>					CStringSet;
	typedef std::set<std::tstring>::iterator		CStringSetIt;
	typedef std::stack<std::tstring>				CStringStack;

	class CMarkerSerializer : public CFormatterSuper
	{	
		std::tstring	m_strSchema;
		CStringSet		m_setMarker;
		CStringStack	m_stackTraverse;

	public:
		CMarkerSerializer(core::IChannel& channel, std::tstring strSchema);
		~CMarkerSerializer(void);

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
