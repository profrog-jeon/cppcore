#pragma once

#include <Windows.h>
#include <map>
#include "../__Common/Type.h"
#include "KernelObject.h"

namespace core
{
	UINT64 UnixTimeFrom(const FILETIME& ft);

	class CWindowsInternal
	{
		typedef std::map<HANDLE, DWORD>		CSharedMemMap;
		typedef CSharedMemMap::iterator		CSharedMemMapIt;

		HANDLE m_csSharedMem;
		CSharedMemMap m_mapSharedMem;

		CWindowsInternal(void)
		{
			m_csSharedMem = InitializeCriticalSection();
		}

	public:
		void RegisterSharedMemHandle(HANDLE hHandle, DWORD dwFileMapAccess)
		{
			EnterCriticalSection(m_csSharedMem);
			m_mapSharedMem.insert(std::make_pair(hHandle, dwFileMapAccess));
			LeaveCriticalSection(m_csSharedMem);
		}

		void UnregisterSharedMemHandle(HANDLE hHandle)
		{
			EnterCriticalSection(m_csSharedMem);
			m_mapSharedMem.erase(hHandle);
			LeaveCriticalSection(m_csSharedMem);
		}

		ECODE GetSharedMemHandle(HANDLE hHandle, DWORD& dwFileMapAccess)
		{
			ECODE nRet = EC_SUCCESS;
			EnterCriticalSection(m_csSharedMem);
			CSharedMemMapIt iter = m_mapSharedMem.find(hHandle);
			if( iter == m_mapSharedMem.end() )
				nRet = EC_INVALID_HANDLE;
			else
				dwFileMapAccess =  iter->second;
			LeaveCriticalSection(m_csSharedMem);
			return nRet;
		}

		static CWindowsInternal* GetInstance(void)
		{
			static CWindowsInternal instance;
			return &instance;
		}
	};

	inline CWindowsInternal* Internal(void)
	{
		return CWindowsInternal::GetInstance();
	}
}
