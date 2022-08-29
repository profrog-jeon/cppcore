#include "stdafx.h"
#include "BytesReader.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CBytesReader::CBytesReader(const void* pBytes, size_t tBytesSize)
		: IChannel()
		, m_pBytes((const BYTE*)pBytes)
		, m_tBytesSize(tBytesSize)
		, m_tReadPos(0)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CBytesReader::~CBytesReader(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool CBytesReader::CheckValidity(std::tstring& refStrErrMsg)
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CBytesReader::Access(void* pData, size_t tDataSize)
	{
		const size_t tContextSize = m_tBytesSize;
		const size_t tRemainedSize = tContextSize < m_tReadPos? 0 : tContextSize - m_tReadPos;
		if( tRemainedSize == 0 )
			return 0;

		if( tDataSize > tRemainedSize )
		{
			memcpy(pData, &m_pBytes[m_tReadPos], tRemainedSize);
			m_tReadPos += tRemainedSize;
			return tRemainedSize;
		}

		memcpy(pData, &m_pBytes[m_tReadPos], tDataSize);
		m_tReadPos += tDataSize;
		return tDataSize;
	}
}
