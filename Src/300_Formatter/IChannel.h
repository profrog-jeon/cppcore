#pragma once

#include "../__Common/Type.h"
#include "FmtTypes.h"

namespace core
{
	struct IChannel
	{
		virtual ~IChannel()	{}
		virtual bool CheckValidity(std::tstring& refStrErrMsg) = 0;
		virtual size_t Access(void* pData, size_t tDataSize) = 0;
	};
}
