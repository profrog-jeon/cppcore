#include "stdafx.h"
#include "../../Src/100_System/TZFileParser.h"
#include <vector>

using namespace core;

//////////////////////////////////////////////////////////////////////////
std::string DateTimeStr(const ST_SYSTEMTIME& stTime)
{
	std::string strRet = Format("%04d-%02d-%02d %02d:%02d:%02d.%03d"
		, stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds);
	return strRet;
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, LocalTimeTest)
{
	ST_SYSTEMTIME stCurrentTime;
	GetLocalTime(&stCurrentTime);

	ST_SYSTEMTIME stTestTime = { 2017, 4, 0, 9, 22, 49, 12};
	EXPECT_TRUE(SetLocalTime(&stTestTime));

	ST_SYSTEMTIME stRestoredTime = { 0, };
	GetLocalTime(&stRestoredTime);
	SetLocalTime(&stCurrentTime);

	stTestTime.wSecond = stRestoredTime.wSecond = 0;
	stTestTime.wMilliseconds = stRestoredTime.wMilliseconds = 0;
	{
		std::string strTimeCompare = Format("test:%s\n, restored:%s\n"
			, DateTimeStr(stTestTime).c_str()
			, DateTimeStr(stRestoredTime).c_str());

		SCOPED_TRACE(strTimeCompare.c_str());
		EXPECT_EQ(0, memcmp(&stTestTime, &stRestoredTime, sizeof(stTestTime)));
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, SystemTimeTest)
{
	ST_SYSTEMTIME stCurrentTime;
	GetSystemTime(&stCurrentTime);

	ST_SYSTEMTIME stTestTime = { 2017, 4, 0, 9, 22, 49, 12};
	EXPECT_TRUE(SetSystemTime(&stTestTime));

	ST_SYSTEMTIME stRestoredTime = { 0, };
	GetSystemTime(&stRestoredTime);
	SetSystemTime(&stCurrentTime);

	stTestTime.wSecond = stRestoredTime.wSecond = 0;
	stTestTime.wMilliseconds = stRestoredTime.wMilliseconds = 0;
	{
		std::string strTimeCompare = Format("test:%s\n, restored:%s\n"
			, DateTimeStr(stTestTime).c_str()
			, DateTimeStr(stRestoredTime).c_str());

		SCOPED_TRACE(strTimeCompare.c_str());
		EXPECT_EQ(0, memcmp(&stTestTime, &stRestoredTime, sizeof(stTestTime)));
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, TZTimeConversionTest)
{
	ST_SYSTEMTIME stTZTime = { 0, 3, 0, 2, 2, 0, 0, 0 };
	ST_SYSTEMTIME stConvertedTime;
	EXPECT_TRUE(MakeAbsoluteTZTime(2018, stTZTime, stConvertedTime));

	ST_SYSTEMTIME stAbsTime = { 2018, 3, 0, 11, 2, 0, 0, 0 };
	EXPECT_EQ(0, memcmp(&stConvertedTime, &stAbsTime, sizeof(stConvertedTime)));
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, TZTimeConversionTest2)
{
	ST_SYSTEMTIME stTZTime = { 0, 11, 0, 1, 2, 0, 0, 0 };
	ST_SYSTEMTIME stConvertedTime;
	EXPECT_TRUE(MakeAbsoluteTZTime(2018, stTZTime, stConvertedTime));

	ST_SYSTEMTIME stAbsTime = { 2018, 11, 0, 4, 2, 0, 0, 0 };
	EXPECT_EQ(0, memcmp(&stConvertedTime, &stAbsTime, sizeof(stConvertedTime)));
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, GetComputerNameTest)
{
	tprintf(TEXT("%s\n"), GetComputerName().c_str());
	EXPECT_NE(std::tstring(TEXT("")), GetComputerName());
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, GetUserNameTest)
{
	tprintf(TEXT("%s\n"), GetUserName().c_str());
	EXPECT_NE(std::tstring(TEXT("")), GetUserName());
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, IPAddressToDWORDTest)
{
	std::tstring strOriginal = TEXT("1.2.3.4");
	EXPECT_EQ(0x04030201, IPAddressFrom(strOriginal.c_str()));
	EXPECT_EQ(strOriginal, IPAddressFrom(0x04030201));
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, IPAddressToDWORDTest_failure1)
{
	EXPECT_EQ(0, IPAddressFrom(TEXT("1.2.3")));
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, IPAddressToDWORDTest_failure2)
{
	EXPECT_EQ(0, IPAddressFrom(TEXT("1.2.3.444")));
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, IPAddressToDWORDTest_failure3)
{
	EXPECT_EQ(0, IPAddressFrom(TEXT("1.2.3.4.5.6")));
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, HostFromNetworkTest)
{
	if( IsBigEndian() )
	{
		WORD wValue = 0x0102;
		EXPECT_EQ(wValue, HostFromNetwork(wValue));

		DWORD dwValue = 0x01020304;
		EXPECT_EQ(dwValue, HostFromNetwork(dwValue));

		QWORD qwValue = 0x0102030405060708ULL;
		EXPECT_EQ(qwValue, HostFromNetwork(qwValue));
	}
	else
	{
		WORD wValue = 0x0102;
		EXPECT_EQ(0x0201, HostFromNetwork(wValue));

		DWORD dwValue = 0x01020304;
		EXPECT_EQ(0x04030201, HostFromNetwork(dwValue));

		QWORD qwValue = 0x0102030405060708ULL;
		EXPECT_EQ(0x0807060504030201ULL, HostFromNetwork(qwValue));
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, NetworkFromHostTest)
{
	if( IsBigEndian() )
	{
		WORD wValue = 1;
		EXPECT_EQ(wValue, NetworkFromHost(wValue));

		DWORD dwValue = 1;
		EXPECT_EQ(dwValue, NetworkFromHost(dwValue));

		QWORD qwValue = 1;
		EXPECT_EQ(qwValue, NetworkFromHost(qwValue));
	}
	else
	{
		WORD wValue = 0x0100;
		EXPECT_EQ(0x01, NetworkFromHost(wValue));

		DWORD dwValue = 0x01000000;
		EXPECT_EQ(0x01, NetworkFromHost(dwValue));

		QWORD qwValue = 0x0100000000000000ULL;
		EXPECT_EQ(0x01, NetworkFromHost(qwValue));
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, GetOSTypeTest)
{
	E_OS_TYPE nOSType = GetOSType();
	ASSERT_NE(OS_TYPE_UNDEFINED, nOSType);
	std::tstring strOSType = GetOSTypeString(nOSType);
	ASSERT_FALSE(strOSType.empty());
	tprintf(TEXT("%s\n"), strOSType.c_str());
}

//////////////////////////////////////////////////////////////////////////
void TestUnixTimeConversion(UINT64 tUnixTime)
{
#ifdef _MSC_VER	// For windows only
	ST_SYSTEMTIME stCorrectSystemTime	= SystemTimeFrom_NativeAPI(tUnixTime);	// test oracle
	ST_SYSTEMTIME stSystemTime			= SystemTimeFrom(tUnixTime);
	UINT64		uRestoredValue		= UnixTimeFrom(stSystemTime);

	{
		std::string strDate = Format("%u-%u-%u %u:%u:%u.%u(%u)"
			, stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay
			, stSystemTime.wHour, stSystemTime.wMinute, stSystemTime.wSecond
			, stSystemTime.wMilliseconds, stSystemTime.wDayOfWeek			);
		SCOPED_TRACE(strDate.c_str());
		EXPECT_EQ(uRestoredValue, tUnixTime);
		EXPECT_EQ(stSystemTime, stCorrectSystemTime);
	}
#endif
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, DISABLED_CalcDayOfWeek_RandomTest)
{
	UINT64 i;
	for(i=0; i<50 * (365 + 1) * 24 * 60 * 60; i+=rand()%1000)	// Test during 50 years include leap(1970 ~ 2020)
	{
		ST_SYSTEMTIME stSystemTime = SystemTimeFrom(i);
		WORD wDayOfWeek = CalcDayOfWeek(stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay);
		ASSERT_EQ(stSystemTime.wDayOfWeek, wDayOfWeek);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, DISABLED_UnixTimeConversion_RandomTest)
{
	UINT64 i;
	for(i=0; i<50 * (365 + 1) * 24 * 60 * 60; i+=rand()%1000)	// Test during 50 years include leap(1970 ~ 2020)
	{
		TestUnixTimeConversion(i);
	}
}

//////////////////////////////////////////////////////////////////////////
// Disabled for spending too much time
TEST(SystemTest, DISABLED_UnixTimeConversion_BruteForceTest)
{
	UINT64 i;
	for(i=0; i<50 * (365 + 1) * 24 * 60 * 60; i++)	// Test during 50 years include leap(1970 ~ 2020)
	{
		TestUnixTimeConversion(i);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, UnixTimeConversion_BoundaryValueTest)
{
	TestUnixTimeConversion(0);					// starting point
	TestUnixTimeConversion(2*365*24*60*60);		// after the second year
	TestUnixTimeConversion((4*365+1)*24*60*60);	// after the first leap year	
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, DISABLED_GuidRandomTest)
{
	std::set<std::tstring> setGuid;

	// Different GUID should be generated at least 100000 times
	size_t i;
	for(i=0; i<100000; i++)
	{
		std::tstring strGuid = GenerateGuid();
		EXPECT_TRUE(setGuid.find(strGuid) == setGuid.end());
		setGuid.insert(strGuid);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, TimeZoneTest)
{
	ST_TIME_ZONE_INFORMATION stTimeZone = { 0, };
	GetTimeZoneInformation(&stTimeZone);
	tprintf(TEXT("Bias         %d\n"), stTimeZone.Bias);
	tprintf(TEXT("StandardBias %d\n"), stTimeZone.StandardBias);
	tprintf(TEXT("StandardDate %04u-%02u-%02u(weekday:%02u) %02u:%02u:%02u.%03u\n")
		, stTimeZone.StandardDate.wYear			
		, stTimeZone.StandardDate.wMonth			
		, stTimeZone.StandardDate.wDay			
		, stTimeZone.StandardDate.wHour			
		, stTimeZone.StandardDate.wDayOfWeek		
		, stTimeZone.StandardDate.wMinute		
		, stTimeZone.StandardDate.wSecond		
		, stTimeZone.StandardDate.wMilliseconds	
		);

	tprintf(TEXT("DaylightBias %d\n"), stTimeZone.DaylightBias);
	tprintf(TEXT("StandardDate %04u-%02u-%02u(weekday:%02u) %02u:%02u:%02u.%03u\n")
		, stTimeZone.DaylightDate.wYear			
		, stTimeZone.DaylightDate.wMonth			
		, stTimeZone.DaylightDate.wDay			
		, stTimeZone.DaylightDate.wHour			
		, stTimeZone.DaylightDate.wDayOfWeek		
		, stTimeZone.DaylightDate.wMinute		
		, stTimeZone.DaylightDate.wSecond		
		, stTimeZone.DaylightDate.wMilliseconds	
		);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, TimeZoneTest_Canberra_Daylight)
{
	ST_TIME_ZONE_INFORMATION stTZInfo = { -600, };

	ST_SYSTEMTIME stTime = { 2018, 1, 1, 1, 1, 0, 0, };
	EXPECT_EQ(TIME_ZONE_ID_DAYLIGHT_, GetTimeZoneInformationByTZFile(&stTZInfo, TEXT("../../Build/Test/TimeZoneTest/Canberra"), stTime));

	EXPECT_EQ(-600, stTZInfo.Bias);
	EXPECT_EQ(0, stTZInfo.StandardBias);
	EXPECT_EQ(-60, stTZInfo.DaylightBias);
	EXPECT_EQ(4, stTZInfo.StandardDate.wMonth);
	EXPECT_EQ(1, stTZInfo.StandardDate.wDay);
	EXPECT_EQ(3, stTZInfo.StandardDate.wHour);
	EXPECT_EQ(10, stTZInfo.DaylightDate.wMonth);
	EXPECT_EQ(7, stTZInfo.DaylightDate.wDay);
	EXPECT_EQ(2, stTZInfo.DaylightDate.wHour);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, TimeZoneTest_Santiago_Daylight)
{
	ST_TIME_ZONE_INFORMATION stTZInfo = { 240 };

	ST_SYSTEMTIME stTime = { 2018, 1, 1, 1, 1, 0, 0, };
	EXPECT_EQ(TIME_ZONE_ID_DAYLIGHT_, GetTimeZoneInformationByTZFile(&stTZInfo, TEXT("../../Build/Test/TimeZoneTest/Santiago"), stTime));

	EXPECT_EQ(240, stTZInfo.Bias);
	EXPECT_EQ(0, stTZInfo.StandardBias);
	EXPECT_EQ(-60, stTZInfo.DaylightBias);
	EXPECT_EQ(5, stTZInfo.StandardDate.wMonth);
	EXPECT_EQ(13, stTZInfo.StandardDate.wDay);
	EXPECT_EQ(0, stTZInfo.StandardDate.wHour);
	EXPECT_EQ(8, stTZInfo.DaylightDate.wMonth);
	EXPECT_EQ(12, stTZInfo.DaylightDate.wDay);
	EXPECT_EQ(0, stTZInfo.DaylightDate.wHour);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, TimeZoneTest_Santiago_Standard)
{
	ST_TIME_ZONE_INFORMATION stTZInfo = { 240 };

	ST_SYSTEMTIME stTime = { 2018, 7, 1, 1, 1, 0, 0, };
	EXPECT_EQ(TIME_ZONE_ID_STANDARD_, GetTimeZoneInformationByTZFile(&stTZInfo, TEXT("../../Build/Test/TimeZoneTest/Santiago"), stTime));

	EXPECT_EQ(240, stTZInfo.Bias);
	EXPECT_EQ(0, stTZInfo.StandardBias);
	EXPECT_EQ(-60, stTZInfo.DaylightBias);
	EXPECT_EQ(5, stTZInfo.StandardDate.wMonth);
	EXPECT_EQ(13, stTZInfo.StandardDate.wDay);
	EXPECT_EQ(0, stTZInfo.StandardDate.wHour);
	EXPECT_EQ(8, stTZInfo.DaylightDate.wMonth);
	EXPECT_EQ(12, stTZInfo.DaylightDate.wDay);
	EXPECT_EQ(0, stTZInfo.DaylightDate.wHour);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, TimeZoneTest_Vancouver_Standard)
{
	ST_TIME_ZONE_INFORMATION stTZInfo = { 480, };

	ST_SYSTEMTIME stTime = { 2018, 1, 1, 1, 1, 0, 0, };
	EXPECT_EQ(TIME_ZONE_ID_STANDARD_, GetTimeZoneInformationByTZFile(&stTZInfo, TEXT("../../Build/Test/TimeZoneTest/Vancouver"), stTime));

	EXPECT_EQ(480, stTZInfo.Bias);
	EXPECT_EQ(0, stTZInfo.StandardBias);
	EXPECT_EQ(-60, stTZInfo.DaylightBias);
	EXPECT_EQ(3, stTZInfo.DaylightDate.wMonth);
	EXPECT_EQ(11, stTZInfo.DaylightDate.wDay);
	EXPECT_EQ(2, stTZInfo.DaylightDate.wHour);
	EXPECT_EQ(11, stTZInfo.StandardDate.wMonth);
	EXPECT_EQ(4, stTZInfo.StandardDate.wDay);
	EXPECT_EQ(2, stTZInfo.StandardDate.wHour);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, TimeZoneTest_Vancouver_Daylight)
{
	ST_TIME_ZONE_INFORMATION stTZInfo = { 480, };

	ST_SYSTEMTIME stTime = { 2018, 7, 1, 1, 1, 0, 0, };
	EXPECT_EQ(TIME_ZONE_ID_DAYLIGHT_, GetTimeZoneInformationByTZFile(&stTZInfo, TEXT("../../Build/Test/TimeZoneTest/Vancouver"), stTime));

	EXPECT_EQ(480, stTZInfo.Bias);
	EXPECT_EQ(0, stTZInfo.StandardBias);
	EXPECT_EQ(-60, stTZInfo.DaylightBias);
	EXPECT_EQ(3, stTZInfo.DaylightDate.wMonth);
	EXPECT_EQ(11, stTZInfo.DaylightDate.wDay);
	EXPECT_EQ(2, stTZInfo.DaylightDate.wHour);
	EXPECT_EQ(11, stTZInfo.StandardDate.wMonth);
	EXPECT_EQ(4, stTZInfo.StandardDate.wDay);
	EXPECT_EQ(2, stTZInfo.StandardDate.wHour);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, TimeZoneTest_Seoul)
{
	ST_TIME_ZONE_INFORMATION stTZInfo;
	ST_TIME_ZONE_INFORMATION stTZInfo2 = { -540 };

	ST_SYSTEMTIME stTime;
	GetSystemTime(&stTime);
	EXPECT_EQ(GetTimeZoneInformation(&stTZInfo), GetTimeZoneInformationByTZFile(&stTZInfo2, TEXT("../../Build/Test/TimeZoneTest/Seoul"), stTime));

	EXPECT_EQ(stTZInfo.Bias, stTZInfo2.Bias);
	EXPECT_EQ(stTZInfo.StandardBias, stTZInfo2.StandardBias);
	EXPECT_STRNE(TEXT(""), stTZInfo.StandardName);
	EXPECT_STRNE(TEXT(""), stTZInfo.DaylightName);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, TimeZoneConversionTest)
{
	ST_SYSTEMTIME stSystemTime = { 0, };
	GetSystemTime(&stSystemTime);

	ST_SYSTEMTIME stLocalTime = { 0, };
	GetLocalTime(&stLocalTime);

	UINT64 uSystemTime = UnixTimeFrom(stSystemTime);
	UINT64 uLocalTime = UnixTimeFrom(stLocalTime);

	ST_TIME_ZONE_INFORMATION stTimeZone = { 0, };
	GetTimeZoneInformation(&stTimeZone);

	std::string strDebugInfo = Format(
		"Local: %04u-%02u-%02u %02u:%02u:%02u\n"
		"GMT:   %04u-%02u-%02u %02u:%02u:%02u\n"
		"Bias: %d\n"
		, stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay, stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond
		, stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay, stSystemTime.wHour, stSystemTime.wMinute, stSystemTime.wSecond
		, stTimeZone.Bias);
	SCOPED_TRACE(strDebugInfo.c_str());
	EXPECT_EQ(uSystemTime, uLocalTime + stTimeZone.Bias * 60);
}

//////////////////////////////////////////////////////////////////////////
TEST(SystemTest, ThreadIdTest)			{	EXPECT_NE(0, GetCurrentThreadId());	}
TEST(SystemTest, ProcessIdTest)			{	EXPECT_NE(0, GetCurrentProcessId());	}
TEST(SystemTest, ThreadHandleTest)		{	EXPECT_NE((HANDLE)NULL, GetCurrentThread());	}
TEST(SystemTest, ProcessHandleTest)		{	EXPECT_NE((HANDLE)NULL, GetCurrentProcess());	}