#include "stdafx.h"
#include "FileReader.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CFileReader::CFileReader(std::tstring strFilename)
		: IChannel()
		, m_nBomType(BOM_UNDEFINED)
		, m_strFilename(strFilename)
		, m_hFile(NULL)
	{
		HANDLE hFile = NULL;
		try
		{
			hFile = CreateFile(strFilename.c_str(), GENERIC_READ_, OPEN_EXISTING_, 0);
			if (hFile == NULL)
			{
				int nErrCode = GetLastError();
				throw exception_format(TEXT("CreateFile(%s, GENERIC_READ_, OPEN_EXISTING_) failure, %d"), strFilename.c_str(), nErrCode);
			}

			m_nBomType = ReadBOM(hFile);
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
	CFileReader::~CFileReader(void)
	{
		if( m_hFile )
		{
			CloseFile(m_hFile);
			m_hFile = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool CFileReader::CheckValidity(std::tstring& refStrErrMsg)
	{
		if( m_hFile == NULL )
		{
			refStrErrMsg = Format(TEXT("Read failure `%s`"), m_strFilename.c_str());
			return false;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CFileReader::Access(void* pData, size_t tDataSize)
	{
		if( m_hFile == NULL )
			return 0;

		DWORD dwReadSize = 0;
		if (!ReadFile(m_hFile, pData, (DWORD)tDataSize, &dwReadSize))
			return 0;

		return dwReadSize;
	}
}

