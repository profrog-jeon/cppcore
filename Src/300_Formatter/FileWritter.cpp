#include "stdafx.h"
#include "FileWritter.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CFileWritter::CFileWritter(std::tstring strFilename)
		: IChannel()
		, m_strFilename(strFilename)
		, m_pFile(NULL)
	{
		FILE* pFile = NULL;
		try
		{
			pFile = fopenT(strFilename.c_str(), TEXT("w+b"));
			if( pFile == NULL )
				throw exception_format(TEXT("`%s` file cannot be written!"), strFilename.c_str());
		}
		catch(std::exception& e)
		{
			if( pFile )
				::fclose(pFile);

			Log_Error("%s", e.what());
			return;
		}

		m_pFile = pFile;
	}

	//////////////////////////////////////////////////////////////////////////
	CFileWritter::~CFileWritter(void)
	{
		if( m_pFile )
		{
			fclose(m_pFile);
			m_pFile = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool CFileWritter::CheckValidity(std::tstring& refStrErrMsg)
	{
		if( m_pFile )
			return true;

		refStrErrMsg = Format(TEXT("File named `%s` open for write failure!"), m_strFilename.c_str());
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CFileWritter::OnAccess(void* pData, size_t tDataSize)
	{
		if( m_pFile == NULL )
			return 0;

		fwrite(pData, 1, tDataSize, m_pFile);
		return tDataSize;
	}
}
