#include "stdafx.h"
#include "ANSIFileReader.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CANSIFileReader::CANSIFileReader(std::tstring strFilename)
		: IChannel()
		, m_strFilename(strFilename)
		, m_hFile(NULL)
		, m_strBuffer()
		, m_strContext()
	{
		HANDLE hFile = NULL;
		try
		{
			hFile = CreateFile(strFilename.c_str(), GENERIC_READ_, OPEN_EXISTING_, 0);
			if( hFile == NULL )
			{
				int nErrCode = GetLastError();
				throw exception_format(TEXT("CreateFile(%s, GENERIC_READ_, OPEN_EXISTING_) failure, %d"), strFilename.c_str(), nErrCode);
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
	CANSIFileReader::~CANSIFileReader(void)
	{
		if( m_hFile )
		{
			CloseFile(m_hFile);
			m_hFile = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool CANSIFileReader::CheckValidity(std::tstring& refStrErrMsg)
	{
		if( m_hFile == NULL )
		{
			refStrErrMsg = Format(TEXT("Read failure `%s`"), m_strFilename.c_str());
			return false;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CANSIFileReader::Access(void* pData, size_t tDataSize)
	{
		if( m_hFile == NULL )
			return 0;

		const DWORD dwBuffSize = 1024;
		char szBuff[dwBuffSize+1];

		DWORD dwRead = 0;
		if( !ReadFile(m_hFile, szBuff, dwBuffSize, &dwRead) && m_strContext.empty() )
			return 0;

		szBuff[dwRead] = 0;
		m_strBuffer += szBuff;

		size_t tReadSize = 0;
		m_strContext += TCSFromANSI(m_strBuffer, &tReadSize);
		if( tReadSize < m_strBuffer.size() )
			m_strBuffer = m_strBuffer.substr(tReadSize);
		else
			m_strBuffer = "";

		const void* pContextData = m_strContext.c_str();
		size_t tContextSize = m_strContext.size() * sizeof(TCHAR);
		if( tDataSize <= tContextSize )
		{
			memcpy(pData, pContextData, tDataSize);			
			std::tstring strTruncatedContext = m_strContext.substr(tDataSize / sizeof(TCHAR));
			m_strContext = strTruncatedContext;
			return tDataSize;
		}

		memcpy(pData, pContextData, tContextSize);
		m_strContext.clear();
		return tContextSize;
	}
}

