#pragma once

#ifdef DEBUG_LINUX
#define PRINT_DEBUG	printf
#else
#define PRINT_DEBUG(...)
#endif

namespace core
{
	template<typename T>
	class CSharedPtr
	{
		T* m_pInstance;
		int* m_pnReferenceCount;

	public:
		CSharedPtr(void)
			: m_pInstance(NULL)
			, m_pnReferenceCount(NULL)
		{
		}

		CSharedPtr(T* pInstance)
			: m_pInstance(pInstance)
			, m_pnReferenceCount(NULL)
		{
			m_pnReferenceCount = new int(1);
			PRINT_DEBUG("m_pnReferenceCount=%d\n", *m_pnReferenceCount);
		}

		CSharedPtr(const CSharedPtr& other)
			: m_pInstance(other.m_pInstance)
			, m_pnReferenceCount(other.m_pnReferenceCount)
		{
			__sync_add_and_fetch(m_pnReferenceCount, 1);
			PRINT_DEBUG("m_pnReferenceCount=%d\n", *m_pnReferenceCount);
		}

		~CSharedPtr(void)
		{
			if( m_pnReferenceCount && (0 == __sync_sub_and_fetch(m_pnReferenceCount, 1)) )
			{
				PRINT_DEBUG("m_pnReferenceCount=%d 1\n", *m_pnReferenceCount);
				delete m_pInstance;
				delete m_pnReferenceCount;
			}
			else if( m_pnReferenceCount )
			{
				PRINT_DEBUG("m_pnReferenceCount=%d 2\n", *m_pnReferenceCount);
			}
		}

		CSharedPtr& operator=(const CSharedPtr& other)
		{
			m_pInstance = other.m_pInstance;
			m_pnReferenceCount = other.m_pnReferenceCount;
			__sync_add_and_fetch(m_pnReferenceCount, 1);
			PRINT_DEBUG("m_pnReferenceCount=%d\n", *m_pnReferenceCount);
			return *this;
		}

		T* operator->() const
		{
			return m_pInstance;
		}
	};
}
