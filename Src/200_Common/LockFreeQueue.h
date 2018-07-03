#pragma once

#include "../100_System/InterlockedOperation.h"
#include <vector>

namespace core
{
	template<typename T>
	class CLockFreeQueue
	{
		struct ItemLink
		{
			T Item;
			ItemLink* pNext;
			ItemLink() : Item(), pNext(NULL)	{}
		};

		ItemLink*		m_pFrontLink;
		ItemLink*		m_pBackLink;

	public:
		CLockFreeQueue(void)
			: m_pFrontLink(NULL)
			, m_pBackLink(NULL)
		{
			m_pFrontLink = m_pBackLink = new ItemLink();
		}

		~CLockFreeQueue(void)
		{
			Clear();
			delete m_pBackLink;
			m_pFrontLink = m_pBackLink = NULL;
		}

		void Clear(void)
		{
			while(m_pFrontLink != m_pBackLink)
			{
				ItemLink* pCur = m_pFrontLink;
				m_pFrontLink = m_pFrontLink->pNext;
				delete pCur;
			}
		}

		ECODE Push(const T& newItem)
		{
			ItemLink* pNewLink = new(std::nothrow) ItemLink();
			if( NULL == pNewLink )
				return EC_NOT_ENOUGH_MEMORY;

			ItemLink* pBackLink;
			while(1)
			{
				pBackLink = m_pBackLink;
				if( InterlockedCompareExchangePointer_((void**)&pBackLink->pNext, pNewLink, NULL) )
					continue;

				break;
			}

			pBackLink->Item = newItem;
			InterlockedExchangePointer_((void**)&m_pBackLink, pNewLink);
			return EC_SUCCESS;
		}

		ECODE Pop(T& outItem)
		{
			ItemLink* pFrontLink;
			while(1)
			{
				pFrontLink = m_pFrontLink;

				InterlockedCompareExchangePointer_((void**)&pFrontLink, NULL, m_pBackLink);
				if( NULL == pFrontLink )
					return EC_NO_DATA;

				ItemLink* pFront = (ItemLink*)InterlockedCompareExchangePointer_((void**)&m_pFrontLink, m_pFrontLink->pNext, pFrontLink);
				if( pFront != pFrontLink )
					continue;

				break;
			}

			outItem = pFrontLink->Item;
			delete pFrontLink;
			return EC_SUCCESS;
		}
	};
}
