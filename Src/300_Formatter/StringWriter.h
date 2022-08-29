#pragma once

#include <string>

#include "FmtTypes.h"
#include "IChannel.h"

namespace fmt_internal
{
	class CStringWriter : public core::IChannel
	{
	private:
		std::tstring& m_strOutput;

	public:
		CStringWriter(std::tstring& strOutput);
		~CStringWriter(void);

		bool CheckValidity(std::tstring& refStrErrMsg);

	private:
		size_t Access(void* pData, size_t tDataSize);
	};
}

