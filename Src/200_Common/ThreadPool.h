#pragma once

#include <vector>
#include "SafeQueue.h"

namespace core
{
	class CThreadPool;
	namespace internal
	{
		struct ST_THREADPOOL_DATA
		{
			std::tstring strName;
			HANDLE	hThread;
			HANDLE	hEvent;
			bool*	pbEscapeFlag;

			CThreadPool* pOwner;

			int		(*pfThreadFunc)(void *pContext);
			void*	pContext;

			ST_THREADPOOL_DATA(void)
				: strName(), hThread(NULL), hEvent(NULL)
				, pbEscapeFlag(NULL), pOwner(NULL)
				, pfThreadFunc(NULL), pContext(NULL)
			{}
		};
	}

	typedef internal::ST_THREADPOOL_DATA*		HTHREADPOOL;
	
	class CThreadPool
	{
		std::vector<internal::ST_THREADPOOL_DATA> m_vecInstance;
		CSafeQueue<internal::ST_THREADPOOL_DATA*> m_queReady;

		bool			m_bEscapeFlag;
		bool			m_bReserved[7];

	public:
						CThreadPool();
		virtual			~CThreadPool();

		ECODE			Init(size_t tPoolCount, E_THREAD_PRIORITY nPriority = THREAD_PRIORITY_MIDDLE);
		void			Destroy(void);

		ECODE			CreateThread(int(*pfEntry)(void* pContext), void* pContext);

	private:
		friend int		ThreadPoolWorker(void* pContext);
		void			ReturnThread(HTHREADPOOL hThread);
	};
}
