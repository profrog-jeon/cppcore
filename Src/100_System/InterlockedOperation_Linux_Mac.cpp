#include "stdafx.h"
#include "InterlockedOperation.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	template<> char InterlockedAnd_(char volatile* pDest, char Exchange)
	{	return __sync_fetch_and_and(pDest, Exchange);	}

	template<> short InterlockedAnd_(short volatile* pDest, short Exchange)
	{	return __sync_fetch_and_and(pDest, Exchange);	}

	template<> int InterlockedAnd_(int volatile* pDest, int Exchange)
	{	return __sync_fetch_and_and(pDest, Exchange);	}

	template<> int64_t InterlockedAnd_(int64_t volatile* pDest, int64_t Exchange)
	{	return __sync_fetch_and_and(pDest, Exchange);	}

	template<> BYTE InterlockedAnd_(BYTE volatile* pDest, BYTE Exchange)
	{	return __sync_fetch_and_and(pDest, Exchange);	}

	template<> WORD InterlockedAnd_(WORD volatile* pDest, WORD Exchange)
	{	return __sync_fetch_and_and(pDest, Exchange);	}

	template<> DWORD InterlockedAnd_(DWORD volatile* pDest, DWORD Exchange)
	{	return __sync_fetch_and_and(pDest, Exchange);	}

	template<> QWORD InterlockedAnd_(QWORD volatile* pDest, QWORD Exchange)
	{	return __sync_fetch_and_and(pDest, Exchange);	}


	//////////////////////////////////////////////////////////////////////////
	template<> char InterlockedOr_(char volatile* pDest, char Exchange)
	{	return __sync_fetch_and_or(pDest, Exchange);	}

	template<> short InterlockedOr_(short volatile* pDest, short Exchange)
	{	return __sync_fetch_and_or(pDest, Exchange);	}

	template<> int InterlockedOr_(int volatile* pDest, int Exchange)
	{	return __sync_fetch_and_or(pDest, Exchange);	}

	template<> int64_t InterlockedOr_(int64_t volatile* pDest, int64_t Exchange)
	{	return __sync_fetch_and_or(pDest, Exchange);	}

	template<> BYTE InterlockedOr_(BYTE volatile* pDest, BYTE Exchange)
	{	return __sync_fetch_and_or(pDest, Exchange);	}

	template<> WORD InterlockedOr_(WORD volatile* pDest, WORD Exchange)
	{	return __sync_fetch_and_or(pDest, Exchange);	}

	template<> DWORD InterlockedOr_(DWORD volatile* pDest, DWORD Exchange)
	{	return __sync_fetch_and_or(pDest, Exchange);	}

	template<> QWORD InterlockedOr_(QWORD volatile* pDest, QWORD Exchange)
	{	return __sync_fetch_and_or(pDest, Exchange);	}


	//////////////////////////////////////////////////////////////////////////
	template<> char InterlockedXor_(char volatile* pDest, char Exchange)
	{	return __sync_fetch_and_xor(pDest, Exchange);	}

	template<> short InterlockedXor_(short volatile* pDest, short Exchange)
	{	return __sync_fetch_and_xor(pDest, Exchange);	}

	template<> int InterlockedXor_(int volatile* pDest, int Exchange)
	{	return __sync_fetch_and_xor(pDest, Exchange);	}

	template<> int64_t InterlockedXor_(int64_t volatile* pDest, int64_t Exchange)
	{	return __sync_fetch_and_xor(pDest, Exchange);	}

	template<> BYTE InterlockedXor_(BYTE volatile* pDest, BYTE Exchange)
	{	return __sync_fetch_and_xor(pDest, Exchange);	}

	template<> WORD InterlockedXor_(WORD volatile* pDest, WORD Exchange)
	{	return __sync_fetch_and_xor(pDest, Exchange);	}

	template<> DWORD InterlockedXor_(DWORD volatile* pDest, DWORD Exchange)
	{	return __sync_fetch_and_xor(pDest, Exchange);	}

	template<> QWORD InterlockedXor_(QWORD volatile* pDest, QWORD Exchange)
	{	return __sync_fetch_and_xor(pDest, Exchange);	}


	//////////////////////////////////////////////////////////////////////////
	template<> char InterlockedIncrement_(char volatile* pDest)
	{	return __sync_add_and_fetch(pDest, 1);	}

	template<> short InterlockedIncrement_(short volatile* pDest)
	{	return __sync_add_and_fetch(pDest, 1);	}

	template<> int InterlockedIncrement_(int volatile* pDest)
	{	return __sync_add_and_fetch(pDest, 1);	}

	template<> int64_t InterlockedIncrement_(int64_t volatile* pDest)
	{	return __sync_add_and_fetch(pDest, 1);	}

	template<> BYTE InterlockedIncrement_(BYTE volatile* pDest)
	{	return __sync_add_and_fetch(pDest, 1);	}

	template<> WORD InterlockedIncrement_(WORD volatile* pDest)
	{	return __sync_add_and_fetch(pDest, 1);	}

	template<> DWORD InterlockedIncrement_(DWORD volatile* pDest)
	{	return __sync_add_and_fetch(pDest, 1);	}

	template<> QWORD InterlockedIncrement_(QWORD volatile* pDest)
	{	return __sync_add_and_fetch(pDest, 1);	}


	//////////////////////////////////////////////////////////////////////////
	template<> char InterlockedDecrement_(char volatile* pDest)
	{	return __sync_sub_and_fetch(pDest, 1);	}

	template<> short InterlockedDecrement_(short volatile* pDest)
	{	return __sync_sub_and_fetch(pDest, 1);	}

	template<> int InterlockedDecrement_(int volatile* pDest)
	{	return __sync_sub_and_fetch(pDest, 1);	}

	template<> int64_t InterlockedDecrement_(int64_t volatile* pDest)
	{	return __sync_sub_and_fetch(pDest, 1);	}

	template<> BYTE InterlockedDecrement_(BYTE volatile* pDest)
	{	return __sync_sub_and_fetch(pDest, 1);	}

	template<> WORD InterlockedDecrement_(WORD volatile* pDest)
	{	return __sync_sub_and_fetch(pDest, 1);	}

	template<> DWORD InterlockedDecrement_(DWORD volatile* pDest)
	{	return __sync_sub_and_fetch(pDest, 1);	}

	template<> QWORD InterlockedDecrement_(QWORD volatile* pDest)
	{	return __sync_sub_and_fetch(pDest, 1);	}


	//////////////////////////////////////////////////////////////////////////
	template<> char InterlockedExchange_(char volatile* pDest, char Exchange)
	{	return __sync_lock_test_and_set(pDest, Exchange);	}

	template<> short InterlockedExchange_(short volatile* pDest, short Exchange)
	{	return __sync_lock_test_and_set(pDest, Exchange);	}

	template<> int InterlockedExchange_(int volatile* pDest, int Exchange)
	{	return __sync_lock_test_and_set(pDest, Exchange);	}

	template<> int64_t InterlockedExchange_(int64_t volatile* pDest, int64_t Exchange)
	{	return __sync_lock_test_and_set(pDest, Exchange);	}

	template<> BYTE InterlockedExchange_(BYTE volatile* pDest, BYTE Exchange)
	{	return __sync_lock_test_and_set(pDest, Exchange);	}
	
	template<> WORD InterlockedExchange_(WORD volatile* pDest, WORD Exchange)
	{	return __sync_lock_test_and_set(pDest, Exchange);	}

	template<> DWORD InterlockedExchange_(DWORD volatile* pDest, DWORD Exchange)
	{	return __sync_lock_test_and_set(pDest, Exchange);	}

	template<> QWORD InterlockedExchange_(QWORD volatile* pDest, QWORD Exchange)
	{	return __sync_lock_test_and_set(pDest, Exchange);	}


	//////////////////////////////////////////////////////////////////////////
	template<> char InterlockedExchangeAdd_(char volatile* pDest, char Value)
	{	return __sync_add_and_fetch(pDest, Value);	}

	template<> short InterlockedExchangeAdd_(short volatile* pDest, short Value)
	{	return __sync_add_and_fetch(pDest, Value);	}
	
	template<> int InterlockedExchangeAdd_(int volatile* pDest, int Value)
	{	return __sync_add_and_fetch(pDest, Value);	}

	template<> int64_t InterlockedExchangeAdd_(int64_t volatile* pDest, int64_t Value)
	{	return __sync_add_and_fetch(pDest, Value);	}

	template<> BYTE InterlockedExchangeAdd_(BYTE volatile* pDest, BYTE Value)
	{	return __sync_add_and_fetch(pDest, Value);	}

	template<> WORD InterlockedExchangeAdd_(WORD volatile* pDest, WORD Value)
	{	return __sync_add_and_fetch(pDest, Value);	}

	template<> DWORD InterlockedExchangeAdd_(DWORD volatile* pDest, DWORD Value)
	{	return __sync_add_and_fetch(pDest, Value);	}

	template<> QWORD InterlockedExchangeAdd_(QWORD volatile* pDest, QWORD Value)
	{	return __sync_add_and_fetch(pDest, Value);	}


	//////////////////////////////////////////////////////////////////////////
	template<> char InterlockedCompareExchange_(char volatile* pDest, char Exchange, char Comperand)
	{	return __sync_val_compare_and_swap(pDest, Comperand, Exchange);	}
	
	template<> short InterlockedCompareExchange_(short volatile* pDest, short Exchange, short Comperand)
	{	return __sync_val_compare_and_swap(pDest, Comperand, Exchange);	}

	template<> int InterlockedCompareExchange_(int volatile* pDest, int Exchange, int Comperand)
	{	return __sync_val_compare_and_swap(pDest, Comperand, Exchange);	}

	template<> int64_t InterlockedCompareExchange_(int64_t volatile* pDest, int64_t Exchange, int64_t Comperand)
	{	return __sync_val_compare_and_swap(pDest, Comperand, Exchange);	}

	template<> BYTE InterlockedCompareExchange_(BYTE volatile* pDest, BYTE Exchange, BYTE Comperand)
	{	return __sync_val_compare_and_swap(pDest, Comperand, Exchange);	}

	template<> WORD InterlockedCompareExchange_(WORD volatile* pDest, WORD Exchange, WORD Comperand)
	{	return __sync_val_compare_and_swap(pDest, Comperand, Exchange);	}

	template<> DWORD InterlockedCompareExchange_(DWORD volatile* pDest, DWORD Exchange, DWORD Comperand)
	{	return __sync_val_compare_and_swap(pDest, Comperand, Exchange);	}

	template<> QWORD InterlockedCompareExchange_(QWORD volatile* pDest, QWORD Exchange, QWORD Comperand)
	{	return __sync_val_compare_and_swap(pDest, Comperand, Exchange);	}


	//////////////////////////////////////////////////////////////////////////
	void* InterlockedExchangePointer_(void* volatile* pDest, void* pExchange)
	{	return __sync_lock_test_and_set(pDest, pExchange);	}


	//////////////////////////////////////////////////////////////////////////
	void* InterlockedCompareExchangePointer_(void* volatile* pDest, void* pExchange, void* pComperand)
	{	return __sync_val_compare_and_swap(pDest, pComperand, pExchange);	}
}