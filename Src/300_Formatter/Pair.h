#pragma once

#include <string>
#include <map>
#include <vector>
#include <list>

#include "PairSuper.h"
#include "ValuePtrHolder.h"

#ifdef UNICODE
#define sPair			sPairW
#define sPairT			sPairW
#else
#define sPair			sPairA
#define sPairT			sPairA
#endif

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	struct sPairA : public sPairSuperA
	{
		template<typename T>
		sPairA(T& inValue) : sPairSuperA(NULL, "")
		{
			pValueHolder = NewValuePtrHolderA(&inValue);
		}

		template<typename T>
		sPairA(const char* pszKey, T& inValue)
			: sPairSuperA(NULL, pszKey)
		{
			pValueHolder = NewValuePtrHolderA(&inValue);
		}

		~sPairA()
		{
			delete pValueHolder;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	struct sPairW : public sPairSuperW
	{
		template<typename T>
		sPairW(T& inValue) : sPairSuperW(NULL, L"")
		{
			pValueHolder = NewValuePtrHolderW(&inValue);
		}

		template<typename T>
		sPairW(const wchar_t* pszKey, T& inValue)
			 : sPairSuperW(NULL, pszKey)
		{
			pValueHolder = NewValuePtrHolderW(&inValue);
		}

		~sPairW()
		{
			delete pValueHolder;
		}
	};

}
