#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
TEST(StringTest, HexToStringConvert)
{
	BYTE btOriginalData[16] = { 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70
							  , 0x80, 0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0 };
	BYTE btRestoredData[16] = { 0, };

	std::tstring strConvertedContext = StringFromHex(btOriginalData, sizeof(btOriginalData));
	EXPECT_EQ(TEXT("00102030405060708090A0B0C0D0E0F0"), strConvertedContext);
	HexFromString(btRestoredData, sizeof(btRestoredData), strConvertedContext);
	EXPECT_EQ(0, memcmp(btOriginalData, btRestoredData, sizeof(btOriginalData)));
}

//////////////////////////////////////////////////////////////////////////
TEST(StringTest, StringToHexConvert)
{
	std::tstring strContext = TEXT("0123456789ABCDEF");
	BYTE btData[8] = { 0, };
	HexFromString(btData, sizeof(btData), strContext);
	std::tstring strRestored = StringFromHex(btData, sizeof(btData));
	EXPECT_EQ(strContext, strRestored);
}