#include "stdafx.h"
#include "FileSystem.h"
#include "Environment.h"
#include "Log.h"
#include "Utility.h"
#undef TEXT
#include "System_Win.h"
#include <Windows.h>
#include <Shlwapi.h>
#include <io.h>
#include <stack>

#pragma comment(lib, "Shlwapi.lib")

#ifdef UNICODE
#define tfopen_s		_wfopen_s
#else
#define tfopen_s		fopen_s
#endif

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	FILE* fopenT(LPCTSTR pszPath, LPCTSTR pszMode)
	{
		FILE* pFile = NULL;
		errno_t tRet = tfopen_s(&pFile, pszPath, pszMode);
		if( 0 != tRet )
		{
			Log_Debug(TEXT("_tfopen_s(%s, %s) failure, %u"), pszPath, pszMode, tRet);
			return NULL;
		}

		return pFile;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GetSystemDirectory(void)
	{
		const size_t tBuffSize = 1024;
		TCHAR szBuff[tBuffSize] = { 0, };
		UINT uRet = ::GetSystemDirectory(szBuff, tBuffSize);
		if( 0 == uRet )
		{
			Log_Debug("GetSystemDirectory calling failure, %u", ::GetLastError());
			return TEXT("C:\\");
		}

		return std::tstring(szBuff);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GetTempPath(void)
	{
		const size_t tBuffSize = 1024;
		TCHAR szBuff[tBuffSize] = { 0, };
		UINT uRet = ::GetTempPath(tBuffSize, szBuff);
		if( 0 == uRet )
		{
			Log_Debug("GetTempPath calling failure, %u", ::GetLastError());
			return TEXT("%TEMP%");
		}

		return std::tstring(szBuff);
	}

	//////////////////////////////////////////////////////////////////////////
	bool PathFileExists(std::tstring strExistFile)
	{
		if (strExistFile.length() < MAX_PATH)
			return ::PathFileExists(strExistFile.c_str()) != FALSE;

		std::tstring strDir = ExtractDirectory(strExistFile);
		std::tstring strFileName = ExtractFileName(strExistFile);
		if (MAX_PATH < strDir.length())
			return false;
		
		if (MAX_PATH < strFileName.length())
			return false;

		CCurrentDirectorySettter CurDir(strDir);
		if (!CurDir.DirChanged())
			return false;

		return ::PathFileExists(strFileName.c_str()) != FALSE;
	}

	//////////////////////////////////////////////////////////////////////////	
	bool IsDirectory(LPCTSTR pszPath)
	{
#ifdef UNICODE
		DWORD dwRet = GetFileAttributesW(pszPath);
#else
		DWORD dwRet = GetFileAttributesA(pszPath);
#endif
		if( INVALID_FILE_ATTRIBUTES == dwRet )
		{
			Log_Debug("GetFileAttributes(%s) returned INVALID_FILE_ATTRIBUTES", pszPath);
			return false;
		}

		if( 0 == (FILE_ATTRIBUTE_DIRECTORY & dwRet) )
			return false;

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool CopyFile(LPCTSTR pszExistFile, LPCTSTR pszNewFile, BOOL bFailIfExist)
	{
		return ::CopyFile(pszExistFile, pszNewFile, bFailIfExist) != FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	bool MoveFile(LPCTSTR pszExistFile, LPCTSTR pszNewFile)
	{
		return ::MoveFile(pszExistFile, pszNewFile) != FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DeleteFile(LPCTSTR pszFileName)
	{
		return ::DeleteFile(pszFileName) != FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	bool RemoveDirectory(LPCTSTR pszPath)
	{
		return ::RemoveDirectory(pszPath) != FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	bool CreateDirectory(LPCTSTR pszPath)
	{
		if( (FALSE == ::CreateDirectory(pszPath, NULL)) && (::GetLastError() != ERROR_ALREADY_EXISTS) )
			return false;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool CreateSymbolicLink(LPCTSTR pszSymlinkFilename, LPCTSTR pszTargetFilename)
	{
		typedef BOOLEAN (APIENTRY *FP_CreateSymbolicLink)(LPCTSTR pszSymlinkFilename, LPCTSTR pszTargetFilename, DWORD dwFlags);
		HMODULE hModule = ::LoadLibrary(TEXT("kernel32.dll"));
		if( NULL == hModule )
			return false;

#ifdef UNICODE
		FP_CreateSymbolicLink fpFunc = (FP_CreateSymbolicLink)::GetProcAddress(hModule, "CreateSymbolicLinkW");
#else
		FP_CreateSymbolicLink fpFunc = (FP_CreateSymbolicLink)::GetProcAddress(hModule, "CreateSymbolicLinkA");
#endif
		if( NULL == fpFunc )
		{
			::FreeLibrary(hModule);
			return false;
		}

		DWORD dwFlag = IsDirectory(pszTargetFilename)? SYMBOLIC_LINK_FLAG_DIRECTORY : 0;
		BOOLEAN bRet = fpFunc(pszSymlinkFilename, pszTargetFilename, dwFlag);
		::FreeLibrary(hModule);
		return bRet != 0;
	}

	//////////////////////////////////////////////////////////////////////////
std::tstring GetFileName(void)
{
	DWORD dwLength = 512;
	std::tstring strRet;

	DWORD dwWrittenLen	= 0;
	do
	{
		dwLength *= 2;
		strRet.resize(dwLength);

		dwWrittenLen = ::GetModuleFileName(NULL, (TCHAR*)strRet.c_str(), dwLength);
		if( 0 == dwWrittenLen )
		{
			Log_Debug("GetModuleFileName calling faliure, %u", ::GetLastError());
			return TEXT("");
		}
	}	while(ERROR_INSUFFICIENT_BUFFER == ::GetLastError());

	strRet.resize(dwWrittenLen);
	return strRet;
}

	//////////////////////////////////////////////////////////////////////////
std::tstring GetModuleFileName(HANDLE hModule)
{
	DWORD dwLength = 512;
	std::tstring strRet;

	DWORD dwWrittenLen	= 0;
	do
	{
		dwLength *= 2;
		strRet.resize(dwLength);

		dwWrittenLen = ::GetModuleFileName((HINSTANCE)hModule, (TCHAR*)strRet.c_str(), dwLength);
		if( 0 == dwWrittenLen )
		{
			Log_Debug("GetModuleFileName faliure, %u", ::GetLastError());
			return TEXT("");
		}
	}	while(ERROR_INSUFFICIENT_BUFFER == ::GetLastError());

	strRet.resize(dwWrittenLen);
	return strRet;
}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GetCurrentDirectory(void)
	{
		TCHAR szBuff[1024];
		DWORD dwRet = ::GetCurrentDirectory(1024, szBuff);
		if( 0 == dwRet )
		{
			Log_Debug("GetCurrentDirectroy failure, %u", ::GetLastError());
			return TEXT("");
		}

		return szBuff;
	}

	//////////////////////////////////////////////////////////////////////////
	bool SetCurrentDirectory(LPCTSTR pszNewPath)
	{
		if( !::SetCurrentDirectory(pszNewPath) )
		{
			Log_Debug("SetCurrentDirectory calling failure, %u", ::GetLastError());
			return false;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	static inline void ConvertFINDDATA(const WIN32_FIND_DATA& stFrom, ST_FILE_FINDDATA* pTo)
	{
		pTo->bIsDirectory = (stFrom.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
		pTo->bIsFile = (stFrom.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
		pTo->bIsLink = (stFrom.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0;
		pTo->strFileName = stFrom.cFileName;
		pTo->uFileSize = ((UINT64)stFrom.nFileSizeHigh << 32) | ((UINT64)stFrom.nFileSizeLow);
		pTo->uCreationTime = UnixTimeFrom(stFrom.ftCreationTime);
		pTo->uLastAccessTime = UnixTimeFrom(stFrom.ftLastAccessTime);
		pTo->uLastWriteTime = UnixTimeFrom(stFrom.ftLastWriteTime);
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE FindFirstFile(LPCTSTR pszFilePattern, ST_FILE_FINDDATA* pFindData)
	{
		WIN32_FIND_DATA stData = { 0, };
		HANDLE hHandle = ::FindFirstFile(pszFilePattern, &stData);
		if( INVALID_HANDLE_VALUE == hHandle )
			return NULL;

		ConvertFINDDATA(stData, pFindData);
		return hHandle;
	}

	//////////////////////////////////////////////////////////////////////////
	bool FindNextFile(HANDLE hFindFile, ST_FILE_FINDDATA* pFindData)
	{
		WIN32_FIND_DATA stData = { 0, };
		if( FALSE == ::FindNextFile(hFindFile, &stData) )
			return false;

		ConvertFINDDATA(stData, pFindData);
		return true;
	}
}
