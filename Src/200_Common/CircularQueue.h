#pragma once

#include "../100_System/InterlockedOperation.h"
#include <vector>

namespace core
{
	template<typename T>
	class CCircularQueue
	{
		std::vector<T> m_vecItems;
		DWORD m_dwFrontIndex;
		DWORD m_dwBackIndex;
		DWORD m_dwReadIndex;
		DWORD m_dwWriteIndex;
		DWORD m_dwMaxCapacity;
		DWORD m_dwMask;

	public:
		CCircularQueue(DWORD dwSeedIndex = 0)
			: m_vecItems()
			, m_dwFrontIndex(dwSeedIndex)
			, m_dwBackIndex(dwSeedIndex)
			, m_dwReadIndex(dwSeedIndex)
			, m_dwWriteIndex(dwSeedIndex)
			, m_dwMaxCapacity(0xFFFF+1)
			, m_dwMask(0xFFFF)
		{
			m_vecItems.resize(m_dwMaxCapacity);
		}

		~CCircularQueue(void)
		{
		}

		void Init(size_t tSize, DWORD dwSeedIndex = 0)
		{
			m_dwFrontIndex = dwSeedIndex;
			m_dwBackIndex = dwSeedIndex;
			m_dwReadIndex = dwSeedIndex;
			m_dwWriteIndex = dwSeedIndex;

			size_t i;
			for(i=4; i<32; i++)
			{
				m_dwMaxCapacity = 0x01 << i;
				if( tSize <= m_dwMaxCapacity )
					break;
			}

			m_dwMask = m_dwMaxCapacity - 1;
			m_vecItems.resize(m_dwMaxCapacity);
		}

		DWORD Count(void)
		{
			DWORD dwFrontIndex = m_dwFrontIndex;
			DWORD dwBackIndex = m_dwBackIndex;

			// overflow safe
			return dwBackIndex - dwFrontIndex;
		}

		ECODE Push(const T& newItem)
		{
			DWORD dwEmptyIndex = -1;
			while(1)
			{
				DWORD dwBackIndex = m_dwBackIndex;
				DWORD dwFrontIndex = m_dwFrontIndex;

				// overflow safe
				DWORD dwItemCount = dwBackIndex - dwFrontIndex;
				if( dwItemCount == m_dwMaxCapacity )
					return EC_DATA_FULL;

				dwEmptyIndex = InterlockedCompareExchange_(&m_dwBackIndex, dwBackIndex+1, dwBackIndex);

				// some thread has moved BackIndex already
				if( dwEmptyIndex != dwBackIndex )
					continue;

				break;
			}

			m_vecItems[dwEmptyIndex & m_dwMask] = newItem;
			while(dwEmptyIndex != InterlockedCompareExchange_(&m_dwWriteIndex, dwEmptyIndex+1, dwEmptyIndex));

			return EC_SUCCESS;
		}

		ECODE Pop(T& outItem)
		{
			DWORD dwItemIndex = -1;
			while(1)
			{
				DWORD dwReadIndex = m_dwReadIndex;
				DWORD dwWriteIndex = m_dwWriteIndex;

				// overflow safe
				DWORD dwItemCount = dwWriteIndex - dwReadIndex;
				if( 0 == dwItemCount )
					return EC_NO_DATA;

				dwItemIndex = InterlockedCompareExchange_(&m_dwReadIndex, dwReadIndex+1, dwReadIndex);

				// some thread has moved BackIndex already
				if( dwItemIndex != dwReadIndex )
					continue;

				break;
			}

			outItem = m_vecItems[dwItemIndex & m_dwMask];
			while(dwItemIndex != InterlockedCompareExchange_(&m_dwFrontIndex, dwItemIndex+1, dwItemIndex));

			return EC_SUCCESS;
		}
	};
}
