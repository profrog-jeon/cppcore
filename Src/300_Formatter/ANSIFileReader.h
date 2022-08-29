#pragma once

#include <stdio.h>

#include "../__Common/Type.h"
#include "../__Common/Define.h"
#include "../000_String/BOM.h"
#include "FmtTypes.h"
#include "IChannel.h"

namespace fmt_internal
{
	class CANSIFileReader : public core::IChannel
	{
		std::tstring	m_strFilename	;
		HANDLE			m_hFile			;
		std::string		m_strBuffer		;
		std::tstring	m_strContext	;

	public:
						CANSIFileReader(std::tstring strFilename);
						~CANSIFileReader(void);

		bool			CheckValidity(std::tstring& refStrErrMsg);

	private:
		size_t			Access(void* pData, size_t tDataSize);
	};

}
