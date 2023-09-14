#include "stdafx.h"

struct ST_INTERNAL_DATA : public core::IFormatterObject
{
	int nInt;
	std::tstring strString;
	std::vector<char> vecFileBinary;

	void OnSync(core::IFormatter& formatter)
	{
		formatter
			+ core::sPair(TEXT("FileBinary"), vecFileBinary)
			+ core::sPair(TEXT("Int"), nInt)
			+ core::sPair(TEXT("String"), strString)
			;
	}
};

bool operator==(const ST_INTERNAL_DATA& lhs, const ST_INTERNAL_DATA& rhs)
{
	if (lhs.nInt != rhs.nInt)	return false;
	if (lhs.strString != rhs.strString)	return false;
	if (lhs.vecFileBinary.size() != rhs.vecFileBinary.size())	return false;
	if (!lhs.vecFileBinary.empty())
	{
		if (memcmp(lhs.vecFileBinary.data(), rhs.vecFileBinary.data(), lhs.vecFileBinary.size()))
			return false;
	}

	return true;
}

struct ST_BIN_PACKET : public core::IFormatterObject
{
	int nInt;
	std::tstring strString;
	std::list<ST_INTERNAL_DATA> listInternal;
	std::map<std::tstring, ST_INTERNAL_DATA> mapInternal;
	std::vector<char> vecFileBinary;

	void OnSync(core::IFormatter& formatter)
	{
		formatter
			+ core::sPair(TEXT("Int"), nInt)
			+ core::sPair(TEXT("String"), strString)
			+ core::sPair(TEXT("InternalList"), listInternal)
			+ core::sPair(TEXT("InternalMap"), mapInternal)
			+ core::sPair(TEXT("FileBinary"), vecFileBinary)
			;
	}
};

TEST(PacketFormatterTest, BinPacketTest)
{
	ST_INTERNAL_DATA stBinPacket;
	stBinPacket.nInt = 1005;
	stBinPacket.strString = TEXT("10020");
	stBinPacket.vecFileBinary.resize(100, 5);

	std::vector<BYTE> packet;
	WriteBinToPacket(&stBinPacket, packet);

	ST_INTERNAL_DATA stRestored;
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
	stBinPacket.vecFileBinary.resize(99, 0);
	stBinPacket.vecFileBinary[0] = '\"';

	{
		ST_INTERNAL_DATA stInternalData;
		stInternalData.nInt = 1;
		stInternalData.strString = TEXT("one");
		stInternalData.vecFileBinary.resize(15, 0);
		stBinPacket.listInternal.push_back(stInternalData);
		stBinPacket.mapInternal.insert(std::make_pair(TEXT("test1"), stInternalData));
	}

	{
		ST_INTERNAL_DATA stInternalData;
		stInternalData.nInt = 2;
		stInternalData.strString = TEXT("two");
		stInternalData.vecFileBinary.resize(155, 0xFF);
		stBinPacket.listInternal.push_back(stInternalData);
		stBinPacket.mapInternal.insert(std::make_pair(TEXT("test2"), stInternalData));
	}

	std::vector<BYTE> packet;
	WriteUBJsonToPacket(&stBinPacket, packet);

	ST_BIN_PACKET stRestored;
	ReadUBJsonFromPacket(&stRestored, packet);

	ASSERT_EQ(stBinPacket.vecFileBinary.size(), stRestored.vecFileBinary.size());
	EXPECT_EQ(stBinPacket.nInt, stRestored.nInt);
	EXPECT_EQ(stBinPacket.strString, stRestored.strString);
	EXPECT_EQ(0, memcmp(stBinPacket.vecFileBinary.data(), stRestored.vecFileBinary.data(), stBinPacket.vecFileBinary.size()));

	ASSERT_EQ(stBinPacket.listInternal.size(), stRestored.listInternal.size());
	{
		auto iter1 = stBinPacket.listInternal.begin();
		auto iter2 = stRestored.listInternal.begin();
		for (; iter1 != stBinPacket.listInternal.end(); iter1++, iter2++)
		{
			EXPECT_EQ(*iter1, *iter2);
		}
	}

	ASSERT_EQ(stBinPacket.mapInternal.size(), stRestored.mapInternal.size());
	{
		auto iter1 = stBinPacket.mapInternal.begin();
		auto iter2 = stRestored.mapInternal.begin();
		for (; iter1 != stBinPacket.mapInternal.end(); iter1++, iter2++)
		{
			EXPECT_EQ(iter1->first, iter2->first);
			EXPECT_EQ(iter1->second, iter2->second);
		}
	}
}

TEST(PacketFormatterTest, JsonPacketTest2)
{
	ST_BIN_PACKET stBinPacket;
	stBinPacket.nInt = 1005;
	stBinPacket.strString = TEXT("10020");
	stBinPacket.vecFileBinary.clear();

	{
		ST_INTERNAL_DATA stInternalData;
		stInternalData.nInt = 1;
		stInternalData.strString = TEXT("one");
		stBinPacket.listInternal.push_back(stInternalData);
		stBinPacket.mapInternal.insert(std::make_pair(TEXT("test1"), stInternalData));
	}

	{
		ST_INTERNAL_DATA stInternalData;
		stInternalData.nInt = 2;
		stInternalData.strString = TEXT("two");
		stBinPacket.listInternal.push_back(stInternalData);
		stBinPacket.mapInternal.insert(std::make_pair(TEXT("test2"), stInternalData));
	}

	std::vector<BYTE> packet;
	WriteUBJsonToPacket(&stBinPacket, packet);

	ST_BIN_PACKET stRestored;
	ReadUBJsonFromPacket(&stRestored, packet);

	ASSERT_EQ(stBinPacket.vecFileBinary.size(), stRestored.vecFileBinary.size());
	EXPECT_EQ(stBinPacket.nInt, stRestored.nInt);
	EXPECT_EQ(stBinPacket.strString, stRestored.strString);
	EXPECT_EQ(0, memcmp(stBinPacket.vecFileBinary.data(), stRestored.vecFileBinary.data(), stBinPacket.vecFileBinary.size()));

	ASSERT_EQ(stBinPacket.listInternal.size(), stRestored.listInternal.size());
	{
		auto iter1 = stBinPacket.listInternal.begin();
		auto iter2 = stRestored.listInternal.begin();
		for (; iter1 != stBinPacket.listInternal.end(); iter1++, iter2++)
		{
			EXPECT_EQ(iter1->nInt, iter2->nInt);
			EXPECT_EQ(iter1->strString, iter2->strString);
		}
	}

	ASSERT_EQ(stBinPacket.mapInternal.size(), stRestored.mapInternal.size());
	{
		auto iter1 = stBinPacket.mapInternal.begin();
		auto iter2 = stRestored.mapInternal.begin();
		for (; iter1 != stBinPacket.mapInternal.end(); iter1++, iter2++)
		{
			EXPECT_EQ(iter1->first, iter2->first);
			EXPECT_EQ(iter1->second.nInt, iter2->second.nInt);
			EXPECT_EQ(iter1->second.strString, iter2->second.strString);
		}
	}
}
