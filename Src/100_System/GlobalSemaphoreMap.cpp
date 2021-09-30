#include "stdafx.h"
#include "GlobalSemaphoreMap.h"
#include "Log.h"
#include "InterlockedOperation.h"
#include "Environment.h"
#include "KernelObject.h"
#include "FileSystem.h"
#include <string.h>
#ifdef __APPLE__
#include <libproc.h>
#endif

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CGlobalSemaphoreMap::CGlobalSemaphoreMap(void)
	{
		// 고정된 공유 메모리 키
		static const DWORD s_SharedMemKey = 0x78888888;

		m_tMemorySize = 1024 * 1024;

		m_hSharedMem = OpenFileMapping(FILE_MAP_READ_|FILE_MAP_WRITE_, s_SharedMemKey);
		if( NULL == m_hSharedMem )
		{
			m_hSharedMem = CreateFileMapping(NULL, PAGE_READWRITE_, FILE_MAP_READ_|FILE_MAP_WRITE_, m_tMemorySize, s_SharedMemKey);
			m_pMemory = (LPBYTE)MapViewOfFile(m_hSharedMem, 0, m_tMemorySize);
			printf("memory cleaning.\n");
			memset(m_pMemory, 0, m_tMemorySize);
		}
		else
			m_pMemory = (LPBYTE)MapViewOfFile(m_hSharedMem, 0, m_tMemorySize);

		m_pBaseData = (_ST_SEM_BASE_DATA*)m_pMemory;
		m_pSemDataArr = (_ST_SEM_DATA*)(m_pMemory + sizeof(_ST_SEM_BASE_DATA));
		m_pRefCountArr = (_ST_SEM_REFCOUNT*)(m_pMemory + m_tMemorySize);
	}

	//////////////////////////////////////////////////////////////////////////
	CGlobalSemaphoreMap::~CGlobalSemaphoreMap(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	inline bool IsExistPid(int nPID)
	{
#ifdef _MSC_VER
		return true;
#endif
#ifdef __linux__
		std::string strProcPath = Format("/proc/%d", nPID);
		if( IsDirectoryA(strProcPath.c_str()) )
			return true;
		return false;
#endif
#ifdef __APPLE__
        char pathbuf[PROC_PIDPATHINFO_MAXSIZE];
        return proc_pidpath(nPID, pathbuf, PROC_PIDPATHINFO_MAXSIZE) > 0;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	struct _ST_SEM_HANDLE_DATA
	{
		char szName[MAX_SEM_NAME];
		_ST_SEM_REFCOUNT* pRefCount;
		int nPID;
	};
	
	//////////////////////////////////////////////////////////////////////////
	CGlobalSemaphoreMap::Owner::Owner(CGlobalSemaphoreMap* pThis, int nPID)
		: m_Instance(*pThis)
		, m_nPID(nPID)
	{
		while(1)
		{
			int nLastPID = InterlockedCompareExchange_(&m_Instance.m_pBaseData->nEnterPID, nPID, 0);
			if( 0 == nLastPID )
				break;

			if( IsExistPid(nLastPID) )
			{
				Sleep(0);
				continue;
			}

			int nCleanupPid = InterlockedCompareExchange_(&m_Instance.m_pBaseData->nEnterPID, nPID, nLastPID);
			if( nCleanupPid != nLastPID )
			{
				Sleep(0);
				continue;
			}

			m_Instance.Remove(nCleanupPid);
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	CGlobalSemaphoreMap::Owner::~Owner(void)
	{
		InterlockedCompareExchange_(&m_Instance.m_pBaseData->nEnterPID, 0, m_nPID);
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE CGlobalSemaphoreMap::Create(LPCSTR pszName, int nPID)
	{
		Owner Lock(this, nPID);

		_ST_SEM_DATA* pSemData = FindSemData(pszName);
		if( NULL == pSemData )
			pSemData = NewSemData(pszName, nPID);

		if( 0 == pSemData->nFirstBlockIndex )
			pSemData->nFirstBlockIndex = NewRefCount(nPID);

		_ST_SEM_REFCOUNT* pRefCount = FindRefCount(pSemData->nFirstBlockIndex, nPID);
		if( NULL == pRefCount )
			pRefCount = NewRefCount(pSemData->nFirstBlockIndex, nPID);
		pRefCount->nValue++;

		_ST_SEM_HANDLE_DATA* pHandleData = new _ST_SEM_HANDLE_DATA;
		pHandleData->nPID = nPID;
		pHandleData->pRefCount = pRefCount;
		StringCchCopy(pHandleData->szName, MAX_SEM_NAME, pszName);
		return (HANDLE)pHandleData;
	}

	//////////////////////////////////////////////////////////////////////////
	std::string CGlobalSemaphoreMap::Destroy(HANDLE hSemaphore)
	{
		_ST_SEM_HANDLE_DATA* pHandleData = (_ST_SEM_HANDLE_DATA*)hSemaphore;
		Owner Lock(this, pHandleData->nPID);

		std::string strRet;
		do 
		{
			pHandleData->pRefCount->nValue--;
			if( pHandleData->pRefCount->nValue > 0 )
				break;

			_ST_SEM_DATA* pSemData = FindSemData(pHandleData->szName);
			if( NULL == pSemData )
			{
				Log_Debug("Sem data called [%s] is not found.", pHandleData->szName);
				break;
			}

			DeleteRefCount(pSemData, pHandleData->pRefCount);
			if( pSemData->nFirstBlockIndex != 0 )
				break;

			DeleteSemData(pSemData);
			strRet = pHandleData->szName;
		}	while (0);

		delete pHandleData;
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	void CGlobalSemaphoreMap::MarkOwner(HANDLE hSemaphore)
	{
		_ST_SEM_HANDLE_DATA* pHandleData = (_ST_SEM_HANDLE_DATA*)hSemaphore;
		Owner Lock(this, pHandleData->nPID);

		_ST_SEM_DATA* pSemData = FindSemData(pHandleData->szName);
		if( NULL == pSemData )
		{
			Log_Debug("Sem data called [%s] is not found.", pHandleData->szName);
			return;
		}

		pSemData->nOwnerPID = pHandleData->nPID;
	}

	//////////////////////////////////////////////////////////////////////////
	void CGlobalSemaphoreMap::UnmarkOwner(HANDLE hSemaphore)
	{
		_ST_SEM_HANDLE_DATA* pHandleData = (_ST_SEM_HANDLE_DATA*)hSemaphore;
		Owner Lock(this, pHandleData->nPID);

		_ST_SEM_DATA* pSemData = FindSemData(pHandleData->szName);
		if( NULL == pSemData )
		{
			Log_Debug("Sem data called [%s] is not found.", pHandleData->szName);
			return;
		}

		pSemData->nOwnerPID = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	bool CGlobalSemaphoreMap::CheckAbandoned(HANDLE hSemaphore)
	{
		_ST_SEM_HANDLE_DATA* pHandleData = (_ST_SEM_HANDLE_DATA*)hSemaphore;
		Owner Lock(this, pHandleData->nPID);

		_ST_SEM_DATA* pSemData = FindSemData(pHandleData->szName);
		if( NULL == pSemData )
		{
			Log_Debug("Sem data called [%s] is not found.", pHandleData->szName);
			return false;
		}

		if( 0 == pSemData->nOwnerPID )
			return false;

		if( pSemData->nOwnerPID == pHandleData->nPID )
			return false;

		if( IsExistPid(pSemData->nOwnerPID) )
			return false;

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool CGlobalSemaphoreMap::IsEmpty(void)
	{
		return m_pSemDataArr->szName[0] == 0;
	}

	//////////////////////////////////////////////////////////////////////////
	bool CGlobalSemaphoreMap::Refresh(LPCSTR pszName)
	{
		_ST_SEM_DATA* pSemData = FindSemData(pszName);
		if( NULL == pSemData )
			return false;

		_ST_SEM_REFCOUNT* pRefCount = NULL;
		int nLastIndex = pSemData->nFirstBlockIndex;
		while(nLastIndex < 0)
		{
			pRefCount = &m_pRefCountArr[nLastIndex];
			if( pRefCount->nPID && !IsExistPid(pRefCount->nPID) )
			{
				if( RemoveWorker(pSemData, pRefCount->nPID) )
					return true;

				nLastIndex = pSemData->nFirstBlockIndex;
				continue;
			}

			if( 0 == pRefCount->nNextIndex )
				break;

			nLastIndex = pRefCount->nNextIndex;
		}
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool CGlobalSemaphoreMap::RemoveWorker(_ST_SEM_DATA* pSemData, int nPID)
	{
		_ST_SEM_REFCOUNT* pRefCount = FindRefCount(pSemData->nFirstBlockIndex, nPID);
		if( NULL == pRefCount )
			return false;

		DeleteRefCount(pSemData, pRefCount);
		if( pSemData->nFirstBlockIndex != 0 )
			return false;

		DeleteSemData(pSemData);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	void CGlobalSemaphoreMap::Remove(int nPID)
	{
		int i;
		for(i=0; m_pSemDataArr[i].nFirstBlockIndex < 0; i++)
		{
			if( !RemoveWorker(&m_pSemDataArr[i], nPID) )
				continue;

			// SemData를 삭제하면 현재 위치에 마지막 SemData가 들어옴
			// 따라서 i 값을 현재 위치부터 다시 검사하도록 i-- 처리함
			i--;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	_ST_SEM_DATA* CGlobalSemaphoreMap::NewSemData(LPCSTR pszName, int nPID)
	{
		size_t i;
		for(i=0; 0 != m_pSemDataArr[i].szName[0]; i++);

		StringCchCopy(m_pSemDataArr[i].szName, MAX_SEM_NAME, pszName);
        m_pSemDataArr[i].nOwnerPID = nPID;
		return &m_pSemDataArr[i];
	}

	//////////////////////////////////////////////////////////////////////////
	_ST_SEM_DATA* CGlobalSemaphoreMap::FindSemData(LPCSTR pszName)
	{
		size_t i;
		for(i=0; m_pSemDataArr[i].szName[0] != 0; i++)
		{
			if( 0 == SafeStrCmp(m_pSemDataArr[i].szName, pszName, MAX_SEM_NAME) )
				return &m_pSemDataArr[i];
		}

		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	void CGlobalSemaphoreMap::DeleteSemData(_ST_SEM_DATA* pTargetSem)
	{
		if( 0 == m_pSemDataArr->szName[0] )
			return;

		// 마지막 다음의 인덱스를 찾음
		int nEndIndex;
		for(nEndIndex=1; m_pSemDataArr[nEndIndex].szName[0] != 0; nEndIndex++);

		// 삭제하더라도 데이터를 연속해서 보관하도록 마지막 데이터와 맞바꿈
		_ST_SEM_DATA* pLastSem = &m_pSemDataArr[nEndIndex-1];
		if( pLastSem != pTargetSem )
			*pTargetSem = *pLastSem;

		pLastSem->szName[0] = 0;
		pLastSem->nFirstBlockIndex = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	_ST_SEM_REFCOUNT* CGlobalSemaphoreMap::FindRefCount(int nFirstBlockIndex, int nPID)
	{
		_ST_SEM_REFCOUNT* pRefCount = NULL;
		int nLastIndex = nFirstBlockIndex;
		while(nLastIndex != 0)
		{
			pRefCount = &m_pRefCountArr[nLastIndex];
			if( pRefCount->nPID == nPID )
				return pRefCount;

			if( 0 == pRefCount->nNextIndex )
				break;

			nLastIndex = pRefCount->nNextIndex;
		}

		if( NULL == pRefCount || pRefCount->nPID != nPID )
			return NULL;
		return pRefCount;
	}

	//////////////////////////////////////////////////////////////////////////
	int CGlobalSemaphoreMap::NewRefCount(int nPID)
	{
		// 비어있는 블록 찾기
		int nNewBlockIndex;
		for(nNewBlockIndex=-1; m_pRefCountArr[nNewBlockIndex].nValue > 0; nNewBlockIndex--);

		m_pRefCountArr[nNewBlockIndex].nPID  = nPID;
		m_pRefCountArr[nNewBlockIndex].nValue = 0;
		m_pRefCountArr[nNewBlockIndex].nNextIndex  = 0;
		m_pRefCountArr[nNewBlockIndex].nPrevIndex  = 0;

		return nNewBlockIndex;
	}

	//////////////////////////////////////////////////////////////////////////
	_ST_SEM_REFCOUNT* CGlobalSemaphoreMap::NewRefCount(int nFirstBlockIndex, int nPID)
	{
		_ST_SEM_REFCOUNT* pLastRef = NULL;

		int nLastBlockIndex = nFirstBlockIndex;
		while(nLastBlockIndex < 0)
		{
			pLastRef = &m_pRefCountArr[nLastBlockIndex];
			if( pLastRef->nNextIndex == 0 )
				break;

			nLastBlockIndex = pLastRef->nNextIndex;
		}

		// 비어있는 블록 찾기
		int nNewBlockIndex;
		for(nNewBlockIndex=-1; m_pRefCountArr[nNewBlockIndex].nValue > 0; nNewBlockIndex--);

		m_pRefCountArr[nNewBlockIndex].nPID  = nPID;
		m_pRefCountArr[nNewBlockIndex].nValue = 0;
		m_pRefCountArr[nNewBlockIndex].nNextIndex  = 0;
		m_pRefCountArr[nNewBlockIndex].nPrevIndex  = nLastBlockIndex;

		if( pLastRef )
		{
			pLastRef->nNextIndex = nNewBlockIndex;
			m_pRefCountArr[nNewBlockIndex].nPrevIndex = nLastBlockIndex;
		}
		return &m_pRefCountArr[nNewBlockIndex];
	}

	//////////////////////////////////////////////////////////////////////////
	void CGlobalSemaphoreMap::DeleteRefCount(_ST_SEM_DATA* pSemData, _ST_SEM_REFCOUNT* pRefCount)
	{
		_ST_SEM_REFCOUNT* pNext = NULL;
		if( pRefCount->nNextIndex < 0 )
			pNext = &m_pRefCountArr[pRefCount->nNextIndex];

		_ST_SEM_REFCOUNT* pPrev = NULL;
		if( pRefCount->nPrevIndex < 0 )
			pPrev = &m_pRefCountArr[pRefCount->nPrevIndex];

		if( pNext )
			pNext->nPrevIndex = pRefCount->nPrevIndex;

		if( pPrev )
			pPrev->nNextIndex = pRefCount->nNextIndex;

		if( NULL == pPrev )
			pSemData->nFirstBlockIndex = pRefCount->nNextIndex;

		memset(pRefCount, 0, sizeof(_ST_SEM_REFCOUNT));
	}
}
