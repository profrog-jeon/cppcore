#include "stdafx.h"
#include "Interface.h"
#include "300_Formatter.h"
#include "FmtTypes.h"

namespace core
{
	IFormatter::IFormatter(core::IChannel& channel)
		: m_Channel(channel)
	{
	}

	IFormatter::~IFormatter()
	{
	}

	void IFormatterObject::Synchronize(IFormatterT& formatter)
	{
		formatter._BeginRootGrouping();
		OnSync(formatter);
		formatter._EndRootGrouping();
	}
}

