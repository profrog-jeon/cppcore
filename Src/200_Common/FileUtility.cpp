#include "stdafx.h"
#include "FileUtility.h"
#include <map>
#include <vector>
#include <stack>

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	static inline bool CreateDirectoryRecursivelyWorker(const TCHAR* pszPath)
	{
		std::vector<std::tstring> vecPath;

		std::tstring strPath(pszPath);
		std::tstring strCurPath;
		int nOffset = 0;
		while(nOffset >= 0)
		{
			std::tstring strToken = Tokenize(strPath, TEXT("/\\"), nOffset);
			strCurPath += strToken + TEXT("/");

			// Except windows root directory
			if( std::tstring::npos != strToken.find(TEXT(':')) )
				continue;

			// Except Non-Windows root directory
			if( TEXT("/") == strCurPath )
				continue;

			vecPath.push_back(strCurPath);
		}

		bool bRet = false;
		size_t i;
		for(i=0; i<vecPath.size(); i++)
			bRet = CreateDirectory(vecPath[i].c_str());

		return bRet;
	}

	//////////////////////////////////////////////////////////////////////////
	bool CreateDirectoryRecursively(const TCHAR* pszPath)
	{
		return CreateDirectoryRecursivelyWorker(pszPath);
	}

	//////////////////////////////////////////////////////////////////////////
	bool CreateDirectoryRecursively(std::tstring strPath)
	{
		return CreateDirectoryRecursivelyWorker(strPath.c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	static bool DeleteFileWorkerCallback(void* pContext, const ST_FILETRAVERSE_INFO& stInfo)
	{
		return DeleteFile(stInfo.pszFilePath);
	}

	//////////////////////////////////////////////////////////////////////////
	static bool RemoveDirectoryWorkerCallback(void* pContext, const ST_FILETRAVERSE_INFO& stInfo)
	{
		std::multimap<size_t, std::tstring>& refContainer = *(std::multimap<size_t, std::tstring>*)pContext;
		std::tstring strDirectory(stInfo.pszFilePath);
		refContainer.insert(std::make_pair(strDirectory.length(), strDirectory));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	static inline bool RemoveDirectoryRecursivelyWorker(const TCHAR* pszPath)
	{
		ECODE nRet = EC_SUCCESS;

		try
		{
			nRet = EnumFileRecursively(pszPath, TEXT("*"), DeleteFileWorkerCallback, NULL);
			if( EC_SUCCESS != nRet )
				throw exception_format("EnumFileRecursively failure, %d", nRet);

			std::multimap<size_t, std::tstring> mapDirectory;
			nRet = TraverseDirectoryRecursively(pszPath, TEXT("*"), RemoveDirectoryWorkerCallback, &mapDirectory);
			if( EC_SUCCESS != nRet )
				throw exception_format("TraverseDirectory failure, %u", nRet);

			std::multimap<size_t, std::tstring>::reverse_iterator iter;
			for(iter=mapDirectory.rbegin(); iter!=mapDirectory.rend(); iter++)
			{
				if( !RemoveDirectory(iter->second.c_str()) )
					throw exception_format(TEXT("RemoveDirectory(%s) failure"), iter->second.c_str());
			}
		}
		catch (std::exception& e)
		{
			Log_Error("%s - %s", __FUNCTION__, e.what());
			return false;
		}
		
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool RemoveDirectoryRecursively(const TCHAR* pszPath)
	{
		return RemoveDirectoryRecursivelyWorker(pszPath);
	}

	//////////////////////////////////////////////////////////////////////////
	bool RemoveDirectoryRecursively(std::tstring strPath)
	{
		return RemoveDirectoryRecursivelyWorker(strPath.c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	bool RemoveDirectoryForcely(std::tstring strPath)
	{
		std::vector<std::tstring> vecFiles;
		GrepFilesRecursively(strPath, TEXT("*"), vecFiles);

		for (size_t i = 0; i < vecFiles.size(); i++)
			DeleteFile(vecFiles[i].c_str());

		return RemoveDirectoryRecursively(strPath);
	}

	//////////////////////////////////////////////////////////////////////////
#ifdef UNICODE
#define __internal_ST_TRAVERSE_FILE_DATA __internal_ST_TRAVERSE_FILE_DATAW
#else
#define __internal_ST_TRAVERSE_FILE_DATA __internal_ST_TRAVERSE_FILE_DATAA
#endif

	struct __internal_ST_TRAVERSE_FILE_DATA
	{
		BOOL bDirSearched;
		std::tstring strPath;

		__internal_ST_TRAVERSE_FILE_DATA(std::tstring path) : bDirSearched(FALSE), strPath(path)	{}
		__internal_ST_TRAVERSE_FILE_DATA& operator=(const __internal_ST_TRAVERSE_FILE_DATA& other)
		{
			this->bDirSearched = other.bDirSearched;
			this->strPath = other.strPath;
			return *this;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	static inline ECODE FindFileWorker(bool bOnlyDirectory, std::tstring strDir, std::tstring strPattern, FP_FILETRAVERSE_CALLBACK fpCallback, void* pCallbackContext)
	{
		const std::tstring strFilePattern = strDir + TEXT("/") + strPattern;

		ST_FILE_FINDDATA stFindFileData	= { 0, };
		HANDLE hFindFile = FindFirstFile(strFilePattern.c_str(), &stFindFileData);
		if( NULL == hFindFile )
			return GetLastError();

		ECODE nRet = EC_SUCCESS;
		do
		{
			if( !bOnlyDirectory && stFindFileData.bIsDirectory
			||  bOnlyDirectory && !stFindFileData.bIsDirectory )
				continue;

			if( std::tstring(TEXT(".")) == stFindFileData.strFileName )
				continue;

			if( std::tstring(TEXT("..")) == stFindFileData.strFileName )
				continue;

			if( stFindFileData.bIsLink )
				continue;

			std::tstring strFilePath = strDir + TEXT("/") + stFindFileData.strFileName;
			ST_FILETRAVERSE_INFO stFileInfo = { 0, };
			stFileInfo.bDirectory = bOnlyDirectory;
			stFileInfo.pszFilePath = strFilePath.c_str();

			if( !(*fpCallback)(pCallbackContext, stFileInfo) )
			{
				nRet = EC_CANCELED;
				break;
			}
		}	while(FindNextFile(hFindFile, &stFindFileData));
		FindClose(hFindFile);

		return nRet;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE EnumFile(std::tstring strDir, std::tstring strPattern, FP_FILETRAVERSE_CALLBACK fpCallback, void* pCallbackContext)
	{
		return FindFileWorker(false, strDir, strPattern, fpCallback, pCallbackContext);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE TraverseDirectory(std::tstring strDir, std::tstring strPattern, FP_FILETRAVERSE_CALLBACK fpCallback, void* pCallbackContext)
	{
		return FindFileWorker(true, strDir, strPattern, fpCallback, pCallbackContext);
	}

	//////////////////////////////////////////////////////////////////////////
	struct ST_ENUMFILE_RECURSIVE_DATA
	{
		LPCTSTR pszPattern;
		FP_FILETRAVERSE_CALLBACK fpCallback;
		void* pCallbackContext;
	};

	//////////////////////////////////////////////////////////////////////////
	static bool EnumFileRecursivelyCallback(void* pContext, const ST_FILETRAVERSE_INFO& stInfo)
	{
		ST_ENUMFILE_RECURSIVE_DATA* pData = (ST_ENUMFILE_RECURSIVE_DATA*)pContext;
		return EC_CANCELED != FindFileWorker(false, stInfo.pszFilePath, pData->pszPattern, pData->fpCallback, pData->pCallbackContext);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE EnumFileRecursively(const std::tstring strDir, const std::tstring strPattern, FP_FILETRAVERSE_CALLBACK fpCallback, void* pCallbackContext)
	{
		ST_ENUMFILE_RECURSIVE_DATA stData;
		stData.pszPattern = strPattern.c_str();
		stData.fpCallback = fpCallback;
		stData.pCallbackContext	= pCallbackContext;
		return TraverseDirectoryRecursively(strDir, TEXT("*"), EnumFileRecursivelyCallback, &stData);
	}

	//////////////////////////////////////////////////////////////////////////
	struct ST_TRAVERSEDIRECTORY_RECURSIVE_DATA
	{
		FP_FILETRAVERSE_CALLBACK fpCallback;
		void* pCallbackContext;
		std::stack<std::tstring>* pLoopStack;
	};

	//////////////////////////////////////////////////////////////////////////
	static bool TraverseDirectoryRecursivelyCallback(void* pContext, const ST_FILETRAVERSE_INFO& stInfo)
	{
		ST_TRAVERSEDIRECTORY_RECURSIVE_DATA* pData = (ST_TRAVERSEDIRECTORY_RECURSIVE_DATA*)pContext;
		pData->pLoopStack->push(stInfo.pszFilePath);
		return pData->fpCallback(pData->pCallbackContext, stInfo);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE TraverseDirectoryRecursively(std::tstring strDir, std::tstring strPattern, FP_FILETRAVERSE_CALLBACK fpCallback, void* pCallbackContext)
	{
		std::stack<std::tstring> stkTraverse;
		stkTraverse.push(strDir);
		{
			ST_FILETRAVERSE_INFO stInfo;
			stInfo.bDirectory = true;
			stInfo.pszFilePath = strDir.c_str();
			if( !fpCallback(pCallbackContext, stInfo) )
				return EC_CANCELED;
		}

		ST_TRAVERSEDIRECTORY_RECURSIVE_DATA stData;
		stData.fpCallback		= fpCallback;
		stData.pCallbackContext	= pCallbackContext;
		stData.pLoopStack		= &stkTraverse;

		while(!stkTraverse.empty())
		{
			const std::tstring strCurPath = stkTraverse.top();
			stkTraverse.pop();
			ECODE nRet = FindFileWorker(true, strCurPath.c_str(), strPattern, TraverseDirectoryRecursivelyCallback, &stData);
			if( EC_CANCELED == nRet )
				return EC_CANCELED;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	static bool GrepFilesCallback(void* pContext, const ST_FILETRAVERSE_INFO& stInfo)
	{
		std::vector<std::tstring>& vecFiles = *(std::vector<std::tstring>*)pContext;
		vecFiles.push_back(stInfo.pszFilePath);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////	
	ECODE GrepFiles(std::tstring strDir, std::tstring strPattern, std::vector<std::tstring>& outFileVec)
	{
		return FindFileWorker(false, strDir, strPattern, GrepFilesCallback, &outFileVec);
	}

	//////////////////////////////////////////////////////////////////////////
	static bool GrepFilesRecursivelyCallback(void* pContext, const ST_FILETRAVERSE_INFO& stInfo)
	{
		ST_ENUMFILE_RECURSIVE_DATA* pData = (ST_ENUMFILE_RECURSIVE_DATA*)pContext;
		return EC_CANCELED != FindFileWorker(false, stInfo.pszFilePath, pData->pszPattern, pData->fpCallback, pData->pCallbackContext);
	}

	//////////////////////////////////////////////////////////////////////////	
	ECODE GrepFilesRecursively(std::tstring strDir, std::tstring strPattern, std::vector<std::tstring>& outFileVec)
	{
		ST_ENUMFILE_RECURSIVE_DATA stData;
		stData.pszPattern = strPattern.c_str();
		stData.fpCallback = GrepFilesCallback;
		stData.pCallbackContext	= &outFileVec;
		return TraverseDirectoryRecursively(strDir, TEXT("*"), GrepFilesRecursivelyCallback, &stData);
	}

	//////////////////////////////////////////////////////////////////////////
	static bool GrepDirectoriesCallback(void* pContext, const ST_FILETRAVERSE_INFO& stInfo)
	{
		std::vector<std::tstring>& vecFiles = *(std::vector<std::tstring>*)pContext;
		vecFiles.push_back(stInfo.pszFilePath);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE GrepDirectories(std::tstring strDir, std::tstring strPattern, std::vector<std::tstring>& outFileVec)
	{
		return FindFileWorker(true, strDir, strPattern, GrepDirectoriesCallback, &outFileVec);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE GrepDirectoriesRecursively(std::tstring strDir, std::tstring strPattern, std::vector<std::tstring>& outFileVec)
	{
		return TraverseDirectoryRecursively(strDir.c_str(), strPattern.c_str(), GrepDirectoriesCallback, &outFileVec);
	}

	//////////////////////////////////////////////////////////////////////////
	struct INTERNAL_ST_COPYDIRECTORY_DATA
	{
		size_t tTargetLength;
		LPCTSTR pszAlternativeText;
	};

	//////////////////////////////////////////////////////////////////////////
	static bool CopyFileWorkerCallback(void* pContext, const ST_FILETRAVERSE_INFO& stInfo)
	{
		INTERNAL_ST_COPYDIRECTORY_DATA* pData = (INTERNAL_ST_COPYDIRECTORY_DATA*)pContext;

		std::tstring strNewPath(stInfo.pszFilePath);
		strNewPath.replace(0, pData->tTargetLength, pData->pszAlternativeText);
		return CopyFile(stInfo.pszFilePath, strNewPath.c_str());;
	}

	//////////////////////////////////////////////////////////////////////////
	static bool CopyDirectoryTraverseCallback(void* pContext, const ST_FILETRAVERSE_INFO& stInfo)
	{
		INTERNAL_ST_COPYDIRECTORY_DATA* pData = (INTERNAL_ST_COPYDIRECTORY_DATA*)pContext;

		std::tstring strNewPath(stInfo.pszFilePath);
		strNewPath.replace(0, pData->tTargetLength, pData->pszAlternativeText);
		CreateDirectory(strNewPath.c_str());
		return EC_SUCCESS == FindFileWorker(false, stInfo.pszFilePath, TEXT("*"), CopyFileWorkerCallback, pContext);
	}

	//////////////////////////////////////////////////////////////////////////
	static inline ECODE CopyDirectoryWorker(LPCTSTR pszDirFrom, size_t tDirFromLen, LPCTSTR pszDirTo)
	{
		INTERNAL_ST_COPYDIRECTORY_DATA stData;
		stData.tTargetLength = tDirFromLen;
		stData.pszAlternativeText = pszDirTo;
		return TraverseDirectoryRecursively(pszDirFrom, TEXT("*"), CopyDirectoryTraverseCallback, &stData);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CopyDirectory(std::tstring strDirFrom, std::tstring strDirTo)
	{
		return CopyDirectoryWorker(strDirFrom.c_str(), strDirFrom.length(), strDirTo.c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CopyDirectory(LPCTSTR pszDirFrom, LPCTSTR pszDirTo)
	{
		return CopyDirectoryWorker(pszDirFrom, SafeStrLen(pszDirFrom, 0xFFFF), pszDirTo);
	}

	//////////////////////////////////////////////////////////////////////////
	bool CopyFiles(std::tstring strSourceFiles, std::tstring strDestDir)
	{
		std::vector<std::tstring> vecFiles;
		std::tstring strSourceDir = ExtractDirectory(strSourceFiles);
		std::tstring strPattern = ExtractFileName(strSourceFiles);

		GrepFiles(strSourceDir, strPattern, vecFiles);
		CreateDirectoryRecursively(strDestDir);

		size_t i;
		for (i = 0; i < vecFiles.size(); i++)
		{
			std::tstring strFileName = ExtractFileName(vecFiles[i]);
			std::tstring strTargetFile = strDestDir + TEXT("/") + strFileName;
			if (!CopyFile(vecFiles[i].c_str(), strTargetFile.c_str()))
			{
				Log_Error(TEXT("CopyFile(%s, %s) has failed"), vecFiles[i].c_str(), strTargetFile.c_str());
				return false;
			}
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool MoveFileForcely(std::tstring strSourceFile, std::tstring strDestFile)
	{
		if (MoveFile(strSourceFile.c_str(), strDestFile.c_str()))
			return true;

		if (!CopyFile(strSourceFile.c_str(), strDestFile.c_str()))
		{
			Log_Error(TEXT("CopyFile(%s, %s) has failed"), strSourceFile.c_str(), strDestFile.c_str());
			return false;
		}

		DeleteFile(strSourceFile.c_str());
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool MoveFiles(std::tstring strSourceFiles, std::tstring strDestDir, bool bOverwrite)
	{
		std::vector<std::tstring> vecFiles;
		std::tstring strSourceDir = ExtractDirectory(strSourceFiles);
		std::tstring strPattern = ExtractFileName(strSourceFiles);

		GrepFiles(strSourceDir, strPattern, vecFiles);
		CreateDirectoryRecursively(strDestDir);

		size_t i;
		for (i = 0; i < vecFiles.size(); i++)
		{
			std::tstring strFileName = ExtractFileName(vecFiles[i]);
			std::tstring strTargetFile = strDestDir + TEXT("/") + strFileName;
			if (!bOverwrite && PathFileExists(strTargetFile.c_str()))
				continue;

			if (!MoveFileForcely(vecFiles[i], strTargetFile))
				Log_Warn(TEXT("MoveFile(%s, %s) has failed"), vecFiles[i].c_str(), strTargetFile.c_str());
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ClearDirectory(std::tstring strDirectory)
	{
		std::vector<std::tstring> vecFiles;
		GrepFiles(strDirectory, TEXT("*"), vecFiles);

		size_t i;
		for (i = 0; i < vecFiles.size(); i++)
		{
			if (!DeleteFile(vecFiles[i].c_str()))
				return false;
		}
		return true;
	}

}

