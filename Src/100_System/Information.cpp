#include "stdafx.h"
#include "Information.h"
#include "Log.h"
#include "Environment.h"

namespace core
{
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
				Log_Error(TEXT("IP address %dth token exceed 0xFF in %s"), i+1, pszIP);
				SetLastError(EC_INVALID_IP_STRING);
				return 0;
			}
			uRet.btValue[i] = dwValue & 0xFF;
		}

		if( (i != 4) || (nOffset > 0) )
		{
			Log_Error(TEXT("Invalid IP address token count, %s"), pszIP);
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
};