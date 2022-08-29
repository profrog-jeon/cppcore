#include "stdafx.h"
#include "ANSIFileWritter.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CANSIFileWritter::CANSIFileWritter(std::tstring strFilename)
		: IChannel()
		, m_strFilename(strFilename)
		, m_hFile(NULL)
	{
		HANDLE hFile = NULL;
		try
		{
			hFile = CreateFile(strFilename.c_str(), GENERIC_WRITE_, CREATE_ALWAYS_, 0);
			if (hFile == NULL)
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
	CANSIFileWritter::~CANSIFileWritter(void)
	{
		if( m_hFile )
		{
			FlushFileBuffers(m_hFile);
			CloseFile(m_hFile);
			m_hFile = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool CANSIFileWritter::CheckValidity(std::tstring& refStrErrMsg)
	{
		if( m_hFile )
			return true;

		refStrErrMsg = Format(TEXT("File named `%s` open for write failure!"), m_strFilename.c_str());
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CANSIFileWritter::Access(void* pData, size_t tDataSize)
	{
		if( m_hFile == NULL )
			return 0;

		std::string strContextEuckr;
		{
			std::tstring strContext;
			strContext.resize(tDataSize / sizeof(TCHAR));
			memcpy((void*)strContext.c_str(), pData, strContext.size() * sizeof(TCHAR));
			strContextEuckr = ANSIFromTCS(strContext);
		}
		
		DWORD dwWritten = 0;
		if (!WriteFile(m_hFile, strContextEuckr.c_str(), strContextEuckr.size(), &dwWritten))
			return 0;

		return dwWritten;
	}
}
