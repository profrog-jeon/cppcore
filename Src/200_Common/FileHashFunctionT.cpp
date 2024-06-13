#include "stdafx.h"
#include "FileHashFunction.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	std::string BuildHash(E_HASH_TYPE nType, std::tstring strFilePath)
	{
		HANDLE hFile = CreateFile(strFilePath.c_str(), GENERIC_READ_, OPEN_EXISTING_, 0);
		if( NULL == hFile )
			return "";

		HANDLE hHash = InitHash(nType);

		const DWORD dwBuffSize = 1024;
		char szBuff[dwBuffSize];

		DWORD dwReadSize = 0;
		while(ReadFile(hFile, szBuff, dwBuffSize, &dwReadSize) && dwReadSize > 0)
		{
			UpdateHash(hHash, (LPCBYTE)szBuff, dwReadSize);
		}

		FlushFileBuffers(hFile);
		CloseFile(hFile);
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

			FlushFileBuffers(hFile);
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
}