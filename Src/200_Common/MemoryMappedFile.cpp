#include "stdafx.h"
#include "MemoryMappedFile.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CMemoryMappedFile::CMemoryMappedFile(void)
	: IMemoryAccess()
	, m_hFileMap(NULL)
	, m_pMemory(NULL)
	, m_tSize(0)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CMemoryMappedFile::~CMemoryMappedFile(void)
	{
		Destroy();
	}

	//////////////////////////////////////////////////////////////////////////
	void CMemoryMappedFile::Destroy(void)
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