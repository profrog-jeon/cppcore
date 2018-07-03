#pragma once

namespace core
{
	template <typename T>
	T				InterlockedAnd_(T volatile* pDest, T Value);			// Returns old value
	template <typename T>
	T				InterlockedOr_(T volatile* pDest, T Value);				// Returns old value
	template <typename T>
	T				InterlockedXor_(T volatile* pDest, T Value);			// Returns old value
	template <typename T>
	T				InterlockedIncrement_(T volatile* pDest);				// Returns new value
	template <typename T>
	T				InterlockedDecrement_(T volatile* pDest);				// Returns new value
	template <typename T>
	T				InterlockedExchange_(T volatile* pDest, T Exchange);	// Returns old value
	template <typename T>
	T				InterlockedExchangeAdd_(T volatile* pDest, T nValue);	// Returns new value
	template <typename T>
	T				InterlockedCompareExchange_(T volatile* pDest, T Exchange, T Comperand);	// Returns old Destination

	void*			InterlockedExchangePointer_(void* volatile* pDest, void* pExchange);	// Returns old Target
	void*			InterlockedCompareExchangePointer_(void* volatile* pDest, void* pExchange, void* pComperand);	// Returns old Destination
}