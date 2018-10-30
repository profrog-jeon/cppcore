#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
TEST(STLString, StringCchCopyN_ExceedFailureTest)
{
	const size_t tMaxCch = 5;
	TCHAR szBuffer[tMaxCch] = { 0, };
	EXPECT_EQ(0, StringCchCopyN(szBuffer, tMaxCch, TEXT("abc"), 50));

	TCHAR szExpect[tMaxCch] = TEXT("abc\0");
	EXPECT_EQ(0, memcmp(szBuffer, szExpect, tMaxCch * sizeof(TCHAR)));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLString, StringCchCopy_FillFailureTest)
{
	const size_t tMaxCch = 5;
	TCHAR szBuffer[tMaxCch] = { 0, };
	EXPECT_NE(0, StringCchCopyEx(szBuffer, tMaxCch, TEXT("abcde"), NULL, NULL, STRSAFE_FILL_ON_FAILURE_|STRSAFE_FAILURE_BYTE_(0xFF)));

	TCHAR szExpect[tMaxCch];
	memset(szExpect, 0xFF, sizeof(TCHAR) * tMaxCch);
	szExpect[tMaxCch-1] = 0;
	EXPECT_EQ(0, memcmp(szBuffer, szExpect, tMaxCch * sizeof(TCHAR)));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLString, StringCchCopy_ExceedTest)
{
	const size_t tMaxCch = 5;
	TCHAR szBuffer[tMaxCch] = { 0, };
	EXPECT_NE(0, StringCchCopyEx(szBuffer, tMaxCch, TEXT("abcde"), NULL, NULL, STRSAFE_NO_TRUNCATION_));
	EXPECT_EQ(0, szBuffer[0]);
	EXPECT_NE(0, StringCchCopyEx(szBuffer, tMaxCch, TEXT("abcde"), NULL, NULL, 0));
	EXPECT_EQ(TEXT('a'), szBuffer[0]);
}

//////////////////////////////////////////////////////////////////////////
TEST(STLString, StringCchCat_ExceedTest)
{
	const size_t tMaxCch = 5;
	TCHAR szBuffer[tMaxCch] = { 0, };
	EXPECT_EQ(0, StringCchCatEx(szBuffer, tMaxCch, TEXT("a"), NULL, NULL, STRSAFE_NO_TRUNCATION_));
	EXPECT_NE(0, StringCchCatEx(szBuffer, tMaxCch, TEXT("abcde"), NULL, NULL, STRSAFE_NO_TRUNCATION_));
	EXPECT_EQ(TEXT('a'), szBuffer[0]);
	EXPECT_EQ(0, szBuffer[1]);
	EXPECT_NE(0, StringCchCatEx(szBuffer, tMaxCch, TEXT("abcde"), NULL, NULL, 0));
	EXPECT_EQ(TEXT('a'), szBuffer[0]);
}

//////////////////////////////////////////////////////////////////////////
TEST(STLString, StringCchLength_ExceedTest)
{
	size_t tSize = 0;
	EXPECT_NE(0, StringCchLength(TEXT("abcde"), 5, &tSize));
	EXPECT_EQ(0, tSize);
}

//////////////////////////////////////////////////////////////////////////
TEST(STLString, StringCchCopy_NullSrcTest)
{
	const size_t tDataSize = 10;
	TCHAR szData[tDataSize];
	EXPECT_EQ(0, StringCbCopy(szData, tDataSize, TEXT("")));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLString, StringCchCopy_ExcceedSrcSizeTest)
{
	const size_t tDataSize = 10;
	TCHAR szData[tDataSize];
	EXPECT_NE(0, StringCbCopy(szData, tDataSize, TEXT("aaaaaaaaaaaaa")));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLString, StringEx_EndPtrTest)
{
	const size_t tBuffSize = 4 * sizeof(TCHAR);
	BYTE btData[tBuffSize];
	LPTSTR pszEndPtr = NULL;
	size_t tRemained;
	EXPECT_EQ(0, StringCbCopyEx((LPTSTR)btData, tBuffSize, TEXT("A"), &pszEndPtr, &tRemained, 0));
	EXPECT_EQ((void*)&btData[sizeof(TCHAR)], (void*)pszEndPtr);
	EXPECT_EQ(tBuffSize - sizeof(TCHAR), tRemained);
}

//////////////////////////////////////////////////////////////////////////
TEST(STLString, StringEx_FlagTest_NO_TRUNCATION)
{
	const size_t tBuffSize = 4 * sizeof(TCHAR);
	BYTE btData[tBuffSize] = { 0xFF, 0xFF, 0xFF, 0xFF };
	EXPECT_EQ(0, StringCbCopyEx((LPTSTR)btData, tBuffSize, TEXT(""), NULL, NULL, STRSAFE_NO_TRUNCATION_|STRSAFE_FILL_BYTE_(0x0F)));

	BYTE btExpect[tBuffSize];
	memset(btExpect, 0x0F, tBuffSize);
	((TCHAR*)btExpect)[0] = 0;
	EXPECT_EQ(0, memcmp(btData, btExpect, tBuffSize));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLString, StringEx_FlagTest_NULL_ON_FAILURE)
{
	LPCTSTR pszTest = TEXT("abcde");
	const size_t tBuffSize = 4 * sizeof(TCHAR);
	BYTE btData[tBuffSize] = { 0xFF, 0 };
	EXPECT_NE(0, StringCbCopyEx((LPTSTR)btData, tBuffSize, pszTest, NULL, NULL, STRSAFE_NULL_ON_FAILURE_|STRSAFE_FILL_BYTE_(0x0F)));

	BYTE btExpect[tBuffSize];
	memcpy(btExpect, pszTest, tBuffSize);
	((TCHAR*)btExpect)[0] = 0;
	((TCHAR*)btExpect)[3] = 0;
	EXPECT_EQ(0, memcmp(btData, btExpect, tBuffSize));

	EXPECT_EQ(0, StringCbCopyEx((LPTSTR)btData, tBuffSize, TEXT(""), NULL, NULL, STRSAFE_NULL_ON_FAILURE_|STRSAFE_FILL_BYTE_(0x0F)));
	memset(btExpect, 0x0F, tBuffSize);
	((TCHAR*)btExpect)[0] = 0;
	EXPECT_EQ(0, memcmp(btData, btExpect, tBuffSize));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLString, StringEx_FlagTest_FILL_ON_FAILURE)
{
	const size_t tBuffSize = 4 * sizeof(TCHAR);
	BYTE btData[tBuffSize];
	EXPECT_NE(0, StringCbCopyEx((LPTSTR)btData, tBuffSize, TEXT("abcde"), NULL, NULL, STRSAFE_FILL_ON_FAILURE_|STRSAFE_FILL_BYTE_(0x0F)));

	BYTE btExpect[tBuffSize];
	memset(btExpect, 0x0F, tBuffSize);
	((TCHAR*)btExpect)[3] = 0;
	EXPECT_EQ(0, memcmp(btData, btExpect,  tBuffSize));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLString, StringEx_FlagTest_FILL_BEHIND_NULL)
{
	const size_t tBuffSize = 4 * sizeof(TCHAR);
	BYTE btData[tBuffSize] = { 0xFF, 0 };
	EXPECT_EQ(0, StringCbCopyEx((LPTSTR)btData, tBuffSize, TEXT(""), NULL, NULL, STRSAFE_FILL_BEHIND_NULL_|STRSAFE_FILL_BYTE_(0x0F)));

	BYTE btExpect[tBuffSize];
	memset(btExpect, 0x0F, tBuffSize);
	((TCHAR*)btExpect)[0] = 0;
	EXPECT_EQ(0, memcmp(btData, btExpect,  tBuffSize));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLString, StringEx_FlagTest_IGNORE_NULLS)
{
	const size_t tBuffSize = 4;
	BYTE btData[tBuffSize] = { 0xFF, 0 };
	EXPECT_EQ(0, StringCbCopyEx((LPTSTR)btData, tBuffSize, NULL, NULL, NULL, STRSAFE_IGNORE_NULLS_));

	DWORD dwData = 0;
	EXPECT_EQ(*(DWORD*)btData, dwData);
}

//////////////////////////////////////////////////////////////////////////
TEST(STLString, StringCopyTest)
{
	TCHAR szDest[5];
	EXPECT_NE(0, StringCbCopy(szDest, sizeof(TCHAR) * 2, TEXT("Hello?")));
	EXPECT_STREQ(TEXT("H"), szDest);

	TCHAR szDest2[5];
	EXPECT_NE(0, StringCchCopy(szDest2, 5, TEXT("Hello?")));
	EXPECT_STREQ(TEXT("Hell"), szDest2);
}

//////////////////////////////////////////////////////////////////////////
TEST(STLString, StringCopyNTest)
{
	const size_t tCchCount = 10;
	TCHAR szDest[tCchCount];
	EXPECT_NE(0, StringCbCopyN(szDest, sizeof(TCHAR) * 5, TEXT("Hello?"), tCchCount));

	TCHAR szDest2[tCchCount];
	EXPECT_EQ(0, StringCchCopyN(szDest2, tCchCount, TEXT("Hello?"), tCchCount));
	EXPECT_STREQ(TEXT("Hello?"), szDest2);
}

//////////////////////////////////////////////////////////////////////////
TEST(STLString, StringCatTest)
{
	const size_t tCchCount = 10;

	TCHAR szDest[tCchCount] = { 0, };
	EXPECT_EQ(0, StringCbCat(szDest, sizeof(TCHAR) * 5, TEXT("Hel?")));
	EXPECT_NE(0, StringCbCat(szDest, sizeof(TCHAR) * 5, TEXT("Hel?")));
	EXPECT_STREQ(TEXT("Hel?"), szDest);

	TCHAR szDest2[tCchCount] = { 0, };
	EXPECT_EQ(0, StringCchCat(szDest2, tCchCount, TEXT("Hel?")));
	EXPECT_STREQ(TEXT("Hel?"), szDest2);
	EXPECT_EQ(0, StringCchCat(szDest2, tCchCount, TEXT("Hel?")));
	EXPECT_STREQ(TEXT("Hel?Hel?"), szDest2);
}

//////////////////////////////////////////////////////////////////////////
TEST(STLString, StringCatNTest)
{
	const size_t tCchCount = 10;

	TCHAR szDest[tCchCount] = { 0, };
	EXPECT_EQ(0, StringCbCatN(szDest, sizeof(TCHAR) * 5, TEXT("Hel?"), sizeof(TCHAR) * 2));
	EXPECT_STREQ(TEXT("He"), szDest);
	EXPECT_EQ(0, StringCbCatN(szDest, sizeof(TCHAR) * 5, TEXT("Hel?"), sizeof(TCHAR) * 2));
	EXPECT_STREQ(TEXT("HeHe"), szDest);
	EXPECT_NE(0, StringCbCatN(szDest, sizeof(TCHAR) * 5, TEXT("Hel?"), sizeof(TCHAR) * 2));

	TCHAR szDest2[tCchCount] = { 0, };
	EXPECT_EQ(0, StringCchCatN(szDest2, tCchCount, TEXT("Hel?"), 5));
	EXPECT_STREQ(TEXT("Hel?"), szDest2);
	EXPECT_EQ(0, StringCchCatN(szDest2, tCchCount, TEXT("Hel?"), 5));
	EXPECT_STREQ(TEXT("Hel?Hel?"), szDest2);
}

//////////////////////////////////////////////////////////////////////////
TEST(STLString, StringPrintfTest)
{
	const size_t tCchCount = 10;

	TCHAR szDest[tCchCount];
	EXPECT_NE(0, StringCbPrintf(szDest, sizeof(TCHAR) * 5, TEXT("Hello?")));
	EXPECT_STREQ(TEXT("Hell"), szDest);

	TCHAR szDest2[tCchCount];
	EXPECT_EQ(0, StringCchPrintf(szDest2, tCchCount, TEXT("Hello?")));
	EXPECT_STREQ(TEXT("Hello?"), szDest2);
}

//////////////////////////////////////////////////////////////////////////
TEST(STLString, StringLengthTest)
{
	size_t tLength = 0;
	EXPECT_NE(0, StringCbLength(TEXT("Hey?"), sizeof(TCHAR) * 2, &tLength));
	EXPECT_EQ(0, tLength);

	EXPECT_EQ(0, StringCchLength(TEXT("Hey?"), 5, &tLength));
	EXPECT_EQ(4, tLength);
}

//////////////////////////////////////////////////////////////////////////
TEST(STLString, SafeStrCmpTest)
{
	EXPECT_EQ(0, SafeStrCmp(TEXT("test1"), TEXT("test2"), 2));
	EXPECT_EQ(0, SafeStrCmp(TEXT("test1"), TEXT("test2"), 4));
	EXPECT_NE(0, SafeStrCmp(TEXT("test1"), TEXT("test2"), 5));
	EXPECT_EQ(0, SafeStrCmp(TEXT("test"), TEXT("test"), 40));
	EXPECT_NE(0, SafeStrCmp(TEXT("test"), TEXT("test test"), 40));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLString, WildCardComparationTest_multiquestion)
{
	EXPECT_FALSE(StrCmpWithWildcard(TEXT("/Root"), TEXT("*??????")));
	EXPECT_TRUE(StrCmpWithWildcard(TEXT("/Root"), TEXT("????*")));
	EXPECT_TRUE(StrCmpWithWildcard(TEXT("/Root"), TEXT("?????*")));
	EXPECT_FALSE(StrCmpWithWildcard(TEXT("/Root"), TEXT("??????*")));
	
	EXPECT_TRUE(StrCmpWithWildcard(TEXT("/Root"), TEXT("*?????")));
	EXPECT_TRUE(StrCmpWithWildcard(TEXT("/Root"), TEXT("*????")));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLString, WildCardComparationTest_multicase)
{
	EXPECT_TRUE(StrCmpWithWildcard(TEXT("/a/b/c/d/efg"), TEXT("*/?fg")));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLString, WildCardComparationTest_multicase_wildcardgroup)
{
	EXPECT_FALSE(StrCmpWithWildcard(TEXT("/a/b/c/d/efg"), TEXT("**??*???*????*/?fg")));
	EXPECT_TRUE(StrCmpWithWildcard(TEXT("/a/b/c/d/efg"), TEXT("**??*?????*/?fg")));
}

//////////////////////////////////////////////////////////////////////////
TEST(STLString, WildCardComparationTest)
{
	EXPECT_TRUE(StrCmpWithWildcard(TEXT("www.microsoft.co.kr"), TEXT("?ww.micro??ft.co.k?")));
	EXPECT_FALSE(StrCmpWithWildcard(TEXT("wwww.microsoft.co.kr"), TEXT("?ww.micro??ft.co.k?")));

	EXPECT_TRUE(StrCmpWithWildcard(TEXT("www.microsoft.co.kr"), TEXT("www.*")));
	EXPECT_FALSE(StrCmpWithWildcard(TEXT("wwww.microsoft.co.kr"), TEXT("www.*")));

	EXPECT_TRUE(StrCmpWithWildcard(TEXT("www.microsoft123124124.co.kr"), TEXT("*.mi*o*f*.co.kr")));
	EXPECT_FALSE(StrCmpWithWildcard(TEXT("www.microsoft123124124.co.krasdf"), TEXT("*.mi*o*f*.co.kr")));
}

//////////////////////////////////////////////////////////////////////////
TEST(StdStringTest, SafeSPrintfTest)
{
	const size_t tBuffLen = 256;
	char szBuff[tBuffLen];
	wchar_t szWide[] = L"Sample";
	SafeSPrintf(szBuff, tBuffLen, "%s", szWide);
	EXPECT_STREQ("S", szBuff);
	SafeSPrintf(szBuff, tBuffLen, "%S", szWide);
	EXPECT_STREQ("Sample", szBuff);
}

//////////////////////////////////////////////////////////////////////////
TEST(StdStringTest, SafeStrLenTest)
{
	EXPECT_EQ(0, SafeStrLen(TEXT(""), 1));
	EXPECT_EQ(5, SafeStrLen(TEXT("abcde"), 6));
}

//////////////////////////////////////////////////////////////////////////
TEST(StdStringTest, SafeStrCpyTest)
{
	DWORD a = 0xAAAAAAAA;
	DWORD b = 0xBBBBBBBB;
	TCHAR szDest[8] = { 0, };
	DWORD f = 0xFFFFFFFF;
	SafeStrCpy(szDest, 8, TEXT("12345678901234567890"));
	EXPECT_EQ(0xAAAAAAAA, a);
	EXPECT_EQ(0xBBBBBBBB, b);
	EXPECT_EQ(0xFFFFFFFF, f);
}

//////////////////////////////////////////////////////////////////////////
TEST(StdStringTest, SafeSPrintTest)
{
	const size_t tDestCch = 12;
	TCHAR szDest[tDestCch] = { 0, };
	EXPECT_EQ(3, SafeSPrintf(szDest, tDestCch, TEXT("%d%u%c"), 1, 2U, TEXT('3')));
	EXPECT_STREQ(szDest, TEXT("123"));

	EXPECT_EQ(-1, SafeSPrintf(szDest, tDestCch, TEXT("%d%u%s"), 1234, 5678U, TEXT("9012")));
	EXPECT_STREQ(szDest, TEXT("12345678901"));
 }

//////////////////////////////////////////////////////////////////////////
TEST(StdStringTest, SafeFindStrTest)
{
	EXPECT_EQ(0, SafeFindStr(TEXT("abcde"), 5, TEXT("abcde")));
	EXPECT_EQ(5, SafeFindStr(TEXT("12345abcde"), 10, TEXT("abcde")));
}

//////////////////////////////////////////////////////////////////////////
TEST(StdStringTest, SafeReverseFindStrTest)
{
	EXPECT_EQ(5, SafeReverseFindStr(TEXT("abcdeabcde"), 10, TEXT("abcde")));
	EXPECT_EQ(10, SafeReverseFindStr(TEXT("abcde12345abcde"), 15, TEXT("abcde")));
	EXPECT_EQ(10, SafeReverseFindStr(TEXT("abcde12345abcde"), 15, TEXT("a")));
	EXPECT_EQ(14, SafeReverseFindStr(TEXT("abcde12345abcde"), 15, TEXT("e")));
}

//////////////////////////////////////////////////////////////////////////
TEST(StdStringTest, SafeReverseFindStr_PosTest)
{
	EXPECT_EQ(6, SafeReverseFindStr(TEXT("123123123"), 10, TEXT("123")));
	EXPECT_EQ(6, SafeReverseFindStr(TEXT("123123123"), 10, TEXT("123"), 6));
	EXPECT_EQ(3, SafeReverseFindStr(TEXT("123123123"), 10, TEXT("123"), 5));
	EXPECT_EQ(0, SafeReverseFindStr(TEXT("123123123"), 10, TEXT("123"), 0));
	EXPECT_EQ(-1, SafeReverseFindStr(TEXT("0123123123"), 10, TEXT("123"), 0));
}

