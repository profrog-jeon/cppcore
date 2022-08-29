#include "stdafx.h"
#include "ANSIStringWritter.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CANSIStringWriter::CANSIStringWriter(std::string& strOutput)
		: IChannel()
		, m_strOutput(strOutput)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CANSIStringWriter::~CANSIStringWriter(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool CANSIStringWriter::CheckValidity(std::tstring& refStrErrMsg)
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CANSIStringWriter::Access(void* pData, size_t tDataSize)
	{
		std::tstring strTemp;
		strTemp.resize(tDataSize / sizeof(TCHAR));
		memcpy(&strTemp[0], pData, tDataSize);

		m_strOutput = m_strOutput + ANSIFromTCS(strTemp);
		return tDataSize;
	}
}
