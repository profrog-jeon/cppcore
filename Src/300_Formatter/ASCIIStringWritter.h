#pragma once

#include <string>

#include "FmtTypes.h"
#include "IChannel.h"

namespace fmt_internal
{
	class CASCIIStringWriter : public core::IChannel
	{
	private:
		std::string& m_strOutput;

	public:
		CASCIIStringWriter(std::string& strOutput);
		~CASCIIStringWriter(void);

		bool CheckValidity(std::tstring& refStrErrMsg);

	private:
		size_t OnAccess(void* pData, size_t tDataSize);
	};
}

