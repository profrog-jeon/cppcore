#pragma once

#include <string>

#include "FmtTypes.h"
#include "IChannel.h"

namespace fmt_internal
{
	class CUTF8StringWriter : public core::IChannel
	{
	private:
		std::string& m_strOutput;

	public:
		CUTF8StringWriter(std::string& strOutput);
		~CUTF8StringWriter(void);

		bool CheckValidity(std::tstring& refStrErrMsg);

	private:
		size_t Access(void* pData, size_t tDataSize);
	};
}

