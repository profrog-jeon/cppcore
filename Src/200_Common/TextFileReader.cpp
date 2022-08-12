#include "stdafx.h"
#include "TextFileReader.h"
#include "Utility.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CTextFileReader::~CTextFileReader(void)
	{
		if( m_pContext)
			m_MemMappedFile.Destroy();
		m_pContext = NULL;
		m_tFileSize = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	bool CTextFileReader::IsNotValid(void)
	{
		return (NULL == m_pContext);
	}

	//////////////////////////////////////////////////////////////////////////
	bool CTextFileReader::IsEof(void)
	{
		return m_tReadPos == m_tFileSize;
	}
}