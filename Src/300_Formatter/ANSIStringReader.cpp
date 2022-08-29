#include "stdafx.h"
#include "ANSIStringReader.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CANSIStringReader::CANSIStringReader(LPCBYTE pContext, size_t tSize)
		: IChannel()
		, m_tReadPos(0)
		, m_strContext()
	{
		core::ST_BOM_INFO stBom;
		core::E_BOM_TYPE nType = ReadBOM(pContext, tSize, stBom);

		switch(nType)
		{
		case BOM_UTF8:
			{
				std::string strUTF8;
				strUTF8.resize(tSize - stBom.tSize);
				memcpy((void*)strUTF8.c_str(), pContext + stBom.tSize, tSize - stBom.tSize);
				m_strContext = TCSFromUTF8(strUTF8.c_str());
			}
			break;

		case BOM_UTF16:
			{
				std::wstring strWCS;
				strWCS.resize((tSize - stBom.tSize) / 2);
				memcpy((void*)strWCS.c_str(), pContext + stBom.tSize, tSize - stBom.tSize);
				m_strContext = TCSFromWCS(strWCS);
			}
			break;

		case BOM_UTF32:
			{
				std::vector<DWORD> vecUTF32;
				vecUTF32.resize((tSize - stBom.tSize) / 4);
				memcpy((void*)&vecUTF32[0], pContext + stBom.tSize, tSize - stBom.tSize);
				m_strContext = TCSFromUTF32(&vecUTF32[0], vecUTF32.size());
			}
			break;

		case BOM_UNDEFINED:
		default:
			m_strContext.resize(tSize / sizeof(TCHAR));
			memcpy((void*)m_strContext.c_str(), pContext, tSize);
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	CANSIStringReader::CANSIStringReader(const char* pszContext)
		: IChannel()
		, m_tReadPos(0)
		, m_strContext()
	{
		m_strContext = TCSFromANSI(pszContext);
	}

	//////////////////////////////////////////////////////////////////////////
	CANSIStringReader::CANSIStringReader(std::string& strContext)
		: IChannel()
		, m_tReadPos(0)
		, m_strContext()
	{
		m_strContext = TCSFromANSI(strContext);
	}

	//////////////////////////////////////////////////////////////////////////
	CANSIStringReader::~CANSIStringReader(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool CANSIStringReader::CheckValidity(std::tstring& refStrErrMsg)
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CANSIStringReader::Access(void* pData, size_t tDataSize)
	{
		tDataSize /= sizeof(TCHAR);

		const size_t tContextSize = m_strContext.size();
		const size_t tRemainedSize = tContextSize < m_tReadPos? 0 : tContextSize - m_tReadPos;
		if( tRemainedSize == 0 )
			return 0;

		if( tDataSize > tRemainedSize )
		{
			memcpy(pData, &m_strContext[m_tReadPos], tRemainedSize * sizeof(TCHAR));
			m_tReadPos += tRemainedSize;
			return tRemainedSize * sizeof(TCHAR);
		}

		memcpy(pData, &m_strContext[m_tReadPos], tDataSize * sizeof(TCHAR));
		m_tReadPos += tDataSize;
		return tDataSize * sizeof(TCHAR);
	}
}
