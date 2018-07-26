#pragma once

#include <queue>
#include "CriticalSection.h"

namespace core
{
	template<typename T>
	class CSyncQueue
	{
		HANDLE				m_hSemaphore;
		CCriticalSection	m_csItems;
		std::queue<T>		m_queItems;

	public:
		CSyncQueue(void)
			: m_hSemaphore(NULL)
			, m_csItems()
			, m_queItems()
		{			
			Create();
		}

		~CSyncQueue(void)
		{
			Destroy();
		}

		ECODE Create(void)
		{
			if( m_hSemaphore )
				return EC_SUCCESS;

			m_hSemaphore = CreateSemaphore(0);
			if( NULL == m_hSemaphore )
			{
				Log_Error("CreateSemaphore failure in TSyncQueue");
				return GetLastError();
			}

			return EC_SUCCESS;
		}

		void Destroy(void)
		{
			if( NULL == m_hSemaphore )
				return;

			CloseSemaphore(m_hSemaphore);
			m_hSemaphore = NULL;
		}

		bool IsEmpty(void)
		{
			return m_queItems.empty();
		}

		void Clear(void)
		{
			CCriticalSection::Owner Lock(m_csItems);
			while(!m_queItems.empty())
				m_queItems.pop();
		}

		void Push(T inItem)
		{
			m_csItems.Enter();
			m_queItems.push(inItem);
			m_csItems.Leave();
			
			ReleaseSemaphore(m_hSemaphore);
		}

		EWAIT Pop(T* pOutItem)
		{
			EWAIT nRet;
			while( WAIT_TIMEOUT_ == (nRet = WaitForSemaphore(m_hSemaphore, 500)) );

			if( nRet != WAIT_OBJECT_0_ )
				return nRet;

			m_csItems.Enter();
			(*pOutItem) = m_queItems.front();
			m_queItems.pop();
			m_csItems.Leave();
			return WAIT_OBJECT_0_;
		}
	};
}
