#pragma once

#include "IMemoryAccess.h"

namespace core
{
	class CSharedMemory : public IMemoryAccess
	{
		HANDLE		m_hFileMap;
		void*		m_pMemory;
		size_t		m_tSize;

	public:
		CSharedMemory(void);
		~CSharedMemory(void);

		ECODE		Create(DWORD dwKey, size_t tSize, E_PAGE_PROTECTION nProtectMode, DWORD dwFileMapAccess);
		void		Destroy(void);

		LPBYTE		Ptr(void)		{	return (LPBYTE)m_pMemory;	}
		size_t		Size(void)		{	return m_tSize;				}
	};
}