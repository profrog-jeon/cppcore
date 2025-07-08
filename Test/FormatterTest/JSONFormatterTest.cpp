#include "stdafx.h"

struct ST_MALICOUS_PATTERN_INFO : public core::IFormatterObject
{
	std::tstring strPatternName;
	DWORD dwTotalChainCount;
	DWORD dwCurrentProcessingChainNo;

	void OnSync(core::IFormatter& formatter)
	{
		formatter.Sync(TEXT("PatternName"), strPatternName);
		formatter.Sync(TEXT("TotalChainCount"), dwTotalChainCount);
		formatter.Sync(TEXT("CurrentProcessingChainNo"), dwCurrentProcessingChainNo);
	}
};

struct ST_FILELESS_STATUS : public core::IFormatterObject
{
	std::map<std::tstring, ST_MALICOUS_PATTERN_INFO> mapPatternStatus;

	void OnSync(core::IFormatter& formatter)
	{
		formatter.Sync(TEXT("PatternStatus"), mapPatternStatus);
	}
};

TEST(FormatterTest, Simpletest)
{
	ST_FILELESS_STATUS stStatus;

	{
		ST_MALICOUS_PATTERN_INFO info;
		info.strPatternName = TEXT("Hollow_1");
		info.dwTotalChainCount = 10;
		info.dwCurrentProcessingChainNo = 0;
		stStatus.mapPatternStatus.insert(std::make_pair(info.strPatternName, info));
	}

	{
		ST_MALICOUS_PATTERN_INFO info;
		info.strPatternName = TEXT("Pattern_1");
		info.dwTotalChainCount = 5;
		info.dwCurrentProcessingChainNo = 2;
		stStatus.mapPatternStatus.insert(std::make_pair(info.strPatternName, info));
	}

	const std::tstring strFileName = GenerateGuid() + TEXT(".txt");
	core::UTF8::WriteJsonToFile(&stStatus, strFileName);

	ST_FILELESS_STATUS stStatusRestored;
	core::UTF8::ReadJsonFromFile(&stStatusRestored, strFileName);

	DeleteFile(strFileName.c_str());

	EXPECT_EQ(2U, stStatusRestored.mapPatternStatus.size());
}
