#pragma once

#include <string>

#include "FmtTypes.h"
#include "IChannel.h"

namespace fmt_internal
{
	class CANSIStringWriter : public core::IChannel
	{
	private:
		std::string& m_strOutput;

	public:
		CANSIStringWriter(std::string& strOutput);
		~CANSIStringWriter(void);

		bool CheckValidity(std::tstring& refStrErrMsg);

	private:
		size_t Access(void* pData, size_t tDataSize);
	};
}

