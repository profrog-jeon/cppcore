#pragma once

#include "../000_String/BOM.h"
#include "Uncopyable.h"

namespace core
{
	class CTextFileWriter : private Uncopyable
	{
		HANDLE m_hFile;
		E_BOM_TYPE m_nBOMType;

	public:
		CTextFileWriter(std::string strFileName, E_BOM_TYPE nBOMType);
		CTextFileWriter(std::wstring strFileName, E_BOM_TYPE nBOMType);
		~CTextFileWriter(void);

		int		WriteLine(std::string strContext);
		int		WriteLine(std::wstring strContext);
	};
}
