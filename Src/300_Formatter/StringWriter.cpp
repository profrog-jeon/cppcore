#include "stdafx.h"
#include "StringWriter.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CStringWriter::CStringWriter(std::tstring& strOutput)
	: IChannel()
	, m_strOutput(strOutput)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CStringWriter::~CStringWriter(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool CStringWriter::CheckValidity(std::tstring& refStrErrMsg)
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CStringWriter::Access(void* pData, size_t tDataSize)
	{
		std::tstring strTemp;
		strTemp.resize(tDataSize / sizeof(TCHAR));
		memcpy(&strTemp[0], pData, tDataSize);

		m_strOutput = m_strOutput + strTemp;
		return tDataSize;
	}
}
