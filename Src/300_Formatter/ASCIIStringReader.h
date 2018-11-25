#pragma once

#include <string>

#include "FmtTypes.h"
#include "IChannel.h"

namespace fmt_internal
{
	class CASCIIStringReader : public core::IChannel
	{
	private:
		size_t m_tReadPos;
		std::tstring m_strContext;

	public:
		CASCIIStringReader(LPCBYTE pContext, size_t tSize);
		CASCIIStringReader(const char* pszContext);
		CASCIIStringReader(std::string& strContext);
		~CASCIIStringReader(void);

		bool CheckValidity(std::tstring& refStrErrMsg);
		size_t OnAccess(void* pData, size_t tDataSize);
	};

}
