#pragma once

namespace core
{
	// Ret - true: continue to traversing, false: abort	
	typedef bool	(*FP_FILETRAVERSE_CALLBACKA)(void* pContext, const ST_FILETRAVERSE_INFOA& stInfo);
	typedef bool	(*FP_FILETRAVERSE_CALLBACKW)(void* pContext, const ST_FILETRAVERSE_INFOW& stInfo);

#ifdef UNICODE
	typedef FP_FILETRAVERSE_CALLBACKW	FP_FILETRAVERSE_CALLBACK	;
#else
	typedef FP_FILETRAVERSE_CALLBACKA	FP_FILETRAVERSE_CALLBACK	;
#endif

	bool CreateDirectoryRecursively(std::string strPath);
	bool CreateDirectoryRecursively(std::wstring strPath);
	bool CreateDirectoryRecursively(const char* pszPath);
	bool CreateDirectoryRecursively(const wchar_t* pszPath);
	bool RemoveDirectoryRecursively(std::string strPath);
	bool RemoveDirectoryRecursively(std::wstring strPath);
	bool RemoveDirectoryRecursively(const char* pszPath);
	bool RemoveDirectoryRecursively(const wchar_t* pszPath);

	ECODE EnumFile(std::string strDir, std::string strPattern, FP_FILETRAVERSE_CALLBACKA fpCallback, void* pCallbackContext);
	ECODE EnumFile(std::wstring strDir, std::wstring strPattern, FP_FILETRAVERSE_CALLBACKW fpCallback, void* pCallbackContext);
	ECODE TraverseDirectory(std::string strDir, std::string strPattern, FP_FILETRAVERSE_CALLBACKA fpCallback, void* pCallbackContext);
	ECODE TraverseDirectory(std::wstring strDir, std::wstring strPattern, FP_FILETRAVERSE_CALLBACKW fpCallback, void* pCallbackContext);

	ECODE EnumFileRecursively(std::string strDir, std::string strPattern, FP_FILETRAVERSE_CALLBACKA fpCallback, void* pCallbackContext);
	ECODE EnumFileRecursively(std::wstring strDir, std::wstring strPattern, FP_FILETRAVERSE_CALLBACKW fpCallback, void* pCallbackContext);
	ECODE TraverseDirectoryRecursively(std::string strDir, std::string strPattern, FP_FILETRAVERSE_CALLBACKA fpCallback, void* pCallbackContext);
	ECODE TraverseDirectoryRecursively(std::wstring strDir, std::wstring strPattern, FP_FILETRAVERSE_CALLBACKW fpCallback, void* pCallbackContext);

	ECODE GrepFiles(std::string strDir, std::string strPattern, std::vector<std::string>& outFileVec);
	ECODE GrepFiles(std::wstring strDir, std::wstring strPattern, std::vector<std::wstring>& outFileVec);
	ECODE GrepFilesRecursively(std::string strDir, std::string strPattern, std::vector<std::string>& outFileVec);
	ECODE GrepFilesRecursively(std::wstring strDir, std::wstring strPattern, std::vector<std::wstring>& outFileVec);

	ECODE GrepDirectories(std::string strDir, std::string strPattern, std::vector<std::string>& outFileVec);
	ECODE GrepDirectories(std::wstring strDir, std::wstring strPattern, std::vector<std::wstring>& outFileVec);
	ECODE GrepDirectoriesRecursively(std::string strDir, std::string strPattern, std::vector<std::string>& outFileVec);
	ECODE GrepDirectoriesRecursively(std::wstring strDir, std::wstring strPattern, std::vector<std::wstring>& outFileVec);

	ECODE CopyDirectory(std::string strDirFrom, std::string strDirTo);
	ECODE CopyDirectory(std::wstring strDirFrom, std::wstring strDirTo);
	ECODE CopyDirectory(const char* pszDirFrom, const char* pszDirTo);
	ECODE CopyDirectory(const wchar_t* pszDirFrom, const wchar_t* pszDirTo);
}

