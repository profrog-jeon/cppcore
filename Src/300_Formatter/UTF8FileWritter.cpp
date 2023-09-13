#include "stdafx.h"
#include "UTF8FileWritter.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CUTF8FileWritter::CUTF8FileWritter(std::tstring strFilename, bool bUseBOM)
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

			if( bUseBOM )
				WriteBOM(hFile, BOM_UTF8);
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
	CUTF8FileWritter::~CUTF8FileWritter(void)
	{
		if( m_hFile )
		{
			FlushFileBuffers(m_hFile);
			CloseFile(m_hFile);
			m_hFile = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool CUTF8FileWritter::CheckValidity(std::tstring& refStrErrMsg)
	{
		if( m_hFile )
			return true;

		refStrErrMsg = Format(TEXT("File named `%s` open for write failure!"), m_strFilename.c_str());
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CUTF8FileWritter::Access(void* pData, size_t tDataSize)
	{
		if( m_hFile == NULL )
			return 0;

		std::string strContextU8;
		{
			std::tstring strContext;
			strContext.resize(tDataSize / sizeof(TCHAR));
			memcpy((void*)strContext.c_str(), pData, strContext.size() * sizeof(TCHAR));
			strContextU8 = UTF8FromTCS(strContext);
		}
		
		DWORD dwWrittenSize = 0;
		WriteFile(m_hFile, strContextU8.c_str(), (DWORD)strContextU8.size(), &dwWrittenSize);
		return dwWrittenSize;
	}
}
