#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
TEST(EncoderTest, DecodeBase64A_AbNormalTest)
{
	DecodeBase64("a", NULL);
	DecodeBase64("ab", NULL);
	DecodeBase64("abc", NULL);
	DecodeBase64("abcd", NULL);

	DecodeBase64("!", NULL);
	DecodeBase64("@#", NULL);
	DecodeBase64("$%^", NULL);
	DecodeBase64("&*()", NULL);
}

//////////////////////////////////////////////////////////////////////////
TEST(EncoderTest, Base64A_NormalTest)
{
	std::string strPlainText =	"Man is distinguished, not only by his reason, but by this singular "
								"passion from other animals, which is a lust of the mind, that by a "
								"perseverance of delight in the continued and indefatigable generation "
								"of knowledge, exceeds the short vehemence of any carnal pleasure.";
	std::string strBase64 = 	"TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0"
								"aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1"
								"c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0"
								"aGUgY29udGludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdl"
								"LCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=";

	std::string strEncoded = EncodeBase64A((LPBYTE)strPlainText.c_str(), strPlainText.length());
	ASSERT_EQ(strBase64, strEncoded);

	std::string strRestored;
	strRestored.resize(DecodeBase64(strEncoded, NULL));
	DecodeBase64(strEncoded, (LPBYTE)strRestored.c_str());
	ASSERT_EQ(strPlainText, strRestored);
}

//////////////////////////////////////////////////////////////////////////
TEST(EncoderTest, Base64W_NormalTest)
{
	std::string strPlainText =	"Man is distinguished, not only by his reason, but by this singular "
								"passion from other animals, which is a lust of the mind, that by a "
								"perseverance of delight in the continued and indefatigable generation "
								"of knowledge, exceeds the short vehemence of any carnal pleasure.";
	std::wstring strBase64 = 	L"TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0"
								L"aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1"
								L"c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0"
								L"aGUgY29udGludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdl"
								L"LCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=";

	std::wstring strEncoded = EncodeBase64W((LPBYTE)strPlainText.c_str(), strPlainText.length());
	ASSERT_EQ(strBase64, strEncoded);

	std::string strRestored;
	strRestored.resize(DecodeBase64(strEncoded, NULL));
	DecodeBase64(strEncoded, (LPBYTE)strRestored.c_str());
	ASSERT_EQ(strPlainText, strRestored);
}

//////////////////////////////////////////////////////////////////////////
TEST(EncoderTest, Base64T_NormalTest)
{
	std::string strPlainText =	"Man is distinguished, not only by his reason, but by this singular "
								"passion from other animals, which is a lust of the mind, that by a "
								"perseverance of delight in the continued and indefatigable generation "
								"of knowledge, exceeds the short vehemence of any carnal pleasure.";
	std::tstring strBase64 = 	TEXT("TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0")
								TEXT("aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1")
								TEXT("c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0")
								TEXT("aGUgY29udGludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdl")
								TEXT("LCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=");

	std::tstring strEncoded = EncodeBase64((LPBYTE)strPlainText.c_str(), strPlainText.length());
	ASSERT_EQ(strBase64, strEncoded);

	std::string strRestored;
	strRestored.resize(DecodeBase64(strEncoded, NULL));
	DecodeBase64(strEncoded, (LPBYTE)strRestored.c_str());
	ASSERT_EQ(strPlainText, strRestored);
}

//////////////////////////////////////////////////////////////////////////
TEST(EncoderTest, Base64_1_Byte_Test)
{
	std::string strPlainText =	"a";
	std::tstring strBase64 = 	TEXT("YQ==");

	std::tstring strEncoded = EncodeBase64((LPBYTE)strPlainText.c_str(), strPlainText.length());
	ASSERT_EQ(strBase64, strEncoded);

	std::string strRestored;
	strRestored.resize(DecodeBase64(strEncoded, NULL));
	DecodeBase64(strEncoded, (LPBYTE)strRestored.c_str());
	ASSERT_EQ(strPlainText, strRestored);
}

//////////////////////////////////////////////////////////////////////////
TEST(EncoderTest, Base64_2_Byte_Test)
{
	std::string strPlainText =	"aa";
	std::tstring strBase64 = 	TEXT("YWE=");

	std::tstring strEncoded = EncodeBase64((LPBYTE)strPlainText.c_str(), strPlainText.length());
	ASSERT_EQ(strBase64, strEncoded);

	std::string strRestored;
	strRestored.resize(DecodeBase64(strEncoded, NULL));
	DecodeBase64(strEncoded, (LPBYTE)strRestored.c_str());
	ASSERT_EQ(strPlainText, strRestored);
}

//////////////////////////////////////////////////////////////////////////
TEST(EncoderTest, Base64_3_Byte_Test)
{
	std::string strPlainText =	"aaa";
	std::tstring strBase64 = 	TEXT("YWFh");

	std::tstring strEncoded = EncodeBase64((LPBYTE)strPlainText.c_str(), strPlainText.length());
	ASSERT_EQ(strBase64, strEncoded);

	std::string strRestored;
	strRestored.resize(DecodeBase64(strEncoded, NULL));
	DecodeBase64(strEncoded, (LPBYTE)strRestored.c_str());
	ASSERT_EQ(strPlainText, strRestored);
}

//////////////////////////////////////////////////////////////////////////
TEST(EncoderTest, Base64_Slash_Test)
{
	std::string strPlainText =	"Q^=\\vrRf}pDOJ7B7hyLSK)R^34^lt5t_t$9>Uo2\"tLosp-h3O5&y]*%@7kXa\'dN_p`ta-^;E5G{SBXP2zD4.1%^iu8&*$vsNyv";
	std::tstring strBase64 = 	TEXT("UV49XHZyUmZ9cERPSjdCN2h5TFNLKVJeMzRebHQ1dF90JDk+VW8yInRMb3NwLWgzTzUmeV0qJUA3a1hhJ2ROX3BgdGEtXjtFNUd7U0J/WFAyekQ0LjElXml1OCYqJHZzTnl2");	

	std::tstring strEncoded = EncodeBase64((LPBYTE)strPlainText.c_str(), strPlainText.length());
	ASSERT_EQ(strBase64, strEncoded);

	std::string strRestored;
	strRestored.resize(DecodeBase64(strEncoded, NULL));
	DecodeBase64(strEncoded, (LPBYTE)strRestored.c_str());
	ASSERT_EQ(strPlainText, strRestored);
}

//////////////////////////////////////////////////////////////////////////
std::string BuildRandomString(void)
{
	const size_t tBuffLen = 1024;
	char szBuff[tBuffLen];

	size_t tLen = rand() % 90 + 10;
	size_t i;
	for(i=0; i<tLen; i++)
		szBuff[i] = 32 + rand() % 96;
	szBuff[i] = 0;
	return szBuff;
}

//////////////////////////////////////////////////////////////////////////
TEST(EncoderTest, Base64_Random_Test)
{
	size_t i;
	for(i=0; i<1000; i++)
	{
		std::string strPlainText = BuildRandomString();
		std::tstring strEncoded = EncodeBase64((LPBYTE)strPlainText.c_str(), strPlainText.length());
		std::string strRestored;
		strRestored.resize(DecodeBase64(strEncoded, NULL));
		DecodeBase64(strEncoded, (LPBYTE)strRestored.c_str());
		ASSERT_EQ(strPlainText, strRestored);
	}
}
