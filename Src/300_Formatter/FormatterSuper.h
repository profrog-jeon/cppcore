#pragma once

#include "IChannel.h"
#include "IFormatterA.h"
#include "IFormatterW.h"

namespace fmt_internal
{
	class CFormatterSuper : public core::IFormatter
	{
	protected:
		core::IChannel& m_Channel;

	public:
		CFormatterSuper(core::IChannel& channel);
		virtual ~CFormatterSuper();

		bool Transform(const core::IFormatterObject* pObject, std::tstring* pOutErrMsg = NULL, std::tstring* pRootName = NULL);
		bool Transform(core::IFormatterObject* pObject, std::tstring* pOutErrMsg = NULL, std::tstring* pRootName = NULL);

	protected:
		virtual bool OnPrepare(core::IFormatterObject* pObject, std::tstring& strErrMsg) = 0;

		virtual void OnBeginRoot(std::tstring& strRootName) = 0;
		virtual void OnEndRoot() = 0;

		core::IFormatter& OnSync(std::tstring& strKey, core::IFormatterObject* pObject);
		core::IFormatter& OnSync(std::tstring& strKey, core::IFormatterObject** pObject);
	};
}
