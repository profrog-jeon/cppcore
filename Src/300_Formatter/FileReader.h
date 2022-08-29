#pragma once

#include <stdio.h>

#include "../__Common/Type.h"
#include "../__Common/Define.h"
#include "../000_String/BOM.h"
#include "FmtTypes.h"
#include "IChannel.h"

namespace fmt_internal
{
	class CFileReader : public core::IChannel
	{
	private:
		E_BOM_TYPE		m_nBomType		;
		std::tstring	m_strFilename	;
		HANDLE			m_hFile			;

	public:
						CFileReader(std::tstring strFilename);
						~CFileReader(void);

		bool			CheckValidity(std::tstring& refStrErrMsg);

	private:
		size_t			Access(void* pData, size_t tDataSize);
	};

}
