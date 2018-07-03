#pragma once

namespace core
{
	struct INTERNAL_COMMON_THREAD_DATA
	{
		virtual ~INTERNAL_COMMON_THREAD_DATA(void)	{}
		virtual int ThreadFunc(void) = 0;
	};

	template<typename T>
	struct INTERNAL_TEMPLATE_THREAD_DATA : public INTERNAL_COMMON_THREAD_DATA
	{
		T* pInstance;
		int (T::*pfMemberFunc)(void* pContext);
		void* pContext;
		int ThreadFunc(void)	{	return (pInstance->*pfMemberFunc)(pContext);	}
	};

	int InternalThreadCaller(void* pContext);

	template<typename T>
	HANDLE CreateThread(T* pInstance, int (T::*pfEntry)(void*), void* pContext, E_THREAD_PRIORITY nPriority = THREAD_PRIORITY_MIDDLE)
	{
		INTERNAL_TEMPLATE_THREAD_DATA<T>* pThreadData = new(std::nothrow) INTERNAL_TEMPLATE_THREAD_DATA<T>;
		if( NULL == pThreadData )
			return NULL;
		pThreadData->pInstance = pInstance;
		pThreadData->pfMemberFunc = pfEntry;
		pThreadData->pContext = pContext;
		return CreateThread(InternalThreadCaller, pThreadData, nPriority);
	}

	template<typename T>
	HANDLE CreateClassMethodThread(T* pInstance, int (T::*pfEntry)(void*), void* pContext, E_THREAD_PRIORITY nPriority = THREAD_PRIORITY_MIDDLE)
	{
		INTERNAL_TEMPLATE_THREAD_DATA<T>* pThreadData = new(std::nothrow) INTERNAL_TEMPLATE_THREAD_DATA<T>;
		if( NULL == pThreadData )
			return NULL;
		pThreadData->pInstance = pInstance;
		pThreadData->pfMemberFunc = pfEntry;
		pThreadData->pContext = pContext;
		return CreateThread(InternalThreadCaller, pThreadData, nPriority);
	}
}
