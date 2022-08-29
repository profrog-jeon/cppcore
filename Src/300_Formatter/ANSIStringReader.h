#pragma once

#include <string>

#include "FmtTypes.h"
#include "IChannel.h"

namespace fmt_internal
{
	class CANSIStringReader : public core::IChannel
	{
	private:
		size_t m_tReadPos;
		std::tstring m_strContext;

	public:
		CANSIStringReader(LPCBYTE pContext, size_t tSize);
		CANSIStringReader(const char* pszContext);
		CANSIStringReader(std::string& strContext);
		~CANSIStringReader(void);

		bool CheckValidity(std::tstring& refStrErrMsg);
		size_t Access(void* pData, size_t tDataSize);
	};

}
