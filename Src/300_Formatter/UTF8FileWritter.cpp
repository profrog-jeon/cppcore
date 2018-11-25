#include "stdafx.h"
#include "UTF8FileWritter.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CUTF8FileWritter::CUTF8FileWritter(std::tstring strFilename, bool bUseBOM)
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

			if( bUseBOM )
				WriteBOM(pFile, BOM_UTF8);
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
	CUTF8FileWritter::~CUTF8FileWritter(void)
	{
		if( m_pFile )
		{
			fclose(m_pFile);
			m_pFile = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool CUTF8FileWritter::CheckValidity(std::tstring& refStrErrMsg)
	{
		if( m_pFile )
			return true;

		refStrErrMsg = Format(TEXT("File named `%s` open for write failure!"), m_strFilename.c_str());
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CUTF8FileWritter::OnAccess(void* pData, size_t tDataSize)
	{
		if( m_pFile == NULL )
			return 0;

		std::string strContextU8;
		{
			std::tstring strContext;
			strContext.resize(tDataSize / sizeof(TCHAR));
			memcpy((void*)strContext.c_str(), pData, strContext.size() * sizeof(TCHAR));
			strContextU8 = UTF8FromTCS(strContext);
		}
		
		fwrite(strContextU8.c_str(), 1, strContextU8.size(), m_pFile);
		return tDataSize;
	}
}
