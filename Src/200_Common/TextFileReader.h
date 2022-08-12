#pragma once

#include <stdio.h>
#include "../000_String/BOM.h"
#include "Uncopyable.h"
#include "MemoryMappedFile.h"

namespace core
{
	class CTextFileReader : private Uncopyable
	{
		CMemoryMappedFile m_MemMappedFile;
		LPCBYTE m_pContext;

		E_BOM_TYPE	m_nBOMType;
		size_t		m_tFileSize;
		size_t		m_tReadPos;

	public:
		CTextFileReader(std::string strFileName, E_BOM_TYPE nEncodeType = BOM_UNDEFINED);
		CTextFileReader(std::wstring strFileName, E_BOM_TYPE nEncodeType = BOM_UNDEFINED);
		~CTextFileReader(void);

		bool		IsNotValid(void);
		bool		IsEof(void);
		ECODE		ReadLine(std::string& strContext);
		ECODE		ReadLine(std::wstring& strContext);
	};
}
