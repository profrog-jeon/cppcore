#include "stdafx.h"
#include "FileReader.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CFileReader::CFileReader(std::tstring strFilename)
		: IChannel()
		, m_nBomType(BOM_UNDEFINED)
		, m_strFilename(strFilename)
		, m_pFile(NULL)
	{
		FILE* pFile = NULL;
		try
		{
			pFile = fopenT(strFilename.c_str(), TEXT("rb"));
			if( pFile == NULL )
				throw exception_format(TEXT("`%s` cannot be opened!!"), strFilename.c_str());

			m_nBomType = ReadBOM(pFile);
		}
		catch(std::exception& e)
		{
			if( pFile )
				fclose(pFile);
			Log_Error("%s", e.what());
			return;
		}

		m_pFile = pFile;
	}

	//////////////////////////////////////////////////////////////////////////
	CFileReader::~CFileReader(void)
	{
		if( m_pFile )
		{
			fclose(m_pFile);
			m_pFile = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool CFileReader::CheckValidity(std::tstring& refStrErrMsg)
	{
		if( m_pFile == NULL )
		{
			refStrErrMsg = Format(TEXT("Read failure `%s`"), m_strFilename.c_str());
			return false;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CFileReader::OnAccess(void* pData, size_t tDataSize)
	{
		if( m_pFile == NULL )
			return 0;

		size_t tRead = fread(pData, 1, tDataSize, m_pFile);
		return tRead;
	}
}

