#include "stdafx.h"
#include "FormatterSuper.h"

namespace fmt_internal
{
	using namespace core;

	CFormatterSuper::CFormatterSuper(core::IChannel& channel)
		: m_Channel(channel)
	{
		
	}

	CFormatterSuper::~CFormatterSuper()
	{
	}

	bool CFormatterSuper::Transform(const IFormatterObject* pObject, std::tstring* pOutErrMsg, std::tstring* pRootName)
	{
		return Transform((IFormatterObject*)pObject, pOutErrMsg, pRootName);
	}

	bool CFormatterSuper::Transform(IFormatterObject* pObject, std::tstring* pOutErrMsg, std::tstring* pRootName)
	{
		std::tstring strErrMsg;
		if (!OnPrepare(pObject, strErrMsg))
		{
			if (pOutErrMsg)
				(*pOutErrMsg) = std::move(strErrMsg);
			return false;
		}

		std::tstring strRootName;
		if (pRootName)
			strRootName = (*pRootName);

		OnBeginRoot(strRootName);
		pObject->OnSync(*this);
		OnEndRoot();

		if (pRootName)
			(*pRootName) = std::move(strRootName);
		return true;
	}

	IFormatter& CFormatterSuper::OnSync(std::tstring& strKey, IFormatterObject* pObject)
	{
		OnBeginObject(strKey);
		pObject->OnSync(*this);
		OnEndObject();
		return *this;
	}

	IFormatter& CFormatterSuper::OnSync(std::tstring& strKey, IFormatterObject** pObject)
	{
		OnBeginObject(strKey);
		(*pObject)->OnSync(*this);
		OnEndObject();
		return *this;
	}
}

