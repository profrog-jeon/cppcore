#pragma once

#include "FmtTypes.h"
#include "IChannel.h"

namespace fmt_internal
{
	class CBytesReader : public core::IChannel
	{
	private:
		const BYTE* m_pBytes;
		size_t			m_tBytesSize;
		size_t			m_tReadPos;

	public:
						CBytesReader(const void* pBytes, size_t tBytesSize);
						~CBytesReader(void);

		bool			CheckValidity(std::tstring& refStrErrMsg);

	private:
		size_t			Access(void* pData, size_t tDataSize);
	};

}
