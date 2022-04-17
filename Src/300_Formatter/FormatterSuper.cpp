#include "stdafx.h"
#include "FormatterSuper.h"

namespace core
{
	CFormatterSuper::CFormatterSuper(core::IChannel& channel)
		: m_Channel(channel)
	{
	}

	CFormatterSuper::~CFormatterSuper()
	{
	}

	void CFormatterSuper::Synchronize(IFormatterObject* pObject)
	{
		BeginRootGrouping();
		pObject->OnSync(*this);
		EndRootGrouping();
	}
}