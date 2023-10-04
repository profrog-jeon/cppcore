#pragma once

#include "Struct.h"

#ifdef UNICODE
#define fopenT						fopenW
#define GetSystemDirectory			GetSystemDirectoryW
#define GetCurrentDirectory			GetCurrentDirectoryW
#define SetCurrentDirectory			SetCurrentDirectoryW
#define GetTempPath					GetTempPathW
#define GetFileName					GetFileNameW
#define GetModuleFileName			GetModuleFileNameW
#define PathFileExists				PathFileExistsW
#define IsDirectory					IsDirectoryW
#define CopyFile					CopyFileW
#define MoveFile					MoveFileW
#define DeleteFile					DeleteFileW
#define RemoveDirectory				RemoveDirectoryW
#define CreateDirectory				CreateDirectoryW
#define CreateSymbolicLink			CreateSymbolicLinkW
#define FindFirstFile				FindFirstFileW
#define FindNextFile				FindNextFileW
#else
#define fopenT						fopenA
#define GetSystemDirectory			GetSystemDirectoryA
#define GetCurrentDirectory			GetCurrentDirectoryA
#define SetCurrentDirectory			SetCurrentDirectoryA
#define GetTempPath					GetTempPathA
#define GetFileName					GetFileNameA
#define GetModuleFileName			GetModuleFileNameA
#define PathFileExists				PathFileExistsA
#define IsDirectory					IsDirectoryA
#define CopyFile					CopyFileA
#define MoveFile					MoveFileA
#define DeleteFile					DeleteFileA
#define RemoveDirectory				RemoveDirectoryA
#define CreateDirectory				CreateDirectoryA
#define CreateSymbolicLink			CreateSymbolicLinkA
#define FindFirstFile				FindFirstFileA
#define FindNextFile				FindNextFileA
#endif

namespace core
{
	FILE*			fopenA(const char* pszPath, const char* pszMode);
	FILE*			fopenW(const wchar_t* pszPath, const wchar_t* pszMode);

	std::string		GetSystemDirectoryA(void);		// c:\Windows\System32  or  /
	std::wstring	GetSystemDirectoryW(void);
	std::string		GetTempPathA(void);		// c:\User\Local\Temp  or  /etc/local
	std::wstring	GetTempPathW(void);
	
	bool			PathFileExistsA(std::string strExistFile);
	bool			PathFileExistsW(std::wstring strExistFile);
	bool			IsDirectoryA(const char* pszPath);
	bool			IsDirectoryW(const wchar_t* pszPath);

	bool			CopyFileA(const char* pszExistFile, const char* pszNewFile, BOOL bFailIfExist = FALSE);
	bool			CopyFileW(const wchar_t* pszExistFile, const wchar_t* pszNewFile, BOOL bFailIfExist = FALSE);

	bool			MoveFileA(const char* pszExistFile, const char* pszNewFile);
	bool			MoveFileW(const wchar_t* pszExistFile, const wchar_t* pszNewFile);

	bool			DeleteFileA(const char* pszFileName);
	bool			DeleteFileW(const wchar_t* pszFileName);

	bool			RemoveDirectoryA(const char* pszPath);
	bool			RemoveDirectoryW(const wchar_t* pszPath);

	bool			CreateDirectoryA(const char* pszPath);
	bool			CreateDirectoryW(const wchar_t* pszPath);

	bool			CreateSymbolicLinkA(const char* pszSymlinkFilename, const char* pszTargetFilename);
	bool			CreateSymbolicLinkW(const wchar_t* pszSymlinkFilename, const wchar_t* pszTargetFilename);

	std::string		GetFileNameA(void);			// returns executable file
	std::wstring	GetFileNameW(void);			// returns executable file
	std::string		GetModuleFileNameA(HANDLE hModule);	// returns module file ex) dll or so...
	std::wstring	GetModuleFileNameW(HANDLE hModule);	// returns module file ex) dll or so...

	std::string		GetCurrentDirectoryA(void);
	std::wstring	GetCurrentDirectoryW(void);
	bool			SetCurrentDirectoryA(const char* pszNewPath);
	bool			SetCurrentDirectoryW(const wchar_t* pszNewPath);

	HANDLE			FindFirstFileA(const char* pszFilePattern, ST_FILE_FINDDATAA* pFindData);
	HANDLE			FindFirstFileW(const wchar_t* pszFilePattern, ST_FILE_FINDDATAW* pFindData);
	bool			FindNextFileA(HANDLE hFindFile, ST_FILE_FINDDATAA* pFindData);
	bool			FindNextFileW(HANDLE hFindFile, ST_FILE_FINDDATAW* pFindData);
	void			FindClose(HANDLE hFindFile);
}
