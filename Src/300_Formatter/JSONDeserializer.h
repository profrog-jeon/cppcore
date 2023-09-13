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
	class CJSONDeserializerA : public core::CFormatterSuperA
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
			CChunkVecA vecChunk;
			sGroupingInfo(eGroupingType t)
				: nType(t), tReadPos(0), vecChunk()	{}
		};

	private:
		std::stack<sGroupingInfo> m_GroupingStack;
		bool					m_bValidity;
		bool					m_bReserved[7];
		std::string				m_strContext;
		CTokenVecA				m_vecJsonToken;
		std::string				m_strErrMsg;

	public:
						CJSONDeserializerA(core::IChannel& channel);
						CJSONDeserializerA(core::IChannel& channel, std::vector<std::string>& vecChunk);
						~CJSONDeserializerA(void);

		bool			CheckValidity(std::string* pStrErrMsg)		{	if( pStrErrMsg )	*pStrErrMsg = m_strErrMsg;	return m_bValidity;		}

	private:
		size_t			BeginDictionary(std::string& strKey, const size_t tSize, bool bAllowMultiKey);
		void			EndDictionary();

		size_t			BeginArray(std::string& strKey, const size_t tSize);
		void			BeginArrayItem(size_t tIndex, size_t tCount);
		void			EndArrayItem(size_t tIndex, size_t tCount);
		void			EndArray();

		void			BeginObject(std::string& strKey);
		void			EndObject();

		void			BeginRoot();
		void			EndRoot();

		core::IFormatterA& Sync(std::string& strKey, std::string* pValue);
		core::IFormatterA& Sync(std::string& strKey, std::wstring* pValue);
		core::IFormatterA& Sync(std::string& strKey, bool* pValue);
		core::IFormatterA& Sync(std::string& strKey, char* pValue);
		core::IFormatterA& Sync(std::string& strKey, short* pValue);
		core::IFormatterA& Sync(std::string& strKey, int32_t* pValue);
		core::IFormatterA& Sync(std::string& strKey, int64_t* pValue);
		core::IFormatterA& Sync(std::string& strKey, BYTE* pValue);
		core::IFormatterA& Sync(std::string& strKey, WORD* pValue);
		core::IFormatterA& Sync(std::string& strKey, DWORD* pValue);
		core::IFormatterA& Sync(std::string& strKey, QWORD* pValue);
		core::IFormatterA& Sync(std::string& strKey, float* pValue);
		core::IFormatterA& Sync(std::string& strKey, double* pValue);
		core::IFormatterA& Sync(std::string& strKey, std::vector<BYTE>* pvecData);
	};

	class CJSONDeserializerW : public core::CFormatterSuperW
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
			CChunkVecW vecChunk;
			sGroupingInfo(eGroupingType t)
				: nType(t), tReadPos(0), vecChunk() {}
		};

	private:
		std::stack<sGroupingInfo> m_GroupingStack;
		bool					m_bValidity;
		bool					m_bReserved[7];
		std::wstring			m_strContext;
		std::vector<sTokenW>	m_vecJsonToken;
		std::wstring			m_strErrMsg;

	public:
		CJSONDeserializerW(core::IChannel& channel);
		CJSONDeserializerW(core::IChannel& channel, std::vector<std::wstring>& vecChunk);
		~CJSONDeserializerW(void);

		bool			CheckValidity(std::wstring* pStrErrMsg) { if (pStrErrMsg)	*pStrErrMsg = m_strErrMsg;	return m_bValidity; }

	private:
		size_t			BeginDictionary(std::wstring& strKey, const size_t tSize, bool bAllowMultiKey);
		void			EndDictionary();

		size_t			BeginArray(std::wstring& strKey, const size_t tSize);
		void			BeginArrayItem(size_t tIndex, size_t tCount);
		void			EndArrayItem(size_t tIndex, size_t tCount);
		void			EndArray();

		void			BeginObject(std::wstring& strKey);
		void			EndObject();

		void			BeginRoot();
		void			EndRoot();

		core::IFormatterW& Sync(std::wstring& strKey, std::wstring* pValue);
		core::IFormatterW& Sync(std::wstring& strKey, std::string* pValue);
		core::IFormatterW& Sync(std::wstring& strKey, bool* pValue);
		core::IFormatterW& Sync(std::wstring& strKey, char* pValue);
		core::IFormatterW& Sync(std::wstring& strKey, short* pValue);
		core::IFormatterW& Sync(std::wstring& strKey, int32_t* pValue);
		core::IFormatterW& Sync(std::wstring& strKey, int64_t* pValue);
		core::IFormatterW& Sync(std::wstring& strKey, BYTE* pValue);
		core::IFormatterW& Sync(std::wstring& strKey, WORD* pValue);
		core::IFormatterW& Sync(std::wstring& strKey, DWORD* pValue);
		core::IFormatterW& Sync(std::wstring& strKey, QWORD* pValue);
		core::IFormatterW& Sync(std::wstring& strKey, float* pValue);
		core::IFormatterW& Sync(std::wstring& strKey, double* pValue);
		core::IFormatterW& Sync(std::wstring& strKey, std::vector<BYTE>* pvecData);
	};

}
