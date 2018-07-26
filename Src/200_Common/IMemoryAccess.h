#pragma once

#include <string.h>

namespace core
{
	struct IMemoryAccess
	{
		template <typename T>
		T Cast(size_t tOffset = 0)
		{
			return (T)(Ptr() + tOffset);
		}

		template <typename T>
		ECODE Query(size_t tOffset, T* pOut)
		{
			LPBYTE pPtr = Ptr();
			if( NULL == pPtr )
				return EC_INVALID_DATA;

			if( Size() < (tOffset + sizeof(T)) )
				return EC_NO_DATA;

			memcpy(pOut, pPtr + tOffset, sizeof(T));
			return EC_SUCCESS;
		}

		template <typename T>
		T* ArrayAt(size_t tIndex)
		{
			T* pArray = (T*)Ptr();
			if( NULL == pArray )
				return NULL;

			if( Size() < ((tIndex+1) * sizeof(T)) )
				return NULL;

			return pArray + tIndex;
		}

		template <typename T>
		T* Read(size_t& tPos)
		{
			LPBYTE pPtr = Ptr();
			if( NULL == pPtr )
				return NULL;

			if( Size() < (tPos + sizeof(T)) )
				return NULL;

			T* pRet = (T*)(pPtr + tPos);
			tPos += sizeof(T);
			return pRet;
		}

		ECODE Read(size_t& tPos, void* pDest, size_t tSize)
		{
			LPBYTE pPtr = Ptr();
			if( NULL == pPtr )
				return EC_NO_DATA;

			if( Size() < (tPos + tSize) )
				return EC_NO_DATA;

			memcpy(pDest, pPtr + tPos, tSize);
			tPos += tSize;
			return EC_SUCCESS;
		}

		virtual LPBYTE Ptr(void) = 0;
		virtual size_t Size(void) = 0;
	};
}