#include "stdafx.h"
#include "ELFParser.h"
#include "Log.h"
#include "FileSystem.h"
#include "KernelObject.h"
#include "Environment.h"

namespace core
{
	static LPCSTR g_pszFileVersionName = "FileVersion";
	static LPCSTR g_pszProductVersionName = "ProductVersion";

	//////////////////////////////////////////////////////////////////////////
	ECODE CELFParser::Parse(LPCTSTR pszFilePath)
	{
		HANDLE hFile = NULL;

		try
		{
			hFile = CreateFile(pszFilePath, GENERIC_READ_, OPEN_ALWAYS_, 0);
			if (NULL == hFile)
				throw exception_format(TEXT("ELF File:%s open failure, %d"), pszFilePath, GetLastError());

			DWORD dwReadSize = 0;
			ST_ELF_IDENTITY stIdentity = { 0, };
			if (!ReadFile(hFile, &stIdentity, sizeof(stIdentity), &dwReadSize))
				throw exception_format(TEXT("ELF header reading failure, read:%u"), dwReadSize);

			if (0x7F != stIdentity.ei_magic[0]
				|| 'E' != stIdentity.ei_magic[1]
				|| 'L' != stIdentity.ei_magic[2]
				|| 'F' != stIdentity.ei_magic[3])
				throw exception_format("ELF magic is not matched");

			ECODE nRet = EC_NO_DATA;
			if (1 == stIdentity.ei_class)
				nRet = Parse32Bit(hFile, stIdentity);
			if (2 == stIdentity.ei_class)
				nRet = Parse64Bit(hFile, stIdentity);
			if (EC_SUCCESS != nRet)
				throw exception_format("ELF parsing failure, %d", nRet);

			ST_VERSIONINFO stVersion;
			if (EC_SUCCESS == QueryVersion(hFile, g_pszFileVersionName, stVersion))
				m_mapVersionInfo.insert(std::make_pair(g_pszFileVersionName, stVersion));

			if (EC_SUCCESS == QueryVersion(hFile, g_pszProductVersionName, stVersion))
				m_mapVersionInfo.insert(std::make_pair(g_pszProductVersionName, stVersion));

			CloseFile(hFile);
		}
		catch (std::exception& e)
		{
			Log_Debug("%s", e.what());
			if (hFile)
				CloseFile(hFile);
			return EC_INTERNAL_ERROR;
		}

		return EC_SUCCESS;
	}
}
