#pragma once

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	//Named semaphores
	//	A named semaphore is identified by a name of the form
	//	/somename; that is, a null-terminated string of up to
	//	NAME_MAX-4 (i.e., 251) characters consisting of an initial
	//	slash, followed by one or more characters, none of which are
	//	slashes.  Two processes can operate on the same named
	//	semaphore by passing the same name to sem_open(3).
#define MAX_SEM_NAME	252

	struct _ST_SEM_REFCOUNT
	{
		int nPID;
		int nValue;
		int nPrevIndex;
		int nNextIndex;
	};

	struct _ST_SEM_DATA
	{
		char szName[MAX_SEM_NAME];
		int nFirstBlockIndex;
		int nOwnerPID;
	};

	struct _ST_SEM_BASE_DATA
	{
		int nEnterPID;
	};

	class CGlobalSemaphoreMap
	{
		struct Owner
		{
			CGlobalSemaphoreMap& m_Instance;
			int m_nPID;

			Owner(CGlobalSemaphoreMap* pThis, int nPID);
			~Owner(void);
		};

		CGlobalSemaphoreMap(void);
		~CGlobalSemaphoreMap(void);

	public:
		HANDLE m_hSharedMem;
		LPBYTE	m_pMemory;
		size_t	m_tMemorySize;

		_ST_SEM_BASE_DATA*	m_pBaseData;
		_ST_SEM_DATA*		m_pSemDataArr;
		_ST_SEM_REFCOUNT*	m_pRefCountArr;

	public:
		HANDLE Create(LPCSTR pszName, int nPID);
		std::string Destroy(HANDLE hSemaphore);

		bool Refresh(LPCSTR pszName);
		void Remove(int nPID);

		void MarkOwner(HANDLE hSemaphore);
		void UnmarkOwner(HANDLE hSemaphore);
		bool CheckAbandoned(HANDLE hSemaphore);

		bool IsEmpty(void);

	private:
		bool RemoveWorker(_ST_SEM_DATA* pSemData, int nPID);

		_ST_SEM_DATA* NewSemData(LPCSTR pszName, int nPID);
		_ST_SEM_DATA* FindSemData(LPCSTR pszName);
		void DeleteSemData(_ST_SEM_DATA* pTargetSem);

		int NewRefCount(int nPID);
		_ST_SEM_REFCOUNT* NewRefCount(int nFirstBlockIndex, int nPID);
		_ST_SEM_REFCOUNT* FindRefCount(int nFirstBlockIndex, int nPID);
		void DeleteRefCount(_ST_SEM_DATA* pSemData, _ST_SEM_REFCOUNT* pRefCount);	// returns first block index

	public:
		static CGlobalSemaphoreMap* GetInstance(void)
		{
			static CGlobalSemaphoreMap instance;
			return &instance;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	inline CGlobalSemaphoreMap* GlobalSemMap(void)
	{
		return CGlobalSemaphoreMap::GetInstance();
	}
}
