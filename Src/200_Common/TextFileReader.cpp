#include "stdafx.h"
#include "TextFileReader.h"
#include "Utility.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CTextFileReader::~CTextFileReader(void)
	{
		if( m_hFile )
			CloseFile(m_hFile);
		m_hFile = NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	bool CTextFileReader::IsNotValid(void)
	{
		return (NULL == m_hFile);
	}

	//////////////////////////////////////////////////////////////////////////
	int64_t	CTextFileReader::Tell(void)
	{
		int64_t nCurPos = SetFilePointer(m_hFile, 0, FILE_CURRENT_);
		if( 0 > nCurPos )
			return 0;
		return nCurPos;
	}

	//////////////////////////////////////////////////////////////////////////
	int64_t CTextFileReader::Seek(int64_t nOffset, E_FILE_MOVE_METHOD nOrigin)
	{
		int64_t nCurPos = SetFilePointer(m_hFile, nOffset, nOrigin);
		if( 0 > nCurPos )
			return 0;
		return nCurPos;
	}
}