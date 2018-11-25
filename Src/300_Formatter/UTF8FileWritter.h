#pragma once

#include <stdio.h>

#include "../__Common/Type.h"
#include "../__Common/Define.h"
#include "FmtTypes.h"
#include "IChannel.h"

namespace fmt_internal
{
	class CUTF8FileWritter : public core::IChannel
	{
	private:
		std::tstring	m_strFilename;
		FILE*			m_pFile;

	public:
		CUTF8FileWritter(std::tstring strFilename, bool bUseBOM = true);
		~CUTF8FileWritter(void);

		bool			CheckValidity(std::tstring& refStrErrMsg);

	private:
		size_t			OnAccess(void* pData, size_t tDataSize);
	};
}

