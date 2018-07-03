#include "stdafx.h"
#include "NumericString.h"
#include "STLString.h"
#include "POSIX.h"
#include "StdString.h"
#include "StdStringLegacy.h"

namespace core
{
	std::tstring StringFrom(char nValue)
	{	return Format(TEXT("%d"), nValue);	}

	std::tstring StringFrom(short nValue)
	{	return Format(TEXT("%d"), nValue);	}

	std::tstring StringFrom(int32_t nValue)
	{	return Format(TEXT("%d"), nValue);	}

	std::tstring StringFrom(int64_t nValue)
	{	return Format(TEXT("%lld"), nValue);}

#if defined(_MSC_VER)
	std::tstring StringFrom(uint32_t nValue)
	{	return Format(TEXT("%u"), nValue);	}
#endif

#if defined(__APPLE__)
    std::tstring StringFrom(size_t nValue)
    {    return Format(TEXT("%u"), nValue);    }
#endif

	std::tstring StringFrom(BYTE uValue)
	{	return Format(TEXT("%u"), uValue);	}

	std::tstring StringFrom(WORD uValue)
	{	return Format(TEXT("%u"), uValue);	}

	std::tstring StringFrom(DWORD uValue)
	{	return Format(TEXT("%u"), uValue);	}

	std::tstring StringFrom(QWORD uValue)
	{	return Format(TEXT("%llu"), uValue);}

	std::tstring StringFrom(float fValue)
	{	return Format(TEXT("%f"), fValue);	}

	std::tstring StringFrom(double dValue)
	{	return Format(TEXT("%lf"), dValue);	}

	std::tstring StringFrom(bool bValue)
	{	return Format(TEXT("%s"), bValue? TEXT("true") : TEXT("false"));	}

	std::tstring StringFrom(std::tstring strValue)
	{	return strValue;	}

	//////////////////////////////////////////////////////////////////////////
	const char g_cDigitTable[256] = 
	{
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* 00-0F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* 10-1F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* 20-2F */
		 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,	/* 30-3F */
		-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* 40-4F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* 50-5F */
		-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* 60-6F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* 70-7F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* 80-8F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* 90-9F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* A0-AF */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* B0-BF */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* C0-CF */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* D0-DF */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* E0-EF */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	/* F0-FF */
	};

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	static inline T NaturalNumFrom(LPCTSTR pszNumber, size_t tNumberLen, int nBase, size_t& tPos)
	{
		T uRet = 0;
		for(; tPos<tNumberLen; tPos++)
		{
			int nValue = g_cDigitTable[pszNumber[tPos] & 0xFF];
			if( (nValue < 0) || (nValue >= nBase) )
				return uRet;

			uRet *= nBase;
			uRet += (T)nValue;
		}

		return uRet;
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	static inline T Base16From(LPCTSTR pszNumber, size_t tNumberLen, size_t& tPos)
	{
		if( tNumberLen <= tPos )
			return 0;

		if( tPos+2 > tNumberLen )
			return 0;

		if( TEXT('0') != pszNumber[tPos] || TEXT('x') != pszNumber[tPos+1] )
			return 0;

		tPos += 2;
		return NaturalNumFrom<T>(pszNumber, tNumberLen, 16, tPos);
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	static inline T IntegerFrom(LPCTSTR pszNumber, size_t tNumberLen, size_t& tPos)
	{
		if( tNumberLen <= tPos )
			return 0;

		bool bNegative = false;
		switch(pszNumber[tPos])
		{
		case TEXT('-'):
			bNegative = true;
		case TEXT('+'):
			tPos ++;
			break;
		}

		T outValue = NaturalNumFrom<T>(pszNumber, tNumberLen, 10, tPos);
		return bNegative? (T)outValue * -1 : (T)outValue;
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	static inline T INTFrom(LPCTSTR pszNumber, size_t tNumberLen)
	{
		size_t tPos = 0;
		T outValue = Base16From<T>(pszNumber, tNumberLen, tPos);
		if( tPos > 0 )
			return outValue;

		return IntegerFrom<T>(pszNumber, tNumberLen, tPos);
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	static inline T RealFrom(LPCTSTR pszNumber, size_t tNumberLen, size_t& tPos)
	{
		if( tNumberLen <= tPos )
			return 0;

		T dReal = IntegerFrom<T>(pszNumber, tNumberLen, tPos);
		if( tNumberLen == tPos || TEXT('.') != pszNumber[tPos] )
			return dReal;

		size_t tPrePos = ++tPos;
		T dFraction = NaturalNumFrom<T>(pszNumber, tNumberLen, 10, tPos);

		size_t i;
		for(i=tPrePos; i<tPos; i++)
			dFraction *= (T)0.1;

		return dReal < 0? dReal - dFraction : dReal + dFraction;
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	static inline T ExpFrom(LPCTSTR pszNumber, size_t tNumberLen)
	{
		size_t tPos = 0;
		T dReal = RealFrom<T>(pszNumber, tNumberLen, tPos);
		if( tNumberLen == tPos )
			return dReal;

		if( TEXT('e') != pszNumber[tPos] && TEXT('E') != pszNumber[tPos] )
			return dReal;

		int nExp = IntegerFrom<int>(pszNumber, tNumberLen, ++tPos);
		int i;
		for(i=nExp; i<0; i++)
			dReal *= (T)0.1;

		for(i=0; i<nExp; i++)
			dReal *= (T)10.0;

		return dReal;
	}

	//////////////////////////////////////////////////////////////////////////
	static inline bool BoolFrom(LPCTSTR pszBool, size_t tBoolLen)
	{
		if( 0 == tBoolLen )
			return false;

		if( 0 == SafeStrCmp(pszBool, TEXT("true"), tBoolLen)
		||  0 == SafeStrCmp(pszBool, TEXT("yes"), tBoolLen) )
			return true;

		if( 0 == SafeStrCmp(pszBool, TEXT("false"), tBoolLen)
		||  0 == SafeStrCmp(pszBool, TEXT("no"), tBoolLen) )
			return false;

		return INTFrom<int>(pszBool, tBoolLen) != 0;
	}

	//////////////////////////////////////////////////////////////////////////
	bool BoolFrom(std::tstring strNum)
	{	return BoolFrom(strNum.c_str(), strNum.length());		}

	char CharFrom(std::tstring strNum)
	{	return INTFrom<char>(strNum.c_str(), strNum.length());	}

	short ShortFrom(std::tstring strNum)
	{	return INTFrom<short>(strNum.c_str(), strNum.length());	}

	int IntFrom(std::tstring strNum)
	{	return INTFrom<int>(strNum.c_str(), strNum.length());	}

	int32_t Int32From(std::tstring strNum)
	{	return INTFrom<int32_t>(strNum.c_str(), strNum.length());	}

	int64_t Int64From(std::tstring strNum)
	{	return INTFrom<int64_t>(strNum.c_str(), strNum.length());	}

	BYTE BYTEFrom(std::tstring strNum)
	{	return INTFrom<BYTE>(strNum.c_str(), strNum.length());	}

	WORD WORDFrom(std::tstring strNum)
	{	return INTFrom<WORD>(strNum.c_str(), strNum.length());	}

	DWORD DWORDFrom(std::tstring strNum)
	{	return INTFrom<DWORD>(strNum.c_str(), strNum.length());	}

	QWORD QWORDFrom(std::tstring strNum)
	{	return INTFrom<QWORD>(strNum.c_str(), strNum.length());	}

	float FloatFrom(std::tstring strNum)
	{	return ExpFrom<float>(strNum.c_str(), strNum.length());	}

	double DoubleFrom(std::tstring strNum)
	{	return ExpFrom<float>(strNum.c_str(), strNum.length());	}

	//////////////////////////////////////////////////////////////////////////	
	template<> bool		ValueFrom(std::tstring strNum)
	{	return BoolFrom(strNum.c_str(), strNum.length());			}

	template<> char		ValueFrom(std::tstring strNum)
	{	return INTFrom<char		>(strNum.c_str(), strNum.length());	}

	template<> short	ValueFrom(std::tstring strNum)
	{	return INTFrom<short	>(strNum.c_str(), strNum.length());	}

	template<> int		ValueFrom(std::tstring strNum)
	{	return INTFrom<int		>(strNum.c_str(), strNum.length());	}

#if defined(_MSC_VER)
	template<> unsigned int		ValueFrom(std::tstring strNum)
	{	return INTFrom<unsigned int>(strNum.c_str(), strNum.length());	}
#endif

#if defined(__APPLE__)
    template<> size_t   ValueFrom(std::tstring strNum)
    {    return INTFrom<size_t>(strNum.c_str(), strNum.length());    }
#endif

	template<> int64_t	ValueFrom(std::tstring strNum)
	{	return INTFrom<int64_t	>(strNum.c_str(), strNum.length());	}

	template<> BYTE		ValueFrom(std::tstring strNum)
	{	return INTFrom<BYTE		>(strNum.c_str(), strNum.length());	}

	template<> WORD		ValueFrom(std::tstring strNum)
	{	return INTFrom<WORD		>(strNum.c_str(), strNum.length());	}

	template<> DWORD	ValueFrom(std::tstring strNum)
	{	return INTFrom<DWORD	>(strNum.c_str(), strNum.length());	}

	template<> QWORD	ValueFrom(std::tstring strNum)
	{	return INTFrom<QWORD	>(strNum.c_str(), strNum.length());	}

	template<> float	ValueFrom(std::tstring strNum)
	{	return ExpFrom<float	>(strNum.c_str(), strNum.length());	}

	template<> double	ValueFrom(std::tstring strNum)
	{	return ExpFrom<double	>(strNum.c_str(), strNum.length());	}

	template<> std::tstring	ValueFrom(std::tstring strNum)
	{	return strNum;												}
}
