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
		BeginRoot();
		pObject->OnSync(*this);
		EndRoot();
	}
}