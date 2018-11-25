#pragma once

#include <string>

#include "FmtTypes.h"
#include "IChannel.h"

namespace fmt_internal
{
	class CUTF8StringReader : public core::IChannel
	{
	private:
		size_t m_tReadPos;
		std::tstring m_strContext;

	public:
		CUTF8StringReader(LPCBYTE pContext, size_t tSize);
		CUTF8StringReader(const char* pszContext);
		CUTF8StringReader(std::string& strContext);
		~CUTF8StringReader(void);

		bool CheckValidity(std::tstring& refStrErrMsg);
		size_t OnAccess(void* pData, size_t tDataSize);
	};

}
