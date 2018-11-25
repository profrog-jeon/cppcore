#pragma once

#include <stdio.h>

#include "../__Common/Type.h"
#include "../__Common/Define.h"
#include "../000_String/BOM.h"
#include "FmtTypes.h"
#include "IChannel.h"

namespace fmt_internal
{
	class CASCIIFileReader : public core::IChannel
	{
		std::tstring	m_strFilename	;
		FILE*			m_pFile			;
		std::string		m_strBuffer		;
		std::tstring	m_strContext	;

	public:
						CASCIIFileReader(std::tstring strFilename);
						~CASCIIFileReader(void);

		bool			CheckValidity(std::tstring& refStrErrMsg);

	private:
		size_t			OnAccess(void* pData, size_t tDataSize);
	};

}
