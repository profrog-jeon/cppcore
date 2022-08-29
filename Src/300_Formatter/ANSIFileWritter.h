#pragma once

#include <stdio.h>

#include "../__Common/Type.h"
#include "../__Common/Define.h"
#include "FmtTypes.h"
#include "IChannel.h"

namespace fmt_internal
{
	class CANSIFileWritter : public core::IChannel
	{
	private:
		std::tstring	m_strFilename;
		HANDLE			m_hFile;

	public:
		CANSIFileWritter(std::tstring strFilename);
		~CANSIFileWritter(void);

		bool			CheckValidity(std::tstring& refStrErrMsg);

	private:
		size_t			Access(void* pData, size_t tDataSize);
	};
}

