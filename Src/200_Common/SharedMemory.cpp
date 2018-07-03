#include "stdafx.h"
#include "SharedMemory.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CSharedMemory::CSharedMemory(void)
	: IMemoryAccess()
	, m_hFileMap(NULL)
	, m_pMemory(NULL)
	, m_tSize(0)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CSharedMemory::~CSharedMemory(void)
	{
		Destroy();
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CSharedMemory::Create(DWORD dwKey, size_t tSize, E_PAGE_PROTECTION nProtectMode, DWORD dwFileMapAccess)
	{
		ECODE nRet = EC_SUCCESS;
		HANDLE hFileMap = NULL;
		void* pMemory = NULL;

		try
		{
			hFileMap = CreateFileMapping(NULL, nProtectMode, dwFileMapAccess, tSize, dwKey);
			nRet = GetLastError();
			if( NULL == hFileMap )
				throw exception_format(TEXT("CreateFileMapping(%u, %lu) failure"), dwKey, tSize);

			pMemory = MapViewOfFile(hFileMap, 0, tSize);
			nRet = GetLastError();
			if( NULL == pMemory )
				throw exception_format(TEXT("MapViewOfFile(%u, %lu) failure"), dwKey, tSize);
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			if( pMemory )
				UnmapViewOfFile(m_hFileMap, pMemory);
			if( hFileMap )
				CloseFileMappingHandle(hFileMap);
			return nRet;
		}

		m_hFileMap = hFileMap;
		m_pMemory = pMemory;
		m_tSize = tSize;
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	void CSharedMemory::Destroy(void)
	{
		if( m_pMemory )
		{
			UnmapViewOfFile(m_hFileMap, m_pMemory);
			m_pMemory = NULL;
		}

		if( m_hFileMap )
		{
			CloseFileMappingHandle(m_hFileMap);
			m_hFileMap = NULL;
		}
	}
}