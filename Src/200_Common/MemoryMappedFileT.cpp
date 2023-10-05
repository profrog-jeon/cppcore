#include "stdafx.h"
#include "MemoryMappedFile.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	ECODE CMemoryMappedFile::Create(std::tstring strFilePath, E_PAGE_PROTECTION nProtectMode, DWORD dwFileMapAccess)
	{
		ECODE nRet = EC_SUCCESS;
		HANDLE hFile = NULL;
		HANDLE hFileMap = NULL;
		void* pMemory = NULL;
		size_t tFileSize = 0;

		try
		{
			DWORD dwDesiredAccess = nProtectMode == PAGE_READONLY_ ? GENERIC_READ_ : (GENERIC_READ_ | GENERIC_WRITE_);
			hFile = CreateFile(strFilePath.c_str(), dwDesiredAccess, OPEN_EXISTING_, 0);
			nRet = GetLastError();
			if( NULL == hFile )
				throw exception_format(TEXT("CreateFile(%s) failure"), strFilePath.c_str());

			tFileSize = (size_t)GetFileSize(hFile);			
			hFileMap = CreateFileMapping(hFile, nProtectMode, dwFileMapAccess, tFileSize);
			nRet = GetLastError();
			if( NULL == hFileMap )
				throw exception_format(TEXT("CreateFileMapping(%s, %lu) failure"), strFilePath.c_str(), tFileSize);

			pMemory = MapViewOfFile(hFileMap, 0, tFileSize);
			nRet = GetLastError();
			if( NULL == pMemory )
				throw exception_format(TEXT("MapViewOfFile(%s, %lu) failure"), strFilePath.c_str(), tFileSize);

			m_hFileMap = hFileMap;
			m_pMemory = pMemory;
			m_tSize = tFileSize;
			CloseFile(hFile);
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			if( pMemory )
				UnmapViewOfFile(m_hFileMap, pMemory);
			if( hFileMap )
				CloseFileMappingHandle(hFileMap);
			if( hFile )
				CloseFile(hFile);
			return nRet;
		}

		return EC_SUCCESS;
	}
}