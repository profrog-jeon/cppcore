#pragma once

#include "IMemoryAccess.h"
#include "../100_System/System.h"

namespace core
{
	class CMemoryMappedFile : public IMemoryAccess
	{
		HANDLE		m_hFileMap;
		void*		m_pMemory;
		size_t		m_tSize;

	public:
		CMemoryMappedFile(void);
		~CMemoryMappedFile(void);

		ECODE		Create(std::string strFilePath, E_PAGE_PROTECTION nProtectMode, DWORD dwFileMapAccess);
		ECODE		Create(std::wstring strFilePath, E_PAGE_PROTECTION nProtectMode, DWORD dwFileMapAccess);
		void		Destroy(void);

		LPBYTE		Ptr(void)	{	return (LPBYTE)m_pMemory;	}
		size_t		Size(void)	{	return m_tSize;				}
	};

}
