#include "stdafx.h"
#include "BytesWriter.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CBytesWriter::CBytesWriter(std::vector<BYTE>& vecOutput)
	: IChannel()
	, m_vecOutput(vecOutput)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CBytesWriter::~CBytesWriter(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool CBytesWriter::CheckValidity(std::tstring& refStrErrMsg)
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CBytesWriter::Access(void* pData, size_t tDataSize)
	{
		size_t tCurSize = m_vecOutput.size();
		m_vecOutput.resize(tCurSize + tDataSize);
		memcpy(&m_vecOutput[tCurSize], pData, tDataSize);
		return tDataSize;
	}
}
