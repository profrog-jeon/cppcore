#pragma once

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include "SharedPtr.h"
#include "KernelObject.h"
#include "GlobalSemaphoreMap.h"

namespace core
{
	struct _ST_FINDFIRSTFILE
	{
		std::tstring strRootPath;
		std::tstring strPattern;
		DIR* pDir;
	};

	struct _ST_SEM_HANDLE
	{
		enum
		{
			UNNAMED_SEM = 0,
			NAMED_SEM = 1,
		}	nType;

		sem_t* pSem;

		HANDLE hGlobalSem;
		int nOwnerTID;
		int nLockCount;

		_ST_SEM_HANDLE(void);
		~_ST_SEM_HANDLE(void);
	};

	struct _ST_EVENT_HANDLE
	{
		_ST_EVENT_HANDLE(void);
		~_ST_EVENT_HANDLE(void);

		pthread_mutex_t tMutex;
		pthread_cond_t tCond;
		bool bAutoReset;
		bool bSignaled;
		bool bReserved[6];
	};
	
	struct _ST_FILEMAP_HANDLE
	{
		enum {	FILEMAP, SHAREDMEM	}	nType;
		int nProtectFlag;
		DWORD dwKey;
		QWORD qwLength;
		union
		{
			struct  
			{
				int nFile;
				void* pBaseAddress;
			}	FileMap;

			struct
			{
				int nSharedMem;
				int nAccessPriv;
				int nProtectFlag;
			}	SharedMem;
		};
	};

	typedef CSharedPtr<_ST_SEM_HANDLE>				CSemHandle;
	typedef CSharedPtr<_ST_EVENT_HANDLE>			CEventHandle;

	class CInternal
	{
		typedef std::map<HANDLE, CSemHandle>				CSemHandleMap;
		typedef CSemHandleMap::iterator						CSemHandleMapIt;
		HANDLE m_csSemHandleMap;
		CSemHandleMap m_SemHandleMap;

		typedef std::map<HANDLE, CEventHandle>				CEventHandleMap;
		typedef CEventHandleMap::iterator					CEventHandleMapIt;
		HANDLE m_csEventHandleMap;
		CEventHandleMap m_EventHandleMap;

		typedef std::set<_ST_FILEMAP_HANDLE*>		CFilemapHandleSet;
		typedef CFilemapHandleSet::iterator					CFilemapHandleSetIt;
		HANDLE m_csFilemapHandleSet;
		CFilemapHandleSet m_FilemapHandleSet;

		CInternal(void);

	public:
		HANDLE CreateSemHandle(CSemHandle& outSemHandle);
		bool GetSemHandle(HANDLE hSemaphore, CSemHandle& outSemHandle);
		void DestroySemHandle(HANDLE hSemaphore);

		HANDLE CreateEventHandle(CEventHandle& outSemHandle);
		bool GetEventHandle(HANDLE hSemaphore, CEventHandle& outSemHandle);
		void DestroyEventHandle(HANDLE hSemaphore);

		void RegisterFilemapHandle(_ST_FILEMAP_HANDLE* pHandle);
		_ST_FILEMAP_HANDLE* GetFilemapHandle(HANDLE hFilemap);
		_ST_FILEMAP_HANDLE* GetFilemapHandleByKey(DWORD dwKey);
		void DestroyFilemapHandle(HANDLE hFilemap);

		static CInternal* GetInstance(void)
		{
			static CInternal instance;
			return &instance;
		}
	};

	inline CInternal* Internal(void)
	{
		return CInternal::GetInstance();
	}

	ECODE CalcDelayedClockTime(DWORD dwMilliSec, timespec& tTimeSpec);
}
