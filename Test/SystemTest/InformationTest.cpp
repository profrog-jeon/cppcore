#include "stdafx.h"

TEST(InformationTest, SystemTimeFromStringTest)
{
	ST_SYSTEMTIME stLocalTime;
	GetLocalTime(&stLocalTime);

	std::tstring strTime = StringFrom(stLocalTime);

	ST_SYSTEMTIME stRestoredTime = SystemTimeFrom(strTime);
	EXPECT_EQ(stLocalTime.wYear			, stRestoredTime.wYear			);
	EXPECT_EQ(stLocalTime.wMonth		, stRestoredTime.wMonth			);
	EXPECT_EQ(stLocalTime.wDayOfWeek	, stRestoredTime.wDayOfWeek		);
	EXPECT_EQ(stLocalTime.wDay			, stRestoredTime.wDay			);
	EXPECT_EQ(stLocalTime.wHour			, stRestoredTime.wHour			);
	EXPECT_EQ(stLocalTime.wMinute		, stRestoredTime.wMinute		);
	EXPECT_EQ(stLocalTime.wSecond		, stRestoredTime.wSecond		);
	//EXPECT_EQ(stLocalTime.wMilliseconds	, stRestoredTime.wMilliseconds	);
}

TEST(InformationTest, VersionFromStringTest)
{
	const ST_VERSIONINFO stVersion = { 1, 4, 3, 2 };

	std::tstring strVersion = StringFrom(stVersion);

	ST_VERSIONINFO stRestoredVersion = VersionFrom(strVersion);
	EXPECT_EQ(stVersion.dwMajor, stRestoredVersion.dwMajor);
	EXPECT_EQ(stVersion.dwMinor, stRestoredVersion.dwMinor);
	EXPECT_EQ(stVersion.dwPatch, stRestoredVersion.dwPatch);
	EXPECT_EQ(stVersion.dwBuild, stRestoredVersion.dwBuild);
}
