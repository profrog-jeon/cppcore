#pragma once

#include <vector>
#include "CriticalSection.h"

namespace core
{
	class CRingBuffer
	{
		CCriticalSection m_csPos;
		size_t m_tPos;
		std::vector<BYTE> m_vecBuffer;

	public:
		CRingBuffer(void);
		~CRingBuffer(void);

		ECODE Create(size_t tSize);
		void Destroy(void);

		LPBYTE Alloc(size_t tSize, size_t tReservedSize = 0);
	};
}
