#include "stdafx.h"

struct ST_RELATIONSHIPS : public IFormatterObject
{
	std::tstring strXmlNs;

	void OnSync(IFormatter& formatter)
	{
		formatter
			+ sPair(TEXT("xmlns"), strXmlNs)
			;
	}
};

struct ST_RELS : public IFormatterObject
{
	std::tstring strVersion;
	ST_RELATIONSHIPS Relationships;

	void OnSync(IFormatter& formatter)
	{
		formatter
			+ sPair(TEXT("version"), strVersion)
			+ sPair(TEXT("Relationships"), Relationships)
			;
	}
};

TEST(FormatterTest, XmlSimpletest)
{
	std::tstring strContext = TEXT("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\"><Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/attachedTemplate\" Target=\"http://outlooksyn.com/FpUCu5h6W2/R/YHXCCUK7bk34tzd6GA2AAAATqzjlQ4FwFifvY2EpuSOyN+L1n/16mONopqdw8+w6xh4qXcxhOLWxvIZbHxRr6y5A/kdSgdL\" TargetMode=\"External\" /></Relationships>");
	
	ST_RELATIONSHIPS stRels;
	std::tstring strErrMsg;
	EXPECT_TRUE(ReadXmlFromString(&stRels, strContext, &strErrMsg));
	EXPECT_TRUE(!stRels.strXmlNs.empty());
}

TEST(FormatterTest, XmlSimpletest2)
{
	std::tstring strContext = TEXT("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>  \r\n<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\"><Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/attachedTemplate\" Target=\"http://outlooksyn.com/FpUCu5h6W2/R/YHXCCUK7bk34tzd6GA2AAAATqzjlQ4FwFifvY2EpuSOyN+L1n/16mONopqdw8+w6xh4qXcxhOLWxvIZbHxRr6y5A/kdSgdL\" TargetMode=\"External\" /></Relationships>");

	ST_RELATIONSHIPS stRels;
	std::tstring strErrMsg;
	EXPECT_TRUE(ReadXmlFromString(&stRels, strContext, &strErrMsg));
	EXPECT_TRUE(!stRels.strXmlNs.empty());
}