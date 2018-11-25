#pragma once

#include <string>
#include <stack>

#include "Interface.h"
#include "Pair.h"
#include "FmtTypes.h"
#include "IChannel.h"
#include "JSONFunctions.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	class CJSONDeserializer : public core::IFormatterT
	{
	public:
		enum eGroupingType
		{
			GT_ROOT,
			GT_DICTIONARY,
			GT_ARRAY,
			GT_OBJECT,
			GT_COUNT
		};
		struct sGroupingInfo
		{
			eGroupingType nType;
			size_t tReadPos;
			fmt_internal::CChunkVec vecChunk;
			sGroupingInfo(eGroupingType t)
				: nType(t), tReadPos(0), vecChunk()	{}
		};

	private:
		std::stack<sGroupingInfo> m_GroupingStack;
		bool					m_bValidity;
		bool					m_bReserved[7];
		std::tstring			m_strContext;
		fmt_internal::CTokenVec m_vecJsonToken;
		std::tstring			m_strErrMsg;

	public:
						CJSONDeserializer(core::IChannel& channel);
						CJSONDeserializer(core::IChannel& channel, CTStringVec& vecChunk);
						~CJSONDeserializer(void);

		bool			CheckValidity(std::tstring* pStrErrMsg)		{	if( pStrErrMsg )	*pStrErrMsg = m_strErrMsg;	return m_bValidity;		}

	private:
		size_t			_BeginDictionaryGrouping(std::tstring& strKey, const size_t tSize, bool bAllowMultiKey);
		void			_EndDictionaryGrouping();

		size_t			_BeginArrayGrouping(std::tstring& strKey, const size_t tSize);
		void			_BeginArrayItem(size_t tIndex, size_t tCount);
		void			_EndArrayItem(size_t tIndex, size_t tCount);
		void			_EndArrayGrouping();

		void			_BeginObjectGrouping(std::tstring& strKey);
		void			_EndObjectGrouping();

		void			_BeginRootGrouping();
		void			_EndRootGrouping();

		core::IFormatterT& Sync(std::tstring& strKey, std::tstring* pString);
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
	};

}
