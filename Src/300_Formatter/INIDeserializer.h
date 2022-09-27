#pragma once

#include <string>
#include <vector>
#include <stack>

#include "Interface.h"
#include "Pair.h"
#include "FmtTypes.h"
#include "IChannel.h"
#include "INIFormatter.h"

namespace fmt_internal
{
	class CINIDeserializer : public core::IFormatterT
	{
	private:
		std::stack<sGroupingData>	m_stkGroupData;
		CINIFormatter	m_INI;
		std::tstring	m_strRootSection;
		std::tstring	m_strErrMsg;
		std::vector<std::tstring> m_vecKeys;
		CStringMap		m_mapArrayValues;
		bool			m_bValidity;
		bool			m_bReserved[7];

	public:
						CINIDeserializer(core::IChannel& channel, std::tstring strSection = TEXT(""));
						~CINIDeserializer(void);

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
