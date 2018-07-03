#pragma once

#include <queue>
#include "CriticalSection.h"

namespace core
{
	template<typename T>
	class CSafeQueue : private Uncopyable
	{
		CCriticalSection m_csData;
		std::queue<T> m_queData;

	public:
		CSafeQueue(void) : m_csData(), m_queData()		{}
		~CSafeQueue(void)								{}

		void Clear(void)
		{
			CCriticalSection::Owner Lock(m_csData);
			while(!m_queData.empty())
				m_queData.pop();
		}

		bool IsEmpty(void)
		{
			CCriticalSection::Owner Lock(m_csData);
			return m_queData.empty();
		}

		size_t Count(void)
		{
			CCriticalSection::Owner Lock(m_csData);
			return m_queData.size();
		}

		ECODE Push(const T& data)
		{
			CCriticalSection::Owner Lock(m_csData);
			m_queData.push(data);
			return EC_SUCCESS;
		}

		ECODE Pop(T& outItem)
		{
			CCriticalSection::Owner Lock(m_csData);
			if( m_queData.empty() )
				return EC_NO_DATA;

			outItem = m_queData.front();
			m_queData.pop();
			return EC_SUCCESS;
		}
	};

}
