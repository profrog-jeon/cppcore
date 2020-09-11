#include "stdafx.h"
#include "TextFileWriter.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CTextFileWriter::~CTextFileWriter(void)
	{
		if( m_hFile )
		{
			FlushFileBuffers(m_hFile);
			CloseFile(m_hFile);
			m_hFile = NULL;
		}
	}
}