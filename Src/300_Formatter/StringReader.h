#pragma once

#include <string>

#include "FmtTypes.h"
#include "IChannel.h"

namespace fmt_internal
{
	class CStringReader : public core::IChannel
	{
	private:
		size_t m_tReadPos;
		std::tstring m_strContext;

	public:
		CStringReader(LPCBYTE pContext, size_t tSize);
		CStringReader(const TCHAR* pszContext);
		CStringReader(std::tstring& strContext);
		~CStringReader(void);

		bool CheckValidity(std::tstring& refStrErrMsg);
		size_t Access(void* pData, size_t tDataSize);
	};

}
