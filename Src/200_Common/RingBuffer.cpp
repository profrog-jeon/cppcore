#include "stdafx.h"
#include "RingBuffer.h"

namespace core
{
	CRingBuffer::CRingBuffer(void)
		: m_csPos()
		, m_tPos(0)
		, m_vecBuffer()
	{
	}

	CRingBuffer::~CRingBuffer(void)
	{
	}

	ECODE CRingBuffer::Create(size_t tSize)
	{
		m_vecBuffer.resize(tSize);
		if (m_vecBuffer.size() != tSize)
			return EC_NOT_ENOUGH_MEMORY;

		m_tPos = 0;
		return EC_SUCCESS;
	}

	void CRingBuffer::Destroy(void)
	{
		m_vecBuffer.clear();
	}

	LPBYTE CRingBuffer::Alloc(size_t tSize, size_t tReservedSize)
	{
		CCriticalSection::Owner lock(m_csPos);

		size_t tPrePos = m_tPos;

		m_tPos += tSize;
		if ((m_tPos + tReservedSize) <= m_vecBuffer.size())
			return &m_vecBuffer[tPrePos];

		m_tPos = tSize;
		return &m_vecBuffer[0];
	}
}
