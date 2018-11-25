#include "stdafx.h"
#include "ASCIIStringWritter.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CASCIIStringWriter::CASCIIStringWriter(std::string& strOutput)
		: IChannel()
		, m_strOutput(strOutput)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CASCIIStringWriter::~CASCIIStringWriter(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool CASCIIStringWriter::CheckValidity(std::tstring& refStrErrMsg)
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CASCIIStringWriter::OnAccess(void* pData, size_t tDataSize)
	{
		std::tstring strTemp;
		strTemp.resize(tDataSize / sizeof(TCHAR));
		memcpy(&strTemp[0], pData, tDataSize);

		m_strOutput = m_strOutput + ASCIIFromTCS(strTemp);
		return tDataSize;
	}
}
