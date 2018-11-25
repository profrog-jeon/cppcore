#include "stdafx.h"
#include "ASCIIFileWritter.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CASCIIFileWritter::CASCIIFileWritter(std::tstring strFilename)
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
	CASCIIFileWritter::~CASCIIFileWritter(void)
	{
		if( m_pFile )
		{
			fclose(m_pFile);
			m_pFile = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool CASCIIFileWritter::CheckValidity(std::tstring& refStrErrMsg)
	{
		if( m_pFile )
			return true;

		refStrErrMsg = Format(TEXT("File named `%s` open for write failure!"), m_strFilename.c_str());
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CASCIIFileWritter::OnAccess(void* pData, size_t tDataSize)
	{
		if( m_pFile == NULL )
			return 0;

		std::string strContextEuckr;
		{
			std::tstring strContext;
			strContext.resize(tDataSize / sizeof(TCHAR));
			memcpy((void*)strContext.c_str(), pData, strContext.size() * sizeof(TCHAR));
			strContextEuckr = ASCIIFromTCS(strContext);
		}
		
		fwrite(strContextEuckr.c_str(), 1, strContextEuckr.size(), m_pFile);
		return tDataSize;
	}
}
