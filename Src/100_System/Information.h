#pragma once

#include "../__Common/Define.h"
#include "../__Common/Type.h"

#ifdef UNICODE
#define IPAddressFrom				IPAddressFromW
#define GetWeekString				GetWeekStringW
#define GetMonthString				GetMonthStringW
#define GetOSTypeString				GetOSTypeStringW
#else
#define IPAddressFrom				IPAddressFromA
#define GetWeekString				GetWeekStringA
#define GetMonthString				GetMonthStringA
#define GetOSTypeString				GetOSTypeStringA
#endif

namespace core
{
	DWORD			IPAddressFromA(const char* pszIP);
	DWORD			IPAddressFromW(const wchar_t* pszIP);
	std::string		IPAddressFromA(DWORD dwIP);
	std::wstring	IPAddressFromW(DWORD dwIP);
	std::string		GetWeekStringA(WORD wDayOfWeek);
	std::wstring	GetWeekStringW(WORD wDayOfWeek);
	std::string		GetMonthStringA(WORD wMonth);
	std::wstring	GetMonthStringW(WORD wMonth);
	const char*		GetOSTypeStringA(E_OS_TYPE nVersion);
	const wchar_t*	GetOSTypeStringW(E_OS_TYPE nVersion);
}