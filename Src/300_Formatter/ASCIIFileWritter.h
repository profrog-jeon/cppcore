#pragma once

#include <stdio.h>

#include "../__Common/Type.h"
#include "../__Common/Define.h"
#include "FmtTypes.h"
#include "IChannel.h"

namespace fmt_internal
{
	class CASCIIFileWritter : public core::IChannel
	{
	private:
		std::tstring	m_strFilename;
		HANDLE			m_hFile;

	public:
		CASCIIFileWritter(std::tstring strFilename);
		~CASCIIFileWritter(void);

		bool			CheckValidity(std::tstring& refStrErrMsg);

	private:
		size_t			OnAccess(void* pData, size_t tDataSize);
	};
}

