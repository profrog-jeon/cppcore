#include "stdafx.h"
#include <map>

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, TokenizeToArrayTest_VarietyDelimiter)
{
	std::tstring strContents = TEXT("1,2-3;4 5 6");

	std::vector<std::tstring> vecTokens;
	EXPECT_EQ(6U, TokenizeToArray(strContents, TEXT(",-; "), vecTokens));

	EXPECT_EQ(TEXT("1"), vecTokens[0]);
	EXPECT_EQ(TEXT("2"), vecTokens[1]);
	EXPECT_EQ(TEXT("3"), vecTokens[2]);
	EXPECT_EQ(TEXT("4"), vecTokens[3]);
	EXPECT_EQ(TEXT("5"), vecTokens[4]);
	EXPECT_EQ(TEXT("6"), vecTokens[5]);
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, TokenizeToArrayTest_EmptyToken_Include)
{
	std::tstring strContents = TEXT("1,-;4, 6");

	std::vector<std::tstring> vecTokens;
	EXPECT_EQ(6U, TokenizeToArray(strContents, TEXT(",-; "), vecTokens));

	EXPECT_EQ(TEXT("1"), vecTokens[0]);
	EXPECT_EQ(TEXT(""),  vecTokens[1]);
	EXPECT_EQ(TEXT(""),  vecTokens[2]);
	EXPECT_EQ(TEXT("4"), vecTokens[3]);
	EXPECT_EQ(TEXT(""),  vecTokens[4]);
	EXPECT_EQ(TEXT("6"), vecTokens[5]);
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, TokenizeToArrayTest_EmptyToken_Exclude)
{
	std::tstring strContents = TEXT("1,-;4, 6");

	std::vector<std::tstring> vecTokens;
	EXPECT_EQ(3U, TokenizeToArray(strContents, TEXT(",-; "), vecTokens, true));

	EXPECT_EQ(TEXT("1"), vecTokens[0]);
	EXPECT_EQ(TEXT("4"), vecTokens[1]);
	EXPECT_EQ(TEXT("6"), vecTokens[2]);
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, TokenizeTest2)
{
	std::tstring strContents = TEXT("1\n1\n");

	int nPos = 0;
	EXPECT_EQ(TEXT("1"), Tokenize(strContents, nPos));
	EXPECT_EQ(TEXT("1"), Tokenize(strContents, nPos));
	EXPECT_EQ(TEXT(""), Tokenize(strContents, nPos));
	EXPECT_EQ(-1, nPos);
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, TokenizeTest)
{
	std::tstring strContents = TEXT("\n\n");

	int nPos = 0;
	EXPECT_EQ(TEXT(""), Tokenize(strContents, nPos));
	EXPECT_EQ(TEXT(""), Tokenize(strContents, nPos));
	EXPECT_EQ(TEXT(""), Tokenize(strContents, nPos));
	EXPECT_EQ(-1, nPos);
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, RightTest)
{
	std::tstring strContents = TEXT("1234567890");
	EXPECT_EQ(TEXT("67890"), Right(strContents, 5));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, RightTest_FalseNegative)
{
	std::tstring strContents = TEXT("1234567890");
	EXPECT_EQ(TEXT("1234567890"), Right(strContents, -1));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, RightTest_FalseMaxExceed)
{
	std::tstring strContents = TEXT("1234567890");
	EXPECT_EQ(TEXT("1234567890"), Right(strContents, 11));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, RightTest_FalseZero)
{
	std::tstring strContents = TEXT("1234567890");
	EXPECT_EQ(TEXT(""), Right(strContents, 0));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, FormatTest)
{
	EXPECT_EQ(TEXT("123"), Format(TEXT("%d%u%c"), 1, 2U, TEXT('3')));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, MaxFormatTest_509)
{
	TCHAR szTest[512] = { 0,};
	size_t i;
	for(i=0; i<509; i++)
		szTest[i] = TEXT('*');

	std::tstring strTest(szTest);
	strTest = Format(TEXT("[%s]"), szTest);
	strTest += TEXT("\nshow!!");

	EXPECT_EQ(TEXT("show!!"), Right(strTest, 6));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, MaxFormatTest_510)
{
	TCHAR szTest[512] = { 0,};
	size_t i;
	for(i=0; i<510; i++)
		szTest[i] = TEXT('*');

	std::tstring strTest(szTest);
	strTest = Format(TEXT("[%s]"), szTest);
	strTest += TEXT("\nshow!!");

	EXPECT_EQ(TEXT("show!!"), Right(strTest, 6));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, MaxFormatTest_511)
{
	TCHAR szTest[512] = { 0,};
	size_t i;
	for(i=0; i<511; i++)
		szTest[i] = TEXT('*');

	std::tstring strTest(szTest);
	strTest = Format(TEXT("[%s]"), szTest);
	strTest += TEXT("\nshow!!");

	EXPECT_EQ(TEXT("show!!"), Right(strTest, 6));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, FormatTest_BigText)
{
	const size_t tSampleTextCch = 1024;
	TCHAR szSampleText[tSampleTextCch+1];

	size_t i;
	for(i=0; i<tSampleTextCch; i++)
		szSampleText[i] = TEXT('a');
	szSampleText[tSampleTextCch] = 0;

	const size_t tRepeatCount = 5;
	std::tstring strExpect;
	for(i=0; i<tRepeatCount; i++)
		strExpect += szSampleText;

	EXPECT_EQ(strExpect, Format(TEXT("%s%s%s%s%s"), szSampleText, szSampleText, szSampleText, szSampleText, szSampleText));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, ReplaceTest)
{
	const std::tstring strUTF8				= TEXT("   AbCdEfGhIjK   "	);
	const std::tstring strUTF8Replace		= TEXT("   AbCdZZZZZhIjK   ");

	std::tstring strBase = strUTF8;
	strBase = Replace(strBase, TEXT("EfG"), TEXT("ZZZZZ"));
	EXPECT_EQ(strUTF8Replace, strBase);

	std::tstring strTest = TEXT("%s %s");
	strTest = Replace(strTest, TEXT("%s"), TEXT("%S"));
	EXPECT_EQ(TEXT("%S %S"), strTest);
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, NestedReplaceTest)
{
	std::tstring strTest = TEXT("&&&");
	strTest = Replace(strTest, TEXT("&"), TEXT("&&&"));
	EXPECT_EQ(strTest, TEXT("&&&&&&&&&"));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, MakeLowerTest)
{
	std::tstring strTest					= TEXT("   AbCdEfGhIjK   ");
	std::tstring strExpect					= TEXT("   abcdefghijk   ");

	EXPECT_EQ(strExpect, MakeLower(strTest));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, MakeUpperTest)
{
	std::tstring strTest		= TEXT("hello World!!123");
	std::tstring strExpect		= TEXT("HELLO WORLD!!123");

	EXPECT_EQ(strExpect, MakeUpper(strTest));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, TrimTest)
{
	const std::tstring strOriginal			= TEXT("   AbCdEfGhIjK   "	);
	const std::tstring strLTrim				= TEXT("AbCdEfGhIjK   "		);
	const std::tstring strRTrim				= TEXT("   AbCdEfGhIjK"		);
	const std::tstring strTrim				= TEXT("AbCdEfGhIjK"		);	

	std::tstring strBase = strOriginal;
	strBase = TrimLeft(strBase);
	EXPECT_EQ(strLTrim, strBase);

	strBase = strOriginal;
	strBase = TrimRight(strBase);
	EXPECT_EQ(strRTrim, strBase);

	strBase = strOriginal;
	strBase = Trim(strBase);
	EXPECT_EQ(strTrim, strBase);
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, TrimTest_optional)
{
	std::tstring strContext = TEXT("aaabbcTESTxyyzzz");
	EXPECT_EQ(TEXT("TESTxyyzzz"), strContext = TrimLeft(strContext, TEXT("abc")));
	EXPECT_EQ(TEXT("TEST"), strContext = TrimRight(strContext, TEXT("xyz")));
	EXPECT_EQ(TEXT("ES"), Trim(strContext, TEXT("T")));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, TokenzeTest)
{
	int nOffset = 8;
	std::tstring strContext = TEXT("10,60,2,2014-11-17 10:18:01.997943");
	EXPECT_EQ(TEXT("2014-11-17 10:18:01.997943"), Tokenize(strContext, TEXT(","), nOffset));
	EXPECT_EQ(-1, nOffset);
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, MultipleTokenizeTest)
{
	int nPos = 0;

	EXPECT_EQ(TEXT("a"), Tokenize(TEXT("a.b.c.d.easdf"), TEXT("."), nPos));
	EXPECT_EQ(TEXT("b"), Tokenize(TEXT("a.b.c.d.easdf"), TEXT("."), nPos));
	EXPECT_EQ(TEXT("c"), Tokenize(TEXT("a.b.c.d.easdf"), TEXT("."), nPos));
	EXPECT_EQ(TEXT("d"), Tokenize(TEXT("a.b.c.d.easdf"), TEXT("."), nPos));
	EXPECT_EQ(TEXT("easdf"), Tokenize(TEXT("a.b.c.d.easdf"), TEXT("."), nPos));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, AbnormalMultipleTokenizeTest1)
{
	int nPos = 0;

	EXPECT_EQ(TEXT("a"), Tokenize(TEXT("a.b."), TEXT("."), nPos));
	EXPECT_EQ(TEXT("b"), Tokenize(TEXT("a.b."), TEXT("."), nPos));
	EXPECT_EQ(TEXT(""), Tokenize(TEXT("a.b."), TEXT("."), nPos));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, AbnormalMultipleTokenizeTest2)
{
	int nPos = 0;

	EXPECT_EQ(TEXT(""), Tokenize(TEXT(".."), TEXT("."), nPos));
	EXPECT_EQ(TEXT(""), Tokenize(TEXT(".."), TEXT("."), nPos));
	EXPECT_EQ(TEXT(""), Tokenize(TEXT(".."), TEXT("."), nPos));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, SwapTest)
{
	std::tstring strA = TEXT("B");
	std::tstring strB = TEXT("A");

	Swap(strA, strB);

	EXPECT_EQ(strA, TEXT("A"));
	EXPECT_EQ(strB, TEXT("B"));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLStringTest, SplitTest)
{
	std::tstring strA, strB;
	strA = Split(TEXT("A\r\nB"), TEXT("\r\n"), &strB);

	EXPECT_EQ(strA, TEXT("A"));
	EXPECT_EQ(strB, TEXT("B"));
}