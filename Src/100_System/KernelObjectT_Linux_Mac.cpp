#include "stdafx.h"
#include "KernelObject.h"
#include "GlobalSemaphoreMap.h"
#ifdef __linux__
#include "System_Linux.h"
#else
#include "System_Mac.h"
#endif
#include "Environment.h"
#include "FileSystem.h"
#include "Log.h"
#include <vector>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	HANDLE CreateFile(LPCTSTR lpFileName, DWORD dwDesiredAccess, E_FILE_DISPOSITION nDisposition, DWORD dwAttributes, DWORD dwMode, HANDLE hTemplateFile)
	{
		// exception case
		if( (FILE_APPEND_DATA_ & dwDesiredAccess) && (TRUNCATE_EXISTING_ == nDisposition) )
			return NULL;

		std::string strFileNameA = MBSFromTCS(lpFileName);

		int nFlag = 0;
		if( FILE_APPEND_DATA_ & dwDesiredAccess )
			nFlag = O_APPEND|O_WRONLY;
		else if( (GENERIC_READ_ & dwDesiredAccess) && (GENERIC_WRITE_ & dwDesiredAccess) )
			nFlag |= O_RDWR;
		else if( GENERIC_READ_ & dwDesiredAccess )
			nFlag |= O_RDONLY;
		else if( GENERIC_WRITE_ & dwDesiredAccess )
			nFlag |= O_WRONLY;

		if( FILE_ATTRIBUTE_DIRECTORY_ & dwAttributes )
			nFlag |= O_DIRECTORY;

		if( FILE_ATTRIBUTE_TEMPORARY_ & dwAttributes )
		{
#ifdef O_TMPFILE
			strFileNameA = ExtractDirectory(strFileNameA);
			nFlag |= O_TMPFILE;
#else
			Log_Error("This system is not support O_TMPFILE");
#endif
		}

		nFlag |= O_CLOEXEC;

		switch(nDisposition)
		{
		case CREATE_NEW_		:		nFlag |= O_CREAT|O_EXCL;	break;
		case CREATE_ALWAYS_		:		nFlag |= O_CREAT|O_TRUNC;	break;
		case OPEN_EXISTING_		:		nFlag |= O_EXCL;			break;
		case OPEN_ALWAYS_		:		nFlag |= O_CREAT;			break;
		case TRUNCATE_EXISTING_	:		nFlag |= O_EXCL|O_TRUNC;	break;
		}

		int nFile = ::open(strFileNameA.c_str(), nFlag, dwMode);
		if( nFile < 0 )
		{
			Log_Error("open failed, %d(%s)", errno, strerror(errno));
			return NULL;
		}
		
		// FD zero is reserved for INVALID HANDLE
		// Change to non-zero
		if (0 == nFile)
		{
			nFile = dup(0);
			close(0);
		}
		return (HANDLE)(size_t)nFile;
	}

	//////////////////////////////////////////////////////////////////////////
	static inline HANDLE CreateProcessWorker(LPCTSTR pszFilePath, LPCTSTR pszDirectory, const std::vector<LPCSTR>& vecArgs, ST_STARTUPINFO* pStartupInfo, ST_PROCESSINFO* pProcessInfo)
	{
		try
		{
			Log_Info("--------------------------");
			Log_Info(TEXT("- FilePath   :%s"), pszFilePath	);
			Log_Info(TEXT("- Directory  :%s"), pszDirectory? pszDirectory : TEXT(""));

			std::string strParamA = vecArgs.size() == 2? "" : vecArgs[1];

			size_t i;
			for(i=2; i<vecArgs.size() - 1; i++)
				strParamA += Format(" %s", vecArgs[i]);
			Log_Info("- Param      :%s", strParamA.c_str()		);

			Log_Info(TEXT("Try to call CreateProcess. FilePath:%s"), pszFilePath);
			if( !IsFileExist(pszFilePath) )
				throw exception_format(TEXT("file(%s) is not exist."), pszFilePath);

			int nStdInPipe[2] = { 0, 0 };
			int nStdOutPipe[2] = { 0, 0 };
			if( pStartupInfo )
			{
				if( 0 < pipe(nStdInPipe) || pipe(nStdOutPipe) )
					Log_Error("pipe() calling failure");
				pStartupInfo->hStdInput = (HANDLE)(size_t)nStdInPipe[1];
				pStartupInfo->hStdOutput = (HANDLE)(size_t)nStdOutPipe[0];
				pStartupInfo->hStdError = (HANDLE)(size_t)nStdOutPipe[0];
			}

			int nPID = ::fork();
			if( nPID < 0 )
				throw exception_format("fork failure, %s", strerror(errno));

			// child
			if( 0 == nPID )
			{
				if( pStartupInfo && nStdInPipe[1] && nStdOutPipe[1] )
				{
					dup2(nStdInPipe[0], STDIN_FILENO);
					dup2(nStdOutPipe[1], STDOUT_FILENO);
					dup2(nStdOutPipe[1], STDERR_FILENO);
					close(nStdInPipe[1]);
					close(nStdOutPipe[0]);
					close(nStdOutPipe[1]);
				}
				if( pszDirectory )
				{
					std::string strDirectoryA = MBSFromTCS(pszDirectory);
					if( ::chdir(strDirectoryA.c_str()) < 0 )
					{
						Log_Error("chdir(%s) failure, %s", strDirectoryA.c_str(), strerror(errno));
						exit(errno);
					}
				}

				std::string strExePathA = MBSFromTCS(pszFilePath);
				if( ::execv(strExePathA.c_str(), (char* const*)&vecArgs[0]) < 0 )
				{
					Log_Error("execvp operation has failed, %s", strerror(errno));
					exit(errno);
				}
			}

			// parent
			Log_Info(TEXT("%s(pid:%d) created."), pszFilePath, nPID);
			Log_Info("--------------------------");

			if( pStartupInfo && nStdInPipe[0] )
			{
				close(nStdInPipe[0]);
				close(nStdOutPipe[1]);
			}
			return (HANDLE)(size_t)nPID;
		}
		catch(std::exception& e)
		{
			Log_Error("%s", e.what());
			Log_Info("--------------------------");
			return NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE CreateProcess(LPCTSTR pszFilePath, LPCTSTR pszDirectory, ST_STARTUPINFO* pStartupInfo, ST_PROCESSINFO* pProcessInfo)
	{
		std::string strFilePathA = MBSFromTCS(pszFilePath);
		std::vector<LPCSTR> vecArgsA;
		vecArgsA.push_back(strFilePathA.c_str());
		vecArgsA.push_back(NULL);
		return CreateProcessWorker(pszFilePath, pszDirectory, vecArgsA, pStartupInfo, pProcessInfo);
	}

	//////////////////////////////////////////////////////////////////////////	
	HANDLE CreateProcess(LPCTSTR pszFilePath, LPCTSTR pszDirectory, const std::vector<std::tstring>& vecArgs, ST_STARTUPINFO* pStartupInfo, ST_PROCESSINFO* pProcessInfo)
	{
		size_t tArgsCount = vecArgs.size();

		std::vector<std::string> vecArgsInstance;
		vecArgsInstance.resize(tArgsCount+1);
		vecArgsInstance[0] = MBSFromTCS(pszFilePath);

		std::vector<LPCSTR> vecArgsA;
		vecArgsA.resize(tArgsCount+2);
		vecArgsA[0] = vecArgsInstance[0].c_str();
		vecArgsA[tArgsCount+1] = NULL;

		size_t i;
		for(i=0; i<tArgsCount; i++)
		{
			vecArgsInstance[i+1] = MBSFromTCS(vecArgs[i]);
			vecArgsA[i+1] = vecArgsInstance[i+1].c_str();
		}
		return CreateProcessWorker(pszFilePath, pszDirectory, vecArgsA, pStartupInfo, pProcessInfo);
	}

	//////////////////////////////////////////////////////////////////////////
	int ShellExecuteByPipe(std::tstring strCmdLine, std::tstring& strOutput)
	{
		FILE* pPipe = NULL;
		int nExitCode;
		try
		{
			pPipe = ::popen(MBSFromTCS(strCmdLine).c_str(), "r");
			if( NULL == pPipe )
				throw exception_format(TEXT("Execute(%s) has failed."), strCmdLine.c_str());

			const size_t tBuffSize = 64;
			char szBuf[tBuffSize];
			while(NULL != ::fgets(szBuf, tBuffSize, pPipe))
				strOutput += TCSFromMBS(szBuf);

			nExitCode = ::pclose(pPipe);
			if( 0 == WIFEXITED(nExitCode) )
				throw exception_format(TEXT("%s process is not well finished"), strCmdLine.c_str());
		}
		catch (std::exception& e)
		{
			Log_Error(TEXT("%s"), e.what());
			if( pPipe )
				::pclose(pPipe);
			return -1;
		}

		return (signed char)WEXITSTATUS(nExitCode);
	}

	//////////////////////////////////////////////////////////////////////////
	static std::string g_strPipePath = "/tmp/pipe";
	HANDLE CreatePipe(LPCTSTR pszName)
	{
		std::string strName;
		int nFile = -1;

		try
		{
			int nRet = ::mkdir(g_strPipePath.c_str(), 0666);
			if( nRet < 0 && EEXIST != errno )
			{
				printf("mkdir(%s) error:%d\n", g_strPipePath.c_str(), errno);
				return NULL;
			}

			strName = g_strPipePath + "/" + MBSFromTCS(pszName);
			::unlink(strName.c_str());
			if( ::mkfifo(strName.c_str(), 0666) < 0 )
				throw exception_format("mkfifo(%s) operation failed, %d", strName.c_str(), errno);

			nFile = ::open(strName.c_str(), O_RDWR);
			if( nFile < 0 )
				throw exception_format(TEXT("mkfifo and open(%s) pipe failure, %d"), strName.c_str(), errno);
		}
		catch(std::exception& e)
		{
			Log_Error("%s", e.what());
			::unlink(strName.c_str());
			if( nFile > 0 )
				::close(nFile);

			return NULL;
		}

		return (HANDLE)(size_t)nFile;
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE OpenPipe(LPCTSTR pszName)
	{
		int nFile = -1;

		try
		{
			int nRet = ::mkdir(g_strPipePath.c_str(), 0666);
			if( nRet < 0 && EEXIST != errno )
			{
				g_strPipePath = "/usr/pipe";
				nRet = ::mkdir(g_strPipePath.c_str(), 0666);
			}

			if( nRet < 0 && EEXIST != errno )
			{
				printf("mkdir(%s) error:%d\n", g_strPipePath.c_str(), errno);
				return NULL;
			}

			std::string strName = g_strPipePath + "/" + MBSFromTCS(pszName);
			nFile = ::open(strName.c_str(), O_RDWR);
			if( nFile < 0 )
				throw exception_format(TEXT("open(%s) pipe failure, %d"), strName.c_str(), errno);
		}
		catch(std::exception& e)
		{
			Log_Error("%s", e.what());
			if( nFile > 0 )
				::close(nFile);

			return NULL;
		}

		return (HANDLE)(size_t)nFile;
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE CreateSemaphore(int nInitialCount, LPCTSTR pszName)
	{
		HANDLE hRet = NULL;
		CSemHandle SemHandle;
		try
		{
			hRet = Internal()->CreateSemHandle(SemHandle);
			if( NULL == hRet )
				throw exception_format("Internal()->CreateSemHandle() failure");

			if( pszName )
			{
				std::string strName = MBSFromTCS(pszName);
				if( GlobalSemMap()->Refresh(strName.c_str()) )
					::sem_unlink(strName.c_str());
				SemHandle->nType = _ST_SEM_HANDLE::NAMED_SEM;
				SemHandle->pSem = ::sem_open(strName.c_str(), O_CREAT, 0777, nInitialCount);
				if( SEM_FAILED == SemHandle->pSem )
					throw exception_format("sem_open failure, %d(%s).", errno, strerror(errno));
				SemHandle->hGlobalSem = GlobalSemMap()->Create(strName.c_str(), GetCurrentProcessId());
			}
			else
			{
#ifdef __linux__
				SemHandle->nType = _ST_SEM_HANDLE::UNNAMED_SEM;
				SemHandle->pSem = new(std::nothrow) sem_t;
				if( ::sem_init(SemHandle->pSem, 0, nInitialCount) )
					throw exception_format("sem_init failure, %d(%s).", errno, strerror(errno));
#else
				SemHandle->nType = _ST_SEM_HANDLE::UNNAMED_SEM;
                SemHandle->Unnamed.Sem = dispatch_semaphore_create(nInitialCount);
                SemHandle->Unnamed.nFlag = 0;
				if( NULL == SemHandle->Unnamed.Sem )
					throw exception_format("dispatch_semaphore_create failure, %d(%s).", errno, strerror(errno));
#endif
			}
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			if( hRet )
				Internal()->DestroySemHandle(hRet);
			return NULL;
		}
		return hRet;
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE OpenSemaphore(LPCTSTR pszName)
	{
		HANDLE hRet = NULL;
		CSemHandle SemHandle;
		try
		{
			if( NULL == pszName )
				throw exception_format("OpenSemaphore name should be specified.");

			hRet = Internal()->CreateSemHandle(SemHandle);
			if( NULL == hRet )
				throw exception_format("Internal()->CreateSemHandle() failure");

			std::string strName = MBSFromTCS(pszName);
			if( GlobalSemMap()->Refresh(strName.c_str()) )
				::sem_unlink(strName.c_str());
			SemHandle->nType = _ST_SEM_HANDLE::NAMED_SEM;
			SemHandle->pSem = ::sem_open(strName.c_str(), 0);
			if( SEM_FAILED == SemHandle->pSem )
				throw exception_format("sem_open(%s) failure, %d", strName.c_str(), errno);
			SemHandle->hGlobalSem = GlobalSemMap()->Create(strName.c_str(), GetCurrentProcessId());
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			if( hRet )
				Internal()->DestroySemHandle(hRet);
			return NULL;
		}
		return hRet;
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE CreateMutex(bool bInitialOwner, LPCTSTR pszName)
	{
		int nInitialCount = bInitialOwner? 0 : 1;
		HANDLE hRet = NULL;
		CSemHandle SemHandle;
		try
		{
			hRet = Internal()->CreateSemHandle(SemHandle);
			if( NULL == hRet )
				throw exception_format("Internal()->CreateSemHandle() failure");

			if( pszName )
			{
				std::string strName = MBSFromTCS(pszName);
				if( GlobalSemMap()->Refresh(strName.c_str()) )
					::sem_unlink(strName.c_str());
				SemHandle->nType = _ST_SEM_HANDLE::NAMED_SEM;
				SemHandle->pSem = ::sem_open(strName.c_str(), O_CREAT|O_EXCL, 0777, nInitialCount);
				SemHandle->nOwnerTID = GetCurrentThreadId();
				SemHandle->nLockCount = bInitialOwner? 1 : 0;
				if( SEM_FAILED == SemHandle->pSem && (EEXIST == errno) )
				{
					errno = EC_ALREADY_EXISTS;
					SemHandle->pSem = ::sem_open(strName.c_str(), O_CREAT, 0777, nInitialCount);
					SemHandle->nOwnerTID = 0;
					SemHandle->nLockCount = 0;
				}
				
				if( SEM_FAILED == SemHandle->pSem )
					throw exception_format("sem_open failure, %d(%s).", errno, strerror(errno));
				SemHandle->hGlobalSem = GlobalSemMap()->Create(strName.c_str(), GetCurrentProcessId());
			}
			else
			{
#ifdef __linux__
				SemHandle->nType = _ST_SEM_HANDLE::UNNAMED_SEM;
				SemHandle->pSem = new(std::nothrow) sem_t;
				if( ::sem_init(SemHandle->pSem, 0, nInitialCount) )
					throw exception_format("sem_init failure, %d(%s).", errno, strerror(errno));
#else
				SemHandle->nType = _ST_SEM_HANDLE::UNNAMED_SEM;
                SemHandle->Unnamed.Sem = dispatch_semaphore_create(1);
                SemHandle->Unnamed.nFlag = 0;
                if( NULL == SemHandle->Unnamed.Sem )
                    throw exception_format("dispatch_semaphore_create failure, %d(%s).", errno, strerror(errno));
#endif
			}
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			if( hRet )
				Internal()->DestroySemHandle(hRet);
			return NULL;
		}
		return hRet;
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE OpenMutex(LPCTSTR pszName)
	{
		HANDLE hRet = NULL;
		CSemHandle SemHandle;
		try
		{
			if( NULL == pszName )
				throw exception_format("OpenMutex name should be specified.");

			hRet = Internal()->CreateSemHandle(SemHandle);
			if( NULL == hRet )
				throw exception_format("Internal()->CreateSemHandle() failure");

			std::string strName = MBSFromTCS(pszName);
			if( GlobalSemMap()->Refresh(strName.c_str()) )
				::sem_unlink(strName.c_str());
			SemHandle->nType = _ST_SEM_HANDLE::NAMED_SEM;
			SemHandle->pSem = ::sem_open(strName.c_str(), 0);
			SemHandle->nOwnerTID = 0;
			SemHandle->nLockCount = 0;
			if( SEM_FAILED == SemHandle->pSem )
				throw exception_format("sem_open(%s) failure, %d", strName.c_str(), errno);
			SemHandle->hGlobalSem = GlobalSemMap()->Create(strName.c_str(), GetCurrentProcessId());
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			if( hRet )
				Internal()->DestroySemHandle(hRet);
			return NULL;
		}
		return hRet;
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE LoadLibrary(LPCTSTR pszPath)
	{
		std::string strPath = MBSFromTCS(pszPath);
		HANDLE hRet = dlopen(strPath.c_str(), RTLD_NOW);
		if( hRet )
			return hRet;
		Log_Error("dlopen(%s) has failed, %s", strPath.c_str(), dlerror());
		errno = EINVAL;
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t EnumProcesses(std::tstring strNamePattern, std::vector<ST_PROCESS_INFO>& vecProcesses)
	{
		std::string strTempFile = GenerateGuidA() + ".temp.output.txt";

		std::string strCommand = "ps -e -o pid,comm > " + strTempFile;
		::system(strCommand.c_str());

		FILE* pFile = fopen(strTempFile.c_str(), "rb");
		if( NULL == pFile )
		{
			Log_Error("Getting ps -e list(%s) has failed.", strCommand.c_str());
			return 0;
		}

		int i;
		for(i=0; ; i++)
		{
			char szBuff[1024] = { 0, };
			char* pszRet = fgets(szBuff, 1024, pFile);
			if( NULL == pszRet )
				break;

			if( 0 == i ) // skip the first, it has not process info, but title
				continue;

			std::string strLine = Trim(pszRet);
			if( strLine.empty() )
				break;

			int nLinePos = 0;
			std::string strPID = Tokenize(strLine, nLinePos);
			std::tstring strCMD = TCSFromMBS(strLine.substr(nLinePos));
			strCMD = ExtractFileName(Trim(strCMD));

			if( !SafeStrCmpWithWildcard(strCMD.c_str(), strCMD.length(), strNamePattern.c_str()) )
				continue;

			if( std::tstring::npos != strCMD.find(TEXT(" <defunct>")) )
				continue;

			size_t tPID = ValueFrom<size_t>(strPID);
			if( 0 == tPID )
				continue;

			ST_PROCESS_INFO stProcInfo;
			stProcInfo.tPID = tPID;
			stProcInfo.strName = strCMD;
			vecProcesses.push_back(stProcInfo);
		}

		fclose(pFile);
		::unlink(strTempFile.c_str());
		return vecProcesses.size();
	}
}
