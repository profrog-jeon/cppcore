#pragma once

#pragma once

#include <string>
#include <vector>
#include <stack>
#include <set>

#include "Interface.h"
#include "Pair.h"
#include "FmtTypes.h"
#include "IChannel.h"
#include "JSONFunctions.h"
#include "HelperFunc.h"

namespace fmt_internal
{
	typedef std::set<std::tstring>					CStringSet;
	typedef std::set<std::tstring>::iterator		CStringSetIt;
	typedef std::stack<std::tstring>				CStringStack;

	class CMarkerSerializer : public core::IFormatterT
	{	
		std::tstring	m_strErrMsg;
		std::tstring	m_strSchema;
		bool			m_bValidity;
		bool			m_bReserved[7];
		CStringSet		m_setMarker;
		CStringStack	m_stackTraverse;

	public:
		CMarkerSerializer(core::IChannel& channel, std::tstring strSchema);
		~CMarkerSerializer(void);

		bool			CheckValidity(std::tstring* pStrErrMsg)		{	if( pStrErrMsg )	*pStrErrMsg = m_strErrMsg;	return m_bValidity;		}

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
	};

}
