#pragma once

#include "../__Common/Define.h"
#include "../__Common/Type.h"

#ifdef UNICODE
#define MakeIPString				MakeIPStringW
#define IPAddressFrom				IPAddressFromW
#define GetWeekString				GetWeekStringW
#define GetMonthString				GetMonthStringW
#define GetOSTypeString				GetOSTypeStringW
#else
#define MakeIPString				MakeIPStringA
#define IPAddressFrom				IPAddressFromA
#define GetWeekString				GetWeekStringA
#define GetMonthString				GetMonthStringA
#define GetOSTypeString				GetOSTypeStringA
#endif

namespace core
{
	struct ST_IP_ADDRESS
	{
		union
		{
			DWORD dwIP;
			struct {
				BYTE addr[4];
			} ip;
		};

		ST_IP_ADDRESS(std::string strIP);
		ST_IP_ADDRESS(std::wstring strIP);
		std::string MakeIPStringA(void);
		std::wstring MakeIPStringW(void);
	};

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

	std::string		StringFromA(const ST_VERSIONINFO& stVersionInfo);
	std::wstring	StringFromW(const ST_VERSIONINFO& stVersionInfo);

	std::string StringFromA(const ST_SYSTEMTIME& stTime);
	std::wstring StringFromW(const ST_SYSTEMTIME& stTime);

	ST_VERSIONINFO VersionFrom(std::string strVersionContext);
	ST_VERSIONINFO VersionFrom(std::wstring strVersionContext);

	ST_SYSTEMTIME SystemTimeFrom(std::string strTimeContext);
	ST_SYSTEMTIME SystemTimeFrom(std::wstring strTimeContext);
}