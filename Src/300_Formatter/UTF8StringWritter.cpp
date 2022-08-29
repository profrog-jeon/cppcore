#include "stdafx.h"
#include "UTF8StringWritter.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CUTF8StringWriter::CUTF8StringWriter(std::string& strOutput)
		: IChannel()
		, m_strOutput(strOutput)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CUTF8StringWriter::~CUTF8StringWriter(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool CUTF8StringWriter::CheckValidity(std::tstring& refStrErrMsg)
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CUTF8StringWriter::Access(void* pData, size_t tDataSize)
	{
		std::tstring strTemp;
		strTemp.resize(tDataSize / sizeof(TCHAR));
		memcpy(&strTemp[0], pData, tDataSize);

		m_strOutput = m_strOutput + UTF8FromTCS(strTemp);
		return tDataSize;
	}
}
