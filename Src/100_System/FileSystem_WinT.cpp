#include "stdafx.h"
#include "FileSystem.h"
#include "Environment.h"
#include "Log.h"
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
			Log_Error(TEXT("_tfopen_s(%s, %s) failure, %u"), pszPath, pszMode, tRet);
			return NULL;
		}

		return pFile;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GetSystemDirectory(void)		// c:\  or  /
	{
		const size_t tBuffSize = 1024;
		TCHAR szBuff[tBuffSize] = { 0, };
		UINT uRet = ::GetSystemDirectory(szBuff, tBuffSize);
		if( 0 == uRet )
		{
			Log_Error("GetSystemDirectory calling failure, %u", ::GetLastError());
			return TEXT("C:\\");
		}

		std::tstring strRet(szBuff);
		return strRet.substr(0, 3);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GetTempPath(void)
	{
		const size_t tBuffSize = 1024;
		TCHAR szBuff[tBuffSize] = { 0, };
		UINT uRet = ::GetTempPath(tBuffSize, szBuff);
		if( 0 == uRet )
		{
			Log_Error("GetTempPath calling failure, %u", ::GetLastError());
			return TEXT("%TEMP%");
		}

		return std::tstring(szBuff);
	}

	//////////////////////////////////////////////////////////////////////////
	#undef PathFileExists
	bool PathFileExists(LPCTSTR pszExistFile)
	{
#ifdef UNICODE
		return ::PathFileExistsW(pszExistFile) != FALSE;
#else
		return ::PathFileExistsA(pszExistFile) != FALSE;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	bool IsFileExist(LPCTSTR pszExistFile)
	{
#ifdef UNICODE
		return 0 == ::_waccess(pszExistFile, 0);
#else
		return 0 == ::_access(pszExistFile, 0);
#endif
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
			Log_Error("GetFileAttributes(%s) returned INVALID_FILE_ATTRIBUTES", pszPath);
			return false;
		}

		if( 0 == (FILE_ATTRIBUTE_DIRECTORY & dwRet) )
			return false;

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	#undef CopyFile
	bool CopyFile(LPCTSTR pszExistFile, LPCTSTR pszNewFile, BOOL bFailIfExist)
	{
#ifdef UNICODE
		return ::CopyFileW(pszExistFile, pszNewFile, bFailIfExist) != FALSE;
#else
		return ::CopyFileA(pszExistFile, pszNewFile, bFailIfExist) != FALSE;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	#undef MoveFile
	bool MoveFile(LPCTSTR pszExistFile, LPCTSTR pszNewFile)
	{
#ifdef UNICODE
		return ::MoveFileW(pszExistFile, pszNewFile) != FALSE;
#else
		return ::MoveFileA(pszExistFile, pszNewFile) != FALSE;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	#undef DeleteFile
	bool DeleteFile(LPCTSTR pszFileName)
	{
#ifdef UNICODE
		return ::DeleteFileW(pszFileName) != FALSE;
#else
		return ::DeleteFileA(pszFileName) != FALSE;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	#undef RemoveDirectory
	bool RemoveDirectory(LPCTSTR pszPath)
	{
#ifdef UNICODE
		return ::RemoveDirectoryW(pszPath) != FALSE;
#else
		return ::RemoveDirectoryA(pszPath) != FALSE;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	#undef CreateDirectory
	bool CreateDirectory(LPCTSTR pszPath)
	{
#ifdef UNICODE
		if( (FALSE == ::CreateDirectoryW(pszPath, NULL)) && (::GetLastError() != ERROR_ALREADY_EXISTS) )
#else
		if( (FALSE == ::CreateDirectoryA(pszPath, NULL)) && (::GetLastError() != ERROR_ALREADY_EXISTS) )
#endif	
			return false;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	#undef CreateSymbolicLink
	bool CreateSymbolicLink(LPCTSTR pszSymlinkFilename, LPCTSTR pszTargetFilename)
	{
		DWORD dwFlag = IsDirectory(pszTargetFilename)? SYMBOLIC_LINK_FLAG_DIRECTORY : 0;
#ifdef UNICODE
		if( FALSE == ::CreateSymbolicLinkW(pszSymlinkFilename, pszTargetFilename, dwFlag) )
#else
		if( FALSE == ::CreateSymbolicLinkA(pszSymlinkFilename, pszTargetFilename, dwFlag) )
#endif
			return false;
		return true;
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
			Log_Error("GetModuleFileName calling faliure, %u", ::GetLastError());
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
			Log_Error("GetModuleFileName faliure, %u", ::GetLastError());
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
			Log_Error("GetCurrentDirectroy failure, %u", ::GetLastError());
			return TEXT("");
		}

		return szBuff;
	}

	//////////////////////////////////////////////////////////////////////////
	#undef SetCurrentDirectory
	bool SetCurrentDirectory(LPCTSTR pszNewPath)
	{
#ifdef UNICODE
		if( !::SetCurrentDirectoryW(pszNewPath) )
#else
		if( !::SetCurrentDirectoryA(pszNewPath) )
#endif
		{
			Log_Error("SetCurrentDirectory calling failure, %u", ::GetLastError());
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
	#undef FindFirstFile
	HANDLE FindFirstFile(LPCTSTR pszFilePattern, ST_FILE_FINDDATA* pFindData)
	{
#ifdef UNICODE
		WIN32_FIND_DATAW stData = { 0, };
		HANDLE hHandle = ::FindFirstFileW(pszFilePattern, &stData);
#else
		WIN32_FIND_DATAA stData = { 0, };
		HANDLE hHandle = ::FindFirstFileA(pszFilePattern, &stData);
#endif
		if( INVALID_HANDLE_VALUE == hHandle )
			return NULL;

		ConvertFINDDATA(stData, pFindData);
		return hHandle;
	}

	//////////////////////////////////////////////////////////////////////////
	#undef FindNextFile
	bool FindNextFile(HANDLE hFindFile, ST_FILE_FINDDATA* pFindData)
	{
#ifdef UNICODE
		WIN32_FIND_DATAW stData = { 0, };
		if( FALSE == ::FindNextFileW(hFindFile, &stData) )
			return false;
#else
		WIN32_FIND_DATAA stData = { 0, };
		if( FALSE == ::FindNextFileA(hFindFile, &stData) )
			return false;
#endif

		ConvertFINDDATA(stData, pFindData);
		return true;
	}
}
