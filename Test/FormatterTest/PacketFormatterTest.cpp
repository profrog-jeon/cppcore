#include "stdafx.h"

struct ST_BIN_PACKET : public core::IFormatterObject
{
	int nInt;
	std::tstring strString;
	std::vector<char> vecFileBinary;

	void OnSync(core::IFormatter& formatter)
	{
		formatter
			+ core::sPair(TEXT("Int"), nInt)
			+ core::sPair(TEXT("String"), strString)
			+ core::sPair(TEXT("FileBinary"), vecFileBinary)
			;
	}
};

TEST(PacketFormatterTest, JsonPacketTest2)
{
	ST_BIN_PACKET stBinPacket;
	stBinPacket.nInt = 1005;
	stBinPacket.strString = TEXT("10020");
	stBinPacket.vecFileBinary.resize(99, 0);
	stBinPacket.vecFileBinary[0] = '\"';

	std::vector<BYTE> packet;
	WriteUBJsonToPacket(&stBinPacket, packet);

	ST_BIN_PACKET stRestored;
	ReadUBJsonFromPacket(&stRestored, packet);

	ASSERT_EQ(stBinPacket.vecFileBinary.size(), stRestored.vecFileBinary.size());
	EXPECT_EQ(stBinPacket.nInt, stRestored.nInt);
	EXPECT_EQ(stBinPacket.strString, stRestored.strString);
	EXPECT_EQ(0, memcmp(stBinPacket.vecFileBinary.data(), stRestored.vecFileBinary.data(), stBinPacket.vecFileBinary.size()));
}

TEST(PacketFormatterTest, BinPacketTest)
{
	ST_BIN_PACKET stBinPacket;
	stBinPacket.nInt = 1005;
	stBinPacket.strString = TEXT("10020");
	stBinPacket.vecFileBinary.resize(100, 5);

	std::vector<BYTE> packet;
	WriteBinToPacket(&stBinPacket, packet);

	ST_BIN_PACKET stRestored;
	ReadBinFromPacket(&stRestored, packet);

	ASSERT_EQ(stBinPacket.vecFileBinary.size(), stRestored.vecFileBinary.size());
	EXPECT_EQ(stBinPacket.nInt, stRestored.nInt);
	EXPECT_EQ(stBinPacket.strString, stRestored.strString);
	EXPECT_EQ(0, memcmp(stBinPacket.vecFileBinary.data(), stRestored.vecFileBinary.data(), stBinPacket.vecFileBinary.size()));
}

TEST(PacketFormatterTest, JsonPacketTest1)
{
	ST_BIN_PACKET stBinPacket;
	stBinPacket.nInt = 1005;
	stBinPacket.strString = TEXT("10020");
	stBinPacket.vecFileBinary.resize(99, 5);

	std::vector<BYTE> packet;
	WriteUBJsonToPacket(&stBinPacket, packet);

	ST_BIN_PACKET stRestored;
	ReadUBJsonFromPacket(&stRestored, packet);

	ASSERT_EQ(stBinPacket.vecFileBinary.size(), stRestored.vecFileBinary.size());
	EXPECT_EQ(stBinPacket.nInt, stRestored.nInt);
	EXPECT_EQ(stBinPacket.strString, stRestored.strString);
	EXPECT_EQ(0, memcmp(stBinPacket.vecFileBinary.data(), stRestored.vecFileBinary.data(), stBinPacket.vecFileBinary.size()));
}

TEST(PacketFormatterTest, JsonPacketTest3)
{
	ST_BIN_PACKET stBinPacket;
	stBinPacket.nInt = 1005;
	stBinPacket.strString = TEXT("10020");
	stBinPacket.vecFileBinary.resize(100, 0);
	stBinPacket.vecFileBinary[0] = '\"';

	std::vector<BYTE> packet;
	WriteUBJsonToPacket(&stBinPacket, packet);

	ST_BIN_PACKET stRestored;
	ReadUBJsonFromPacket(&stRestored, packet);

	ASSERT_EQ(stBinPacket.vecFileBinary.size(), stRestored.vecFileBinary.size());
	EXPECT_EQ(stBinPacket.nInt, stRestored.nInt);
	EXPECT_EQ(stBinPacket.strString, stRestored.strString);
	EXPECT_EQ(0, memcmp(stBinPacket.vecFileBinary.data(), stRestored.vecFileBinary.data(), stBinPacket.vecFileBinary.size()));
}

TEST(PacketFormatterTest, JsonPacketTest4)
{
	ST_BIN_PACKET stBinPacket;
	stBinPacket.nInt = 1005;
	stBinPacket.strString = TEXT("10020");
	stBinPacket.vecFileBinary.clear();

	std::vector<BYTE> packet;
	WriteUBJsonToPacket(&stBinPacket, packet);

	ST_BIN_PACKET stRestored;
	ReadUBJsonFromPacket(&stRestored, packet);

	ASSERT_EQ(stBinPacket.vecFileBinary.size(), stRestored.vecFileBinary.size());
	EXPECT_EQ(stBinPacket.nInt, stRestored.nInt);
	EXPECT_EQ(stBinPacket.strString, stRestored.strString);
	EXPECT_EQ(0, memcmp(stBinPacket.vecFileBinary.data(), stRestored.vecFileBinary.data(), stBinPacket.vecFileBinary.size()));
}
