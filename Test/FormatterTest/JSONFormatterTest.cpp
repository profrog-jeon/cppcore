#include "stdafx.h"

struct ST_MALICOUS_PATTERN_INFO : public core::IFormatterObject
{
	std::tstring strPatternName;
	DWORD dwTotalChainCount;
	DWORD dwCurrentProcessingChainNo;

	void OnSync(core::IFormatter& formatter)
	{
		formatter
			+ core::sPair(TEXT("PatternName"), strPatternName)
			+ core::sPair(TEXT("TotalChainCount"), dwTotalChainCount)
			+ core::sPair(TEXT("CurrentProcessingChainNo"), dwCurrentProcessingChainNo)
			;
	}
};

struct ST_FILELESS_STATUS : public core::IFormatterObject
{
	std::map<std::tstring, ST_MALICOUS_PATTERN_INFO> mapPatternStatus;

	void OnSync(core::IFormatter& formatter)
	{
		formatter
			+ core::sPair(TEXT("PatternStatus"), mapPatternStatus)
			;
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

	core::UTF8::WriteJsonToFile(&stStatus, TEXT("d:/output.txt"));

	ST_FILELESS_STATUS stStatusRestored;
	core::UTF8::ReadJsonFromFile(&stStatusRestored, TEXT("d:/output.txt"));

	EXPECT_EQ(2U, stStatusRestored.mapPatternStatus.size());
}

struct ST_BIN_PACKET : public core::IFormatterObject
{
	int nValue;
	std::tstring strValue;
	std::vector<char> vecFileBinary;

	void OnSync(core::IFormatter& formatter)
	{
		formatter
			+ core::sPair(TEXT("nValue"), nValue)
			+ core::sPair(TEXT("strValue"), strValue)
			+ core::sPair(TEXT("FileBinary"), vecFileBinary)
			;
	}
};

TEST(FormatterTest, BinPacketTest)
{
	ST_BIN_PACKET stBinPacket;
	stBinPacket.nValue = 1005;
	stBinPacket.strValue = TEXT("10020");
	stBinPacket.vecFileBinary.resize(100, 5);

	std::vector<BYTE> packet;
	UTF8::WriteBinToPacket(&stBinPacket, packet);

	ST_BIN_PACKET stRestored;
	UTF8::ReadBinFromPacket(&stRestored, packet);

	ASSERT_EQ(stBinPacket.vecFileBinary.size(), stRestored.vecFileBinary.size());
	EXPECT_EQ(stBinPacket.nValue, stRestored.nValue);
	EXPECT_EQ(stBinPacket.strValue, stRestored.strValue);
	EXPECT_EQ(0, memcmp(stBinPacket.vecFileBinary.data(), stRestored.vecFileBinary.data(), stBinPacket.vecFileBinary.size()));
}