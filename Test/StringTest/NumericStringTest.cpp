#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
TEST(NumericString, ValueFrom_UndefindedTest)
{
	{
		unsigned int tValue = 1;
		EXPECT_EQ(tValue, ValueFrom<unsigned int>(TEXT("1")));
	}
	
	{
		uint32_t tValue = 1;
		EXPECT_EQ(TEXT("1"), StringFrom(tValue));
	}

	{
		std::tstring strValue = TEXT("-1");
		EXPECT_EQ(TEXT("-1"), StringFrom(strValue));
	}
}

//////////////////////////////////////////////////////////////////////////
TEST(NumericString, ValueFrom_LongLongTest)
{
	EXPECT_EQ(-1, ValueFrom<int64_t>(TEXT("0xFFFFFFFFFFFFFFFF")));
	EXPECT_EQ(-1, ValueFrom<QWORD>(TEXT("0xFFFFFFFFFFFFFFFF")));

	EXPECT_EQ(0xFFFFFFFFFFFFFFFF, ValueFrom<int64_t>(TEXT("-1")));
	EXPECT_EQ(0xFFFFFFFFFFFFFFFF, ValueFrom<QWORD>(TEXT("-1")));
}

//////////////////////////////////////////////////////////////////////////
TEST(NumericString, CharFromTest)
{
	EXPECT_EQ('\n', CharFrom(TEXT("0x0A")));
	EXPECT_EQ('\n', CharFrom(TEXT("0xFF0A")));
}

//////////////////////////////////////////////////////////////////////////
TEST(NumericString, ShortFromTest)
{
	EXPECT_EQ(-240, ShortFrom(TEXT("0xFF10")));
	EXPECT_EQ(-240, ShortFrom(TEXT("0x10FFFF10")));
}

//////////////////////////////////////////////////////////////////////////
TEST(NumericString, UINTFromTest)
{
	EXPECT_EQ((BYTE)-1, BYTEFrom(TEXT("0xFFFFFFFFFFFFFFFF")));
	EXPECT_EQ((WORD)-1, WORDFrom(TEXT("0xFFFFFFFFFFFFFFFF")));
	EXPECT_EQ((DWORD)-1, DWORDFrom(TEXT("0xFFFFFFFFFFFFFFFF")));
	EXPECT_EQ((QWORD)-1, QWORDFrom(TEXT("0xFFFFFFFFFFFFFFFF")));
}

//////////////////////////////////////////////////////////////////////////
TEST(NumericString, MalformValueStringTest2)
{
	DoubleFrom(TEXT("-"));
}

//////////////////////////////////////////////////////////////////////////
TEST(NumericString, IntBase16ValueTest)
{
	int nValue = -1;
	EXPECT_EQ(TEXT("-1"), StringFrom(nValue));
	EXPECT_EQ(nValue, IntFrom(TEXT("0xFFFFFFFF")));
}

// Disable for MAC -> ambiguous(int and long is same) compile error
////////////////////////////////////////////////////////////////////////////
//TEST(NumericString, LongValueTest)
//{
//    long nValue = 123;
//    EXPECT_EQ(TEXT("123"), StringFrom(nValue));
//    EXPECT_EQ(nValue, IntFrom(TEXT("123")));
//}

//////////////////////////////////////////////////////////////////////////
TEST(NumericString, DoubleValueTest_NoDot_Positive)
{
	EXPECT_TRUE(DIFF(1234, DoubleFrom(TEXT("1234"))) < 0.00001);
}

//////////////////////////////////////////////////////////////////////////
TEST(NumericString, DoubleValueTest_NoDot_Negative)
{
	EXPECT_TRUE(DIFF(-1234.0, DoubleFrom(TEXT("-1234"))) < 0.00001);
}


//////////////////////////////////////////////////////////////////////////
TEST(NumericString, DoubleValueTest)
{
	EXPECT_TRUE(DIFF(1.234, DoubleFrom(TEXT("1.234"))) < 0.00001);
}

//////////////////////////////////////////////////////////////////////////
TEST(NumericString, DoubleValueTest_DoubleDot)
{
	EXPECT_TRUE(DIFF(1.23, DoubleFrom(TEXT("1.23.4"))) < 0.00001);
}

//////////////////////////////////////////////////////////////////////////
TEST(NumericString, DoubleValueTest_PositiveExponential)
{
	EXPECT_TRUE(DIFF(90.0, DoubleFrom(TEXT("9.000000e+01"))) < 0.00001);
}

//////////////////////////////////////////////////////////////////////////
TEST(NumericString, DoubleValueTest_NegativeExponential)
{
	EXPECT_TRUE(DIFF(0.9, DoubleFrom(TEXT("9.000000e-01"))) < 0.00001);
}

//////////////////////////////////////////////////////////////////////////
TEST(NumericString, DoubleStringTest)
{
	EXPECT_EQ(TEXT("1.234000"), StringFrom(1.234));
}

//////////////////////////////////////////////////////////////////////////
TEST(NumericString, IntTest)
{
	EXPECT_EQ(1234,			IntFrom(TEXT("1234")));
	EXPECT_EQ(-1234,		IntFrom(TEXT("-1234")));
	EXPECT_EQ((int)1234123412341234,	IntFrom(TEXT("1234123412341234")));
	EXPECT_EQ((int)-1234123412341234,	IntFrom(TEXT("-1234123412341234")));
	EXPECT_EQ(0,			IntFrom(TEXT("0")));
	EXPECT_EQ(0xFFFFFFFF,	IntFrom(TEXT("0xFFFFFFFF")));
	EXPECT_EQ(0xFFFFFFFF,	IntFrom(TEXT("0xFFFFFFFFFFFFFFFF")));
	EXPECT_EQ(0xFFFFFFFE,	IntFrom(TEXT("0xFFFFFFFFFFFFFFFE")));
}

//////////////////////////////////////////////////////////////////////////
TEST(NumericString, Int64Test)
{
	EXPECT_EQ(1234,			Int64From(TEXT("1234")));
	EXPECT_EQ(-1234,		Int64From(TEXT("-1234")));
	EXPECT_EQ(1234123412341234,		Int64From(TEXT("1234123412341234")));
	EXPECT_EQ(-1234123412341234,	Int64From(TEXT("-1234123412341234")));
	EXPECT_EQ(0,			Int64From(TEXT("0")));
	EXPECT_EQ(0xFFFFFFFF,	Int64From(TEXT("0xFFFFFFFF")));
	EXPECT_EQ(0xFFFFFFFFFFFFFFFF,	Int64From(TEXT("0xFFFFFFFFFFFFFFFF")));
	EXPECT_EQ(0xFFFFFFFFFFFFFFFF,	Int64From(TEXT("0xFFFFFFFFFFFFFFFFFFFFFFFF")));
	EXPECT_EQ(0xFFFFFFFFFFFFFFFE,	Int64From(TEXT("0xFFFFFFFFFFFFFFFFFFFFFFFE")));
}

//////////////////////////////////////////////////////////////////////////
TEST(NumericString, UIntTest)
{
	EXPECT_EQ(1234,			DWORDFrom(TEXT("1234")));
	EXPECT_EQ(0xfffffb2e,	DWORDFrom(TEXT("-1234")));
	EXPECT_EQ(0,			DWORDFrom(TEXT("0")));
	EXPECT_EQ(0xFFFFFFFF,	DWORDFrom(TEXT("0xFFFFFFFF")));
	EXPECT_EQ(0xFFFFFFFF,	DWORDFrom(TEXT("0xFFFFFFFFFFFFFFFF")));
	EXPECT_EQ(0xFFFFFFFE,	DWORDFrom(TEXT("0xFFFFFFFFFFFFFFFE")));
}

//////////////////////////////////////////////////////////////////////////
TEST(NumericString, UInt64Test)
{
	EXPECT_EQ(1234,			QWORDFrom(TEXT("1234")));
	EXPECT_EQ(0xfffffffffffffb2e,	QWORDFrom(TEXT("-1234")));
	EXPECT_EQ(0,			QWORDFrom(TEXT("0")));
	EXPECT_EQ(0xFFFFFFFF,	QWORDFrom(TEXT("0xFFFFFFFF")));
	EXPECT_EQ(0xFFFFFFFFFFFFFFFF,	QWORDFrom(TEXT("0xFFFFFFFFFFFFFFFF")));
	EXPECT_EQ(0xFFFFFFFFFFFFFFFF,	QWORDFrom(TEXT("0xFFFFFFFFFFFFFFFFFFFFFFFF")));
	EXPECT_EQ(0xFFFFFFFFFFFFFFFE,	QWORDFrom(TEXT("0xFFFFFFFFFFFFFFFFFFFFFFFE")));
}

