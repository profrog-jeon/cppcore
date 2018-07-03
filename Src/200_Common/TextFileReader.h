#pragma once

#include <stdio.h>
#include "../000_String/BOM.h"
#include "Uncopyable.h"

namespace core
{
	class CTextFileReader : private Uncopyable
	{
		HANDLE		m_hFile;
		E_BOM_TYPE	m_nBOMType;
		QWORD		m_qwFileSize;

	public:
		CTextFileReader(std::string strFileName, E_BOM_TYPE nEncodeType = BOM_UNDEFINED);
		CTextFileReader(std::wstring strFileName, E_BOM_TYPE nEncodeType = BOM_UNDEFINED);
		~CTextFileReader(void);

		bool		IsNotValid(void);
		int64_t		Tell(void);
		int64_t		Seek(int64_t nOffset, E_FILE_MOVE_METHOD nOrigin);
		ECODE		ReadLine(std::string& strContext);
		ECODE		ReadLine(std::wstring& strContext);
	};
}
