#include "stdafx.h"
#include "Information.h"
#include "Log.h"
#include "Environment.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	ST_IP_ADDRESS::ST_IP_ADDRESS(std::tstring strIP)
	{
		dwIP = core::IPAddressFrom(strIP.c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring ST_IP_ADDRESS::MakeIPString(void)
	{
		return core::IPAddressFrom(dwIP);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GetWeekString(WORD wDayOfWeek)
	{
		switch(wDayOfWeek)
		{
		case 0:			return TEXT("Sunday");
		case 1:			return TEXT("Monday");
		case 2:			return TEXT("Tuesday");
		case 3:			return TEXT("Wednesday");
		case 4:			return TEXT("Thursday");
		case 5:			return TEXT("Friday");
		case 6:			return TEXT("Saturday");
		}

		return Format(TEXT("Unknown Week:%u"), wDayOfWeek);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GetMonthString(WORD wMonth)
	{
		switch(wMonth)
		{
		case 1:			return TEXT("January");
		case 2:			return TEXT("February");
		case 3:			return TEXT("March");
		case 4:			return TEXT("April");
		case 5:			return TEXT("May");
		case 6:			return TEXT("June");
		case 7:			return TEXT("July");
		case 8:			return TEXT("August");
		case 9:			return TEXT("September");
		case 10:		return TEXT("October");
		case 11:		return TEXT("November");
		case 12:		return TEXT("December");
		}

		return Format(TEXT("Undefined Month:%u"), wMonth);
	}

	//////////////////////////////////////////////////////////////////////////
	LPCTSTR GetOSTypeString(E_OS_TYPE nVersion)
	{
		switch(nVersion)
		{
		case OS_TYPE_UNDEFINED				:		return TEXT("Undefined OS Type");
		case OS_TYPE_WINDOWS_2000			:		return TEXT("Windows 2000");
		case OS_TYPE_WINDOWS_XP				:		return TEXT("Windows XP");
		case OS_TYPE_WINDOWS_SERVER_2003	:		return TEXT("Windows Server 2003");
		case OS_TYPE_WINDOWS_SERVER_2003_R2	:		return TEXT("Windows Server 2003 R2");
		case OS_TYPE_WINDOWS_SERVER_2008	:		return TEXT("Windows Server 2008");
		case OS_TYPE_WINDOWS_SERVER_2008_R2	:		return TEXT("Windows Server 2008 R2");
		case OS_TYPE_WINDOWS_VISTA			:		return TEXT("Windows Vista");
		case OS_TYPE_WINDOWS_7				:		return TEXT("Windows 7");
		case OS_TYPE_WINDOWS_SERVER_2012	:		return TEXT("Windows Server 2012");
		case OS_TYPE_WINDOWS_SERVER_2012_R2	:		return TEXT("Windows Server 2012 R2");
		case OS_TYPE_WINDOWS_8				:		return TEXT("Windows 8");
		case OS_TYPE_WINDOWS_8_1			:		return TEXT("Windows 8.1");
		case OS_TYPE_WINDOWS_SERVER_2016	:		return TEXT("Windows Server 2016");
		case OS_TYPE_WINDOWS_10				:		return TEXT("Windows 10");
		case OS_TYPE_LINUX_X86				:		return TEXT("Linux x86");
		case OS_TYPE_LINUX_X64				:		return TEXT("Linux x64");
		}
		return TEXT("Unknown OS Type");
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring IPAddressFrom(DWORD dwIP)
	{
		union
		{
			DWORD dwValue;
			BYTE btValue[4];
		}	uValue;
		uValue.dwValue = dwIP;
		return Format(TEXT("%u.%u.%u.%u"), uValue.btValue[0], uValue.btValue[1], uValue.btValue[2], uValue.btValue[3]);
	}

	//////////////////////////////////////////////////////////////////////////
	DWORD IPAddressFrom(LPCTSTR pszIP)
	{		
		std::tstring strIP(pszIP);

		union
		{
			DWORD dwValue;
			BYTE btValue[4];
		}	uRet;

		int nOffset = 0;
		int i;
		for(i=0; i<4 && nOffset>=0; i++)
		{
			std::tstring strToken = Tokenize(strIP, TEXT("."), nOffset);
			DWORD dwValue = DWORDFrom(strToken);
			if( dwValue > 0xFF )
			{
				Log_Debug(TEXT("IP address %dth token exceed 0xFF in %s"), i+1, pszIP);
				SetLastError(EC_INVALID_IP_STRING);
				return 0;
			}
			uRet.btValue[i] = dwValue & 0xFF;
		}

		if( (i != 4) || (nOffset > 0) )
		{
			Log_Debug(TEXT("Invalid IP address token count, %s"), pszIP);
			SetLastError(EC_INVALID_IP_STRING);
			return 0;
		}

		return uRet.dwValue;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring StringFrom(const ST_VERSIONINFO& stVersionInfo)
	{
		return Format(TEXT("%u.%u.%u.%u")
			, stVersionInfo.dwMajor
			, stVersionInfo.dwMinor
			, stVersionInfo.dwPatch
			, stVersionInfo.dwBuild);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring StringFrom(const ST_SYSTEMTIME& stTime)
	{
		return Format(TEXT("%04d-%02d-%02d %02d:%02d:%02d")
			, stTime.wYear, stTime.wMonth, stTime.wDay
			, stTime.wHour, stTime.wMinute, stTime.wSecond);
	}

	//////////////////////////////////////////////////////////////////////////
	ST_VERSIONINFO VersionFrom(std::tstring strVersionContext)
	{
		std::vector<std::tstring> vecToken;
		TokenizeToArray(strVersionContext, TEXT("."), vecToken, false);

		ST_VERSIONINFO stVersion = { 0, };
		if (0 < vecToken.size())		stVersion.dwMajor = DWORDFrom(vecToken[0]);
		if (1 < vecToken.size())		stVersion.dwMinor = DWORDFrom(vecToken[1]);
		if (2 < vecToken.size())		stVersion.dwPatch = DWORDFrom(vecToken[2]);
		if (3 < vecToken.size())		stVersion.dwBuild = DWORDFrom(vecToken[3]);

		return stVersion;
	}

	//////////////////////////////////////////////////////////////////////////
	ST_SYSTEMTIME SystemTimeFrom(std::tstring strTimeContext)
	{
		std::vector<std::tstring> vecToken;
		TokenizeToArray(strTimeContext, TEXT("- :."), vecToken, false);

		ST_SYSTEMTIME stTime = { 0, };
		if (0 < vecToken.size())		stTime.wYear		 = WORDFrom(vecToken[0]);
		if (1 < vecToken.size())		stTime.wMonth		 = WORDFrom(vecToken[1]);
		if (2 < vecToken.size())		stTime.wDay			 = WORDFrom(vecToken[2]);
		if (3 < vecToken.size())		stTime.wHour		 = WORDFrom(vecToken[3]);
		if (4 < vecToken.size())		stTime.wMinute		 = WORDFrom(vecToken[4]);
		if (5 < vecToken.size())		stTime.wSecond		 = WORDFrom(vecToken[5]);
		if (6 < vecToken.size())		stTime.wMilliseconds = WORDFrom(vecToken[6]);

		stTime.wDayOfWeek = CalcDayOfWeek(stTime.wYear, stTime.wMonth, stTime.wDay);
		return stTime;
	}
};