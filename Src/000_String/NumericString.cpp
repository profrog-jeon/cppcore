#include "stdafx.h"
#include "NumericString.h"
#include "STLString.h"
#include "POSIX.h"
#include "StdString.h"
#include "StdStringLegacy.h"
#include "Unicode.h"
#include "ASCII.h"

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

	std::tstring StringFrom(std::ntstring strValue)
	{	return TCSFromNTCS(strValue);	}





	std::tstring HexFrom(char nValue)
	{
		return Format(TEXT("0x%08X"), (unsigned char)nValue);
	}

	std::tstring HexFrom(short nValue)
	{
		return Format(TEXT("0x%08X"), (unsigned short)nValue);
	}

	std::tstring HexFrom(int32_t nValue)
	{
		return Format(TEXT("0x%08X"), (unsigned int)nValue);
	}

	std::tstring HexFrom(int64_t nValue)
	{
		return Format(TEXT("0x%016llX"), nValue);
	}

#if defined(_MSC_VER)
	std::tstring HexFrom(uint32_t nValue)
	{
		return Format(TEXT("0x%08X"), nValue);
	}
#endif

#if defined(__APPLE__)
	std::tstring HexFrom(size_t nValue)
	{
		return Format(TEXT("0x%08X"), nValue);
	}
#endif

	std::tstring HexFrom(BYTE uValue)
	{
		return Format(TEXT("0x%08X"), uValue);
	}

	std::tstring HexFrom(WORD uValue)
	{
		return Format(TEXT("0x%08X"), uValue);
	}

	std::tstring HexFrom(DWORD uValue)
	{
		return Format(TEXT("0x%08X"), uValue);
	}

	std::tstring HexFrom(QWORD uValue)
	{
		return Format(TEXT("0x%016llX"), uValue);
	}

	std::tstring HexFrom(float fValue)
	{
		return Format(TEXT("%f"), fValue);
	}

	std::tstring HexFrom(double dValue)
	{
		return Format(TEXT("%lf"), dValue);
	}

	std::tstring HexFrom(bool bValue)
	{
		return Format(TEXT("%s"), bValue ? TEXT("true") : TEXT("false"));
	}

	std::tstring HexFrom(std::tstring strValue)
	{
		return strValue;
	}

	std::tstring HexFrom(std::ntstring strValue)
	{
		return TCSFromNTCS(strValue);
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	static inline T NaturalNumFrom(LPCTSTR pszNumber, size_t tNumberLen, int nBase, size_t& tPos)
	{
		T uRet = 0;
		for(; tPos<tNumberLen; tPos++)
		{
			int nValue = g_cHexCharAscii[pszNumber[tPos] & 0xFF];
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
	{	return ExpFrom<double>(strNum.c_str(), strNum.length());	}

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

	template<> std::tstring	ValueFrom(std::ntstring strNum)
	{	return TCSFromNTCS(strNum);									}

	DWORD DWORDFromBase(std::tstring strNum, int nBase)
	{
		size_t tPos = 0;
		return NaturalNumFrom<DWORD>(strNum.c_str(), strNum.length(), nBase, tPos);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring StringFromHex(const unsigned char* pData, size_t tDataSize)
	{
		std::tstring strResult;
		strResult.resize(tDataSize * 2);
		TCHAR* pDest = (TCHAR*)strResult.c_str();

		size_t i;
		for (i = 0; i < tDataSize; i++)
		{
			*(pDest++) = g_cHexCharTable[(pData[i] >> 4) & 0x0F];
			*(pDest++) = g_cHexCharTable[(pData[i] >> 0) & 0x0F];
		}

		return strResult;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring StringFromHex(const unsigned int* pData, size_t tDataSize)
	{
		std::tstring strResult;
		strResult.resize(tDataSize * 8);
		TCHAR* pDest = (TCHAR*)strResult.c_str();

		size_t i;
		for (i = 0; i < tDataSize; i++)
		{
			*(pDest++) = g_cHexCharTable[(pData[i] >> 28) & 0x0F];
			*(pDest++) = g_cHexCharTable[(pData[i] >> 24) & 0x0F];
			*(pDest++) = g_cHexCharTable[(pData[i] >> 20) & 0x0F];
			*(pDest++) = g_cHexCharTable[(pData[i] >> 16) & 0x0F];
			*(pDest++) = g_cHexCharTable[(pData[i] >> 12) & 0x0F];
			*(pDest++) = g_cHexCharTable[(pData[i] >> 8) & 0x0F];
			*(pDest++) = g_cHexCharTable[(pData[i] >> 4) & 0x0F];
			*(pDest++) = g_cHexCharTable[(pData[i] >> 0) & 0x0F];
		}

		return strResult;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE HexFromString(LPBYTE pDest, size_t tDestSize, std::tstring strContext)
	{
		size_t i;
		for (i = 0; (i * 2 + 1) < strContext.length() && i < tDestSize; i++)
		{
			DWORD cFront = strContext.at(i * 2);
			DWORD cBack = strContext.at(i * 2 + 1);
			if (cFront > 0xFF)
				return EC_INVALID_DATA;
			if (cBack > 0xFF)
				return EC_INVALID_DATA;

			BYTE btFrontValue = g_cHexCharAscii[cFront];
			BYTE btBackValue = g_cHexCharAscii[cBack];
			if (btFrontValue == 0xFF)
				return EC_INVALID_DATA;
			if (btBackValue == 0xFF)
				return EC_INVALID_DATA;

			pDest[i] = (btFrontValue << 4) | (btBackValue << 0);
		}

		return EC_SUCCESS;
	}
}
