#include "stdafx.h"

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

TEST(PacketFormatterTest, JsonPacketTest2)
{
	ST_BIN_PACKET stBinPacket;
	stBinPacket.nValue = 1005;
	stBinPacket.strValue = TEXT("10020");
	stBinPacket.vecFileBinary.resize(99, 0);
	stBinPacket.vecFileBinary[0] = '\"';

	std::vector<BYTE> packet;
	UTF8::WriteJsonToPacket(&stBinPacket, packet);

	ST_BIN_PACKET stRestored;
	UTF8::ReadJsonFromPacket(&stRestored, packet);

	ASSERT_EQ(stBinPacket.vecFileBinary.size(), stRestored.vecFileBinary.size());
	EXPECT_EQ(stBinPacket.nValue, stRestored.nValue);
	EXPECT_EQ(stBinPacket.strValue, stRestored.strValue);
	EXPECT_EQ(0, memcmp(stBinPacket.vecFileBinary.data(), stRestored.vecFileBinary.data(), stBinPacket.vecFileBinary.size()));
}

TEST(PacketFormatterTest, BinPacketTest)
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

TEST(PacketFormatterTest, JsonPacketTest1)
{
	ST_BIN_PACKET stBinPacket;
	stBinPacket.nValue = 1005;
	stBinPacket.strValue = TEXT("10020");
	stBinPacket.vecFileBinary.resize(99, 5);

	std::vector<BYTE> packet;
	UTF8::WriteJsonToPacket(&stBinPacket, packet);

	ST_BIN_PACKET stRestored;
	UTF8::ReadJsonFromPacket(&stRestored, packet);

	ASSERT_EQ(stBinPacket.vecFileBinary.size(), stRestored.vecFileBinary.size());
	EXPECT_EQ(stBinPacket.nValue, stRestored.nValue);
	EXPECT_EQ(stBinPacket.strValue, stRestored.strValue);
	EXPECT_EQ(0, memcmp(stBinPacket.vecFileBinary.data(), stRestored.vecFileBinary.data(), stBinPacket.vecFileBinary.size()));
}

TEST(PacketFormatterTest, JsonPacketTest3)
{
	ST_BIN_PACKET stBinPacket;
	stBinPacket.nValue = 1005;
	stBinPacket.strValue = TEXT("10020");
	stBinPacket.vecFileBinary.resize(100, 0);
	stBinPacket.vecFileBinary[0] = '\"';

	std::vector<BYTE> packet;
	UTF8::WriteJsonToPacket(&stBinPacket, packet);

	ST_BIN_PACKET stRestored;
	UTF8::ReadJsonFromPacket(&stRestored, packet);

	ASSERT_EQ(stBinPacket.vecFileBinary.size(), stRestored.vecFileBinary.size());
	EXPECT_EQ(stBinPacket.nValue, stRestored.nValue);
	EXPECT_EQ(stBinPacket.strValue, stRestored.strValue);
	EXPECT_EQ(0, memcmp(stBinPacket.vecFileBinary.data(), stRestored.vecFileBinary.data(), stBinPacket.vecFileBinary.size()));
}

TEST(PacketFormatterTest, JsonPacketTest4)
{
	ST_BIN_PACKET stBinPacket;
	stBinPacket.nValue = 1005;
	stBinPacket.strValue = TEXT("10020");
	stBinPacket.vecFileBinary.clear();

	std::vector<BYTE> packet;
	UTF8::WriteJsonToPacket(&stBinPacket, packet);

	ST_BIN_PACKET stRestored;
	UTF8::ReadJsonFromPacket(&stRestored, packet);

	ASSERT_EQ(stBinPacket.vecFileBinary.size(), stRestored.vecFileBinary.size());
	EXPECT_EQ(stBinPacket.nValue, stRestored.nValue);
	EXPECT_EQ(stBinPacket.strValue, stRestored.strValue);
	EXPECT_EQ(0, memcmp(stBinPacket.vecFileBinary.data(), stRestored.vecFileBinary.data(), stBinPacket.vecFileBinary.size()));
}
