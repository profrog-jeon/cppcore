#include "stdafx.h"
#include "Information.h"
#ifdef __APPLE__
#include "System_Mac.h"
#else
#include "System_Linux.h"
#endif
#include "FileSystem.h"
#include "Log.h"
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <dlfcn.h>
#include <pwd.h>

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	FILE* fopenT(LPCTSTR pszPath, LPCTSTR pszMode)
	{
		return ::fopen(MBSFromTCS(pszPath).c_str(), MBSFromTCS(pszMode).c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GetSystemDirectory(void)
	{
		return TEXT("/sys/");
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GetTempPath(void)
	{
		const char* pszUserName = "null";
		passwd* pPasswd = getpwuid(geteuid());
		if( pPasswd )
			pszUserName = pPasswd->pw_name;
		std::string strTmpPath = Format("/tmp/%s/", pszUserName);
		if( !PathFileExistsA(strTmpPath.c_str()) )
			CreateDirectoryA(strTmpPath.c_str());
		return TCSFromMBS(strTmpPath);
	}

	//////////////////////////////////////////////////////////////////////////
	bool PathFileExists(std::tstring strExistFile)
	{
		struct stat stStat = { 0, };
		if( ::stat(MBSFromTCS(strExistFile).c_str(), &stStat) < 0 )
			return false;

		return (S_ISDIR(stStat.st_mode) || S_ISREG(stStat.st_mode));
	}

	//////////////////////////////////////////////////////////////////////////
	bool IsDirectory(LPCTSTR pszPath)
	{
		struct stat stStat = { 0, };
		if( ::stat(MBSFromTCS(pszPath).c_str(), &stStat) < 0 )
			return false;

		return S_ISDIR(stStat.st_mode);
	}

	//////////////////////////////////////////////////////////////////////////
	bool CopyFile(LPCTSTR pszExistFile, LPCTSTR pszNewFile, BOOL bFailIfExist)
	{
		std::string strExistFile = MBSFromTCS(pszExistFile);
		std::string strNewFile = MBSFromTCS(pszNewFile);
		FILE* pInFile = NULL;
		FILE* pOutFile = NULL;
		size_t tReadLen = 0;

		try
		{
			if( bFailIfExist && PathFileExists(pszNewFile) )
			{
				Log_Info(TEXT("The destination file(%s) exists already."), pszNewFile);
				return false;
			}

			if (MakeFormalPath(strExistFile) == MakeFormalPath(strNewFile))
			{
				Log_Info("CopyFile Src: %s", MakeFormalPath(strExistFile).c_str());
				Log_Info("CopyFile Dst: %s", MakeFormalPath(strNewFile).c_str());
				Log_Info(TEXT("Source(%s) and dest(%s) is same."), pszExistFile, pszNewFile);
				return false;
			}

			if( (pInFile = fopen(strExistFile.c_str(), "rb")) == NULL )
				throw exception_format(TEXT("Opening exist(%s) failed."), pszExistFile);

			if( (pOutFile = fopen(strNewFile.c_str(), "wb")) == NULL )
				throw exception_format(TEXT("Opening new(%s) failed."), pszNewFile);

			const size_t tBuffSize = 1024;
			char szBuff[tBuffSize];
			while( (tReadLen = fread(szBuff, 1, tBuffSize, pInFile)) > 0 )
			{
				if( 0 == fwrite(szBuff, sizeof(char), tReadLen, pOutFile) )
					throw exception_format("Writing file contents failed.");
			}

			fclose(pInFile);
			fclose(pOutFile);
		}
		catch( std::exception& e )
		{
			Log_Debug("%s", e.what());

			if( pInFile )
				fclose(pInFile);

			if( pOutFile )
				fclose(pOutFile);

			DeleteFile(pszNewFile);
			return false;
		}

		struct stat stStat = { 0, };
		if( ::stat(strExistFile.c_str(), &stStat) < 0 )
			Log_Warn(TEXT("%s mode_t copy has failed."), pszExistFile);
		else if( chmod(strNewFile.c_str(), stStat.st_mode) < 0 )
			Log_Warn(TEXT("%s mode_t copy has failed."), pszNewFile);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool MoveFile(LPCTSTR pszExistFile, LPCTSTR pszNewFile)
	{
		return 0 == ::rename(MBSFromTCS(pszExistFile).c_str(), MBSFromTCS(pszNewFile).c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	bool DeleteFile(LPCTSTR pszFileName)
	{
		if( ::unlink(MBSFromTCS(pszFileName).c_str()) < 0 )
		{
			Log_Debug(TEXT("unlink(%s) failure, %d"), pszFileName, errno);
			return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool CreateDirectory(LPCTSTR pszPath)
	{
		if( ::mkdir(MBSFromTCS(pszPath).c_str(), 0755) < 0 )
		{
			if( errno != EEXIST )
			{
				Log_Debug(TEXT("mkdir(%s) failure, %d"), pszPath, errno);
				return false;
			}

			Log_Debug(TEXT("%s is already exists."), pszPath);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool RemoveDirectory(LPCTSTR pszPath)
	{
		if( ::rmdir(MBSFromTCS(pszPath).c_str()) < 0 )
		{
			Log_Debug(TEXT("rmdir(%s) failure, %d"), pszPath, errno);
			return false;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool CreateSymbolicLink(LPCTSTR pszSymlinkFilename, LPCTSTR pszTargetFilename)
	{
		if( 0 == ::link(MBSFromTCS(pszTargetFilename).c_str(), MBSFromTCS(pszSymlinkFilename).c_str()) )
			return true;
		Log_Debug("link failure, %d(%s)", errno, strerror(errno));
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GetFileName(void)
	{
#ifdef __APPLE__
		Dl_info stModuleInfo;
		if( 0 == ::dladdr((void*)GetFileName, &stModuleInfo) )
			return TEXT("");
		std::tstring strRet = TCSFromMBS(stModuleInfo.dli_fname);
		return MakeFormalPath(strRet);
#else
		std::string strTmp = Format("/proc/%d/exe", ::getpid());
		char szPath[1024] = { 0x00, };
		if( ::readlink(strTmp.c_str(), szPath, 1024) < 0 )
			Log_Debug("readlink(%s) failure, %d", strTmp.c_str(), errno);
		return TCSFromMBS(szPath);
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GetModuleFileName(HANDLE hModule)
	{
		if( NULL == hModule )
			return GetFileName();

		Dl_info stModuleInfo;
		if( 0 == ::dladdr(hModule, &stModuleInfo) )
		{
			Log_Debug("dladdr calling failure, hModule=0x%X", hModule);
			return TEXT("");
		}

		return TCSFromMBS(stModuleInfo.dli_fname);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GetCurrentDirectory(void)
	{
		char* pszCWD = ::getcwd(NULL, 0);
		if( NULL == pszCWD )
		{
			Log_Debug("getcwd failure, %d", errno);
			return TEXT("");
		}

		std::tstring strRet = TCSFromMBS(pszCWD);
		free(pszCWD);
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	bool SetCurrentDirectory(LPCTSTR pszNewPath)
	{
		std::string strNewPathA = MBSFromTCS(pszNewPath);
		if( ::chdir(strNewPathA.c_str()) < 0 )
		{
			Log_Debug("chdir(%s) failure, %s", strNewPathA.c_str(), strerror(errno));
			return false;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	static inline bool FindFileWorkder(DIR* pDir, std::string strRootPath, std::string strPattern, ST_FILE_FINDDATA* pFindData)
	{
		struct dirent* pDirEntry = NULL;
		while( (pDirEntry = ::readdir(pDir)) != NULL )
		{
			std::string strFileName = pDirEntry->d_name;
			std::string strFilePath = strRootPath + "/" + strFileName;
			if( !SafeStrCmpWithWildcard(strFileName.c_str(), strFileName.length(), strPattern.c_str()) )
				continue;

			struct stat stStat = { 0, };
			if( ::stat(strFilePath.c_str(), &stStat) < 0 )
			{
				Log_Debug("::stat(%s) has failed, %s", strFilePath.c_str(), strerror(errno));
				continue;
			}

			if( S_ISLNK(stStat.st_mode) )
			{
				if( ::lstat(strFilePath.c_str(), &stStat) < 0 )
					Log_Debug("::lstat(%s) has failed, %s", strFilePath.c_str(), strerror(errno));
			}

			pFindData->bIsDirectory = S_ISDIR(stStat.st_mode);
			pFindData->bIsFile = S_ISREG(stStat.st_mode);
			pFindData->bIsLink = S_ISLNK(stStat.st_mode);
			pFindData->strFileName = TCSFromMBS(strFileName);
			pFindData->uFileSize = stStat.st_size;
			pFindData->uCreationTime = stStat.st_ctime;
			pFindData->uLastAccessTime = stStat.st_atime;
			pFindData->uLastWriteTime = stStat.st_mtime;
			return true;
		}

		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE FindFirstFile(LPCTSTR pszFilePattern, ST_FILE_FINDDATA* pFindData)
	{
		std::string strCurPath = ExtractDirectory(MBSFromTCS(pszFilePattern));
		std::string strPattern = ExtractFileName(MBSFromTCS(pszFilePattern));

		DIR* pDir = ::opendir(strCurPath.c_str());
		if( NULL == pDir )
		{
			Log_Debug("opendir(%s) operation failure, %s", strCurPath.c_str(), strerror(errno));
			return NULL;
		}

		if( !FindFileWorkder(pDir, strCurPath, strPattern, pFindData) )
		{
			::closedir(pDir);
			return NULL;
		}

		_ST_FINDFIRSTFILE* pHandle = new _ST_FINDFIRSTFILE();
		pHandle->pDir = pDir;
		pHandle->strRootPath = strCurPath;
		pHandle->strPattern = strPattern;
		return pHandle;
	}

	//////////////////////////////////////////////////////////////////////////
	bool FindNextFile(HANDLE hFindFile, ST_FILE_FINDDATA* pFindData)
	{
		_ST_FINDFIRSTFILE* pHandle = (_ST_FINDFIRSTFILE*)hFindFile;
		return FindFileWorkder(pHandle->pDir, pHandle->strRootPath, pHandle->strPattern, pFindData);
	}
}
