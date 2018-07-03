#include "stdafx.h"
#include "HashFunction.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	std::string HashFile(E_HASH_TYPE nType, std::tstring strFilePath)
	{
		FILE* pFile = fopenT(strFilePath.c_str(), TEXT("rb"));
		if( NULL == pFile )
			return "";

		HANDLE hHash = InitHash(nType);

		while(!::feof(pFile))
		{
			const size_t tBuffSize = 1024;
			char szBuff[tBuffSize];
			size_t tRead = fread(szBuff, 1, tBuffSize, pFile);
			if( tRead )
				UpdateHash(hHash, (LPCBYTE)szBuff, tRead);
		}

		fclose(pFile);
		return FinalHash(hHash);
	}

	//////////////////////////////////////////////////////////////////////////
	static inline HANDLE BuildFileHashWorker(E_HASH_TYPE nType, std::tstring strFilePath)
	{
		HANDLE hFile = NULL;
		HANDLE hHash = NULL;
		
		try
		{
			hFile = CreateFile(strFilePath.c_str(), GENERIC_READ_, OPEN_EXISTING_, 0);
			if( NULL == hFile )
				throw exception_format(TEXT("File(%s) open faliure."), strFilePath.c_str());

			hHash = InitHash(nType);
			if( NULL == hHash )
				throw exception_format("InitHash(%d) has failed.", nType);

			const size_t tBuffSize = 1024;
			char szBuff[tBuffSize];

			DWORD dwReadSize = 0;
			while(ReadFile(hFile, szBuff ,tBuffSize, &dwReadSize) && dwReadSize)
				UpdateHash(hHash, (LPCBYTE)szBuff, dwReadSize);

			CloseFile(hFile);
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			if( hFile )
				CloseFile(hFile);
			if( hHash )
				FinalHash(hHash);
			return NULL;
		}
		
		return hHash;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring BuildFileHash(E_HASH_TYPE nType, std::tstring strFilePath)
	{
		HANDLE hHash = BuildFileHashWorker(nType, strFilePath);
		if( NULL == hHash )
			return TEXT("");
		return TCSFromMBS(FinalHash(hHash));
	}

	//////////////////////////////////////////////////////////////////////////
	std::string BuildFileHashA(E_HASH_TYPE nType, std::tstring strFilePath)
	{
		HANDLE hHash = BuildFileHashWorker(nType, strFilePath);
		if( NULL == hHash )
			return "";
		return FinalHash(hHash);
	}

	//////////////////////////////////////////////////////////////////////////
	std::wstring BuildFileHashW(E_HASH_TYPE nType, std::tstring strFilePath)
	{
		HANDLE hHash = BuildFileHashWorker(nType, strFilePath);
		if( NULL == hHash )
			return L"";
		return WCSFromMBS(FinalHash(hHash));
	}

	//////////////////////////////////////////////////////////////////////////
	bool BuildFileHash(E_HASH_TYPE nType, std::tstring strFilePath, std::vector<BYTE>& vecHash)
	{
		HANDLE hHash = BuildFileHashWorker(nType, strFilePath);
		if( NULL == hHash )
			return false;

		FinalHash(hHash, vecHash);
		return true;
	}

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
			Log_Error("%s", e.what());
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