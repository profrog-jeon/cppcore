#include "stdafx.h"
#include "HashFunction.h"
#include "Hash.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	static inline std::string BuildStringHashWorker(E_HASH_TYPE nType, std::tstring strContext)
	{
		HANDLE hHash = NULL;

		try
		{
			hHash = InitHash(nType);
			if( NULL == hHash )
				throw exception_format("InitHash(%d) has failed.", nType);

			std::string strContextA = UTF8FromTCS(strContext);
			UpdateHash(hHash, (LPCBYTE)strContextA.c_str(), strContextA.length());
			return FinalHash(hHash);
		}
		catch (std::exception& e)
		{
			printf("%s\n", e.what());
			if( hHash )
				FinalHash(hHash);
			return "";
		}
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring BuildStringHash(E_HASH_TYPE nType, std::tstring strContext)
	{
		return TCSFromMBS(BuildStringHashWorker(nType, strContext));
	}

	//////////////////////////////////////////////////////////////////////////
	std::string BuildStringHashA(E_HASH_TYPE nType, std::tstring strContext)
	{
		return BuildStringHashWorker(nType, strContext);
	}

	//////////////////////////////////////////////////////////////////////////
	std::wstring BuildStringHashW(E_HASH_TYPE nType, std::tstring strContext)
	{
		return WCSFromMBS(BuildStringHashWorker(nType, strContext));
	}

	//////////////////////////////////////////////////////////////////////////
	bool BuildStringHash(E_HASH_TYPE nType, std::tstring strContext, std::vector<BYTE>& vecHash)
	{
		HANDLE hHash = InitHash(nType);
		if( NULL == hHash )
			return false;

		std::string strContextA = UTF8FromTCS(strContext);
		UpdateHash(hHash, (LPCBYTE)strContextA.c_str(), strContextA.length());
		FinalHash(hHash, vecHash);
		return true;
	}
}