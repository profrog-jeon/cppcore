#pragma once

#include "Struct.h"

namespace core
{
	class CExeParserSuper
	{
	public:
		CExeParserSuper(void) {}
		virtual ~CExeParserSuper(void) {}

		ECODE Open(std::string strFilePath);
		ECODE Open(std::wstring strFilePath);
		ECODE Open(LPCSTR pszFilePath);
		ECODE Open(LPCWSTR pszFilePath);

		virtual ECODE OpenWorker(HANDLE hFile) = 0;
		virtual void Close(void) = 0;

		virtual ECODE GetFileVersion(ST_VERSIONINFO& outVersionInfo) = 0;
		virtual ECODE GetProductVersion(ST_VERSIONINFO& outVersionInfo) = 0;

		// For lower version compatibility
		virtual ECODE QueryFileVersion(ST_VERSIONINFO& outVersionInfo)
		{
			return GetFileVersion(outVersionInfo);
		}

		virtual ECODE QueryProductVersion(ST_VERSIONINFO& outVersionInfo)
		{
			return GetProductVersion(outVersionInfo);
		}
	};
}
