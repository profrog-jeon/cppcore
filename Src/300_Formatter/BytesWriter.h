#pragma once

#include <string>

#include "FmtTypes.h"
#include "IChannel.h"

namespace fmt_internal
{
	class CBytesWriter : public core::IChannel
	{
	private:
		std::vector<BYTE>& m_vecOutput;

	public:
		CBytesWriter(std::vector<BYTE>& vecOutput);
		~CBytesWriter(void);

		bool CheckValidity(std::tstring& refStrErrMsg);

	private:
		size_t Access(void* pData, size_t tDataSize);
	};
}

