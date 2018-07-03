#pragma once

#include "Struct.h"

#ifdef UNICODE
#define fopenT						fopenW
#define GetSystemDirectory			GetSystemDirectoryW
#define GetCurrentDirectory			GetCurrentDirectoryW
#define GetTempPath					GetTempPathW
#define GetFileName					GetFileNameW
#define GetModuleFileName			GetModuleFileNameW
#else
#define fopenT						fopenA
#define GetSystemDirectory			GetSystemDirectoryA
#define GetCurrentDirectory			GetCurrentDirectoryA
#define GetTempPath					GetTempPathA
#define GetFileName					GetFileNameA
#define GetModuleFileName			GetModuleFileNameA
#endif

namespace core
{
	FILE*			fopenA(const char* pszPath, const char* pszMode);
	FILE*			fopenW(const wchar_t* pszPath, const wchar_t* pszMode);

	std::string		GetSystemDirectoryA(void);		// c:\  or  /
	std::wstring	GetSystemDirectoryW(void);		// c:\  or  /
	std::string		GetTempPathA(void);		// c:\  or  /etc/local
	std::wstring	GetTempPathW(void);		// c:\  or  /

	bool			PathFileExists(const char* pszExistFile);
	bool			PathFileExists(const wchar_t* pszExistFile);
	bool			IsFileExist(const char* pszExistFile);
	bool			IsFileExist(const wchar_t* pszExistFile);
	bool			IsDirectory(const char* pszPath);
	bool			IsDirectory(const wchar_t* pszPath);

	#undef			CopyFile
	bool			CopyFile(const char* pszExistFile, const char* pszNewFile, BOOL bFailIfExist = FALSE);
	bool			CopyFile(const wchar_t* pszExistFile, const wchar_t* pszNewFile, BOOL bFailIfExist = FALSE);

	#undef			MoveFile
	bool			MoveFile(const char* pszExistFile, const char* pszNewFile);
	bool			MoveFile(const wchar_t* pszExistFile, const wchar_t* pszNewFile);

	#undef			DeleteFile
	bool			DeleteFile(const char* pszFileName);
	bool			DeleteFile(const wchar_t* pszFileName);

	#undef			RemoveDirectory
	bool			RemoveDirectory(const char* pszPath);
	bool			RemoveDirectory(const wchar_t* pszPath);

	#undef			CreateDirectory
	bool			CreateDirectory(const char* pszPath);
	bool			CreateDirectory(const wchar_t* pszPath);

	#undef			CreateSymbolicLink
	bool			CreateSymbolicLink(const char* pszSymlinkFilename, const char* pszTargetFilename);
	bool			CreateSymbolicLink(const wchar_t* pszSymlinkFilename, const wchar_t* pszTargetFilename);

	std::string		GetFileNameA(void);			// returns executable file
	std::wstring	GetFileNameW(void);			// returns executable file
	std::string		GetModuleFileNameA(HANDLE hModule);	// returns module file ex) dll or so...
	std::wstring	GetModuleFileNameW(HANDLE hModule);	// returns module file ex) dll or so...

	std::string		GetCurrentDirectoryA(void);
	std::wstring	GetCurrentDirectoryW(void);
	bool			SetCurrentDirectory(const char* pszNewPath);
	bool			SetCurrentDirectory(const wchar_t* pszNewPath);

	HANDLE			FindFirstFile(const char* pszFilePattern, ST_FILE_FINDDATAA* pFindData);
	HANDLE			FindFirstFile(const wchar_t* pszFilePattern, ST_FILE_FINDDATAW* pFindData);
	bool			FindNextFile(HANDLE hFindFile, ST_FILE_FINDDATAA* pFindData);
	bool			FindNextFile(HANDLE hFindFile, ST_FILE_FINDDATAW* pFindData);
	void			FindClose(HANDLE hFindFile);
}

