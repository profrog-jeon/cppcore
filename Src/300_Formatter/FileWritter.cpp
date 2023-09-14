#include "stdafx.h"
#include "FileWritter.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CFileWritter::CFileWritter(std::tstring strFilename)
		: IChannel()
		, m_strFilename(strFilename)
		, m_hFile(NULL)
	{
		HANDLE hFile = NULL;
		try
		{
			hFile = CreateFile(strFilename.c_str(), GENERIC_WRITE_, CREATE_ALWAYS_, 0);
			if( hFile == NULL )
			{
				int nErrCode = GetLastError();
				throw exception_format(TEXT("CreateFile(%s, GENERIC_WRITE_, CREATE_ALWAYS_) failure, %d"), strFilename.c_str(), nErrCode);
			}
		}
		catch(std::exception& e)
		{
			if( hFile )
				CloseFile(hFile);

			Log_Error("%s", e.what());
			return;
		}

		m_hFile = hFile;
	}

	//////////////////////////////////////////////////////////////////////////
	CFileWritter::~CFileWritter(void)
	{
		if( m_hFile )
		{
			FlushFileBuffers(m_hFile);
			CloseFile(m_hFile);
			m_hFile = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool CFileWritter::CheckValidity(std::tstring& refStrErrMsg)
	{
		if( m_hFile )
			return true;

		refStrErrMsg = Format(TEXT("File named `%s` open for write failure!"), m_strFilename.c_str());
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CFileWritter::Access(void* pData, size_t tDataSize)
	{
		if( m_hFile == NULL )
			return 0;

		DWORD dwWrittenSize = 0;
		if (!WriteFile(m_hFile, pData, (DWORD)tDataSize, &dwWrittenSize))
			return 0;

		return dwWrittenSize;
	}
}
