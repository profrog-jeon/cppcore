#include "stdafx.h"
#include <Windows.h>
#include <intrin.h>
#include "InterlockedOperation.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	template<> char InterlockedAnd_(char volatile* pDest, char Value)
	{	return _InterlockedAnd8(pDest, Value);	}

	template<> short InterlockedAnd_(short volatile* pDest, short Value)
	{	return _InterlockedAnd16(pDest, Value);	}

	template<> int InterlockedAnd_(int volatile* pDest, int Value)
	{	return _InterlockedAnd((long*)pDest, Value);	}

	template<> int64_t InterlockedAnd_(int64_t volatile* pDest, int64_t Value)
	{	return InterlockedAnd64(pDest, Value);	}

	template<> BYTE InterlockedAnd_(BYTE volatile* pDest, BYTE Value)
	{	return _InterlockedAnd8((char*)pDest, Value);	}

	template<> WORD InterlockedAnd_(WORD volatile* pDest, WORD Value)
	{	return _InterlockedAnd16((short*)pDest, Value);	}

	template<> DWORD InterlockedAnd_(DWORD volatile* pDest, DWORD Value)
	{	return _InterlockedAnd((long*)pDest, Value);	}

	template<> QWORD InterlockedAnd_(QWORD volatile* pDest, QWORD Value)
	{	return InterlockedAnd64((int64_t*)pDest, Value);	}


	//////////////////////////////////////////////////////////////////////////
	template<> char InterlockedOr_(char volatile* pDest, char Value)
	{	return _InterlockedOr8(pDest, Value);	}

	template<> short InterlockedOr_(short volatile* pDest, short Value)
	{	return _InterlockedOr16(pDest, Value);	}

	template<> int InterlockedOr_(int volatile* pDest, int Value)
	{	return _InterlockedOr((long*)pDest, Value);	}

	template<> int64_t InterlockedOr_(int64_t volatile* pDest, int64_t Value)
	{	return InterlockedOr64(pDest, Value);	}

	template<> BYTE InterlockedOr_(BYTE volatile* pDest, BYTE Value)
	{	return _InterlockedOr8((char*)pDest, Value);	}

	template<> WORD InterlockedOr_(WORD volatile* pDest, WORD Value)
	{	return _InterlockedOr16((short*)pDest, Value);	}

	template<> DWORD InterlockedOr_(DWORD volatile* pDest, DWORD Value)
	{	return _InterlockedOr((long*)pDest, Value);	}

	template<> QWORD InterlockedOr_(QWORD volatile* pDest, QWORD Value)
	{	return InterlockedOr64((int64_t*)pDest, Value);	}


	//////////////////////////////////////////////////////////////////////////
	template<> char InterlockedXor_(char volatile* pDest, char Value)
	{	return _InterlockedXor8(pDest, Value);	}

	template<> short InterlockedXor_(short volatile* pDest, short Value)
	{	return _InterlockedXor16(pDest, Value);	}

	template<> int InterlockedXor_(int volatile* pDest, int Value)
	{	return _InterlockedXor((long*)pDest, Value);	}

	template<> int64_t InterlockedXor_(int64_t volatile* pDest, int64_t Value)
	{	return InterlockedXor64(pDest, Value);	}

	template<> BYTE InterlockedXor_(BYTE volatile* pDest, BYTE Value)
	{	return _InterlockedXor8((char*)pDest, Value);	}

	template<> WORD InterlockedXor_(WORD volatile* pDest, WORD Value)
	{	return _InterlockedXor16((short*)pDest, Value);	}

	template<> DWORD InterlockedXor_(DWORD volatile* pDest, DWORD Value)
	{	return _InterlockedXor((long*)pDest, Value);	}

	template<> QWORD InterlockedXor_(QWORD volatile* pDest, QWORD Value)
	{	return InterlockedXor64((int64_t*)pDest, Value);	}


	//////////////////////////////////////////////////////////////////////////
	template<> short InterlockedIncrement_(short volatile* pDest)
	{	return _InterlockedIncrement16(pDest);	}

	template<> int InterlockedIncrement_(int volatile* pDest)
	{	return InterlockedIncrement((long*)pDest);	}

	template<> int64_t InterlockedIncrement_(int64_t volatile* pDest)
	{	return InterlockedIncrement64(pDest);	}

	template<> WORD InterlockedIncrement_(WORD volatile* pDest)
	{	return _InterlockedIncrement16((short*)pDest);	}

	template<> DWORD InterlockedIncrement_(DWORD volatile* pDest)
	{	return InterlockedIncrement((long*)pDest);	}

	template<> QWORD InterlockedIncrement_(QWORD volatile* pDest)
	{	return InterlockedIncrement64((int64_t*)pDest);	}


	//////////////////////////////////////////////////////////////////////////
	template<> short InterlockedDecrement_(short volatile* pDest)
	{	return _InterlockedDecrement16(pDest);	}

	template<> int InterlockedDecrement_(int volatile* pDest)
	{	return InterlockedDecrement((long*)pDest);	}

	template<> int64_t InterlockedDecrement_(int64_t volatile* pDest)
	{	return InterlockedDecrement64(pDest);	}

	template<> WORD InterlockedDecrement_(WORD volatile* pDest)
	{	return _InterlockedDecrement16((short*)pDest);	}

	template<> DWORD InterlockedDecrement_(DWORD volatile* pDest)
	{	return InterlockedDecrement((long*)pDest);	}

	template<> QWORD InterlockedDecrement_(QWORD volatile* pDest)
	{	return InterlockedDecrement64((int64_t*)pDest);	}


	//////////////////////////////////////////////////////////////////////////
	template<> int InterlockedExchange_(int volatile* pDest, int Exchange)
	{	return InterlockedExchange((long*)pDest, Exchange);	}

	template<> int64_t InterlockedExchange_(int64_t volatile* pDest, int64_t Exchange)
	{	return InterlockedExchange64(pDest, Exchange);	}

	template<> DWORD InterlockedExchange_(DWORD volatile* pDest, DWORD Exchange)
	{	return InterlockedExchange((long*)pDest, Exchange);	}

	template<> QWORD InterlockedExchange_(QWORD volatile* pDest, QWORD Exchange)
	{	return InterlockedExchange64((int64_t*)pDest, Exchange);	}


	//////////////////////////////////////////////////////////////////////////
	template<> int InterlockedExchangeAdd_(int volatile* pDest, int Value)
	{	return InterlockedExchangeAdd((long*)pDest, Value);	}

	template<> int64_t InterlockedExchangeAdd_(int64_t volatile* pDest, int64_t Value)
	{	return InterlockedExchangeAdd64(pDest, Value);	}

	template<> DWORD InterlockedExchangeAdd_(DWORD volatile* pDest, DWORD Value)
	{	return InterlockedExchangeAdd((long*)pDest, Value);	}

	template<> QWORD InterlockedExchangeAdd_(QWORD volatile* pDest, QWORD Value)
	{	return InterlockedExchangeAdd64((int64_t*)pDest, Value);	}


	//////////////////////////////////////////////////////////////////////////
	template<> short InterlockedCompareExchange_(short volatile* pDest, short Exchange, short Comperand)
	{	return InterlockedCompareExchange16(pDest, Exchange, Comperand);	}

	template<> int InterlockedCompareExchange_(int volatile* pDest, int Exchange, int Comperand)
	{	return InterlockedCompareExchange((long*)pDest, Exchange, Comperand);	}

	template<> int64_t InterlockedCompareExchange_(int64_t volatile* pDest, int64_t Exchange, int64_t Comperand)
	{	return InterlockedCompareExchange64(pDest, Exchange, Comperand);	}

	template<> WORD InterlockedCompareExchange_(WORD volatile* pDest, WORD Exchange, WORD Comperand)
	{	return InterlockedCompareExchange16((short*)pDest, Exchange, Comperand);	}

	template<> DWORD InterlockedCompareExchange_(DWORD volatile* pDest, DWORD Exchange, DWORD Comperand)
	{	return InterlockedCompareExchange((long*)pDest, Exchange, Comperand);	}

	template<> QWORD InterlockedCompareExchange_(QWORD volatile* pDest, QWORD Exchange, QWORD Comperand)
	{	return InterlockedCompareExchange64((int64_t*)pDest, Exchange, Comperand);	}


	//////////////////////////////////////////////////////////////////////////
	void* InterlockedExchangePointer_(void* volatile* pDest, void* pExchange)
	{	return InterlockedExchangePointer(pDest, pExchange);	}


	//////////////////////////////////////////////////////////////////////////
	void* InterlockedCompareExchangePointer_(void* volatile* pDest, void* pExchange, void* pComperand)
	{	return InterlockedCompareExchangePointer(pDest, pExchange, pComperand);	}
}