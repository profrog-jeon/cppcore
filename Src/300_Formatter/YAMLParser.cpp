#include "stdafx.h"
#include "YAMLParser.h"

//////////////////////////////////////////////////////////////////////////
namespace fmt_internal
{
	ST_YAML_NODE::ST_YAML_NODE(void)
		: nIndentationLevel(0)
		, strKey()
		, strValue()
		, Children()
	{
	}

	ST_YAML_NODE::~ST_YAML_NODE(void)
	{
	}

	void ST_YAML_NODE::PrintOut(void)
	{
		for (int i = 0; i < nIndentationLevel; i++)
			tprintf(TEXT("  "));

		tprintf(TEXT("%s: "), strKey.c_str());
		if (!strValue.empty())
			tprintf(TEXT("%s"), strValue.c_str());
		tprintf(TEXT("\n"));

		for (ST_YAML_NODE& child : Children)
			child.PrintOut();
	}

	CYAMLParser::CYAMLParser(core::IChannel& Channel)
		: m_Channel(Channel)
		, m_tIndentationWidth(-1)
	{
	}

	CYAMLParser::~CYAMLParser(void)
	{
	}

	void RemoveYamlComment(std::tstring& refLine)
	{
		bool bInQuotation = false;

		size_t tPos = 0;
		while (tPos != std::tstring::npos)
		{
			const size_t tIndex = refLine.find_first_of(TEXT("#\""), tPos);
			if (std::tstring::npos == tIndex)
				break;

			tPos = tIndex + 1;
			switch (refLine[tIndex])
			{
			case TEXT('\"'):
				if (!bInQuotation)
					bInQuotation = true;
				else
				{
					if (tIndex != 0 && refLine[tIndex - 1] == '\\')
						continue;
					bInQuotation = false;
				}
				break;

			case TEXT('#'):
				if (bInQuotation)
					continue;

				// 주석 발견, 잘라내고 탈출
				refLine = refLine.substr(0, tIndex);
				return;
			}
		}
	}

	void CYAMLParser::CalcIndentation(std::tstring& refLine, int& outIndentationLevel)
	{
		// 들여쓰기 길이 계산
		size_t tIndentationLength = refLine.find_first_not_of(' ');
		if (tIndentationLength == -1)
			tIndentationLength = 0;

		if (-1 == m_tIndentationWidth)
		{
			if (0 == tIndentationLength)
				outIndentationLevel = 0;
			else
			{
				m_tIndentationWidth = tIndentationLength;
				outIndentationLevel = 1;
			}
		}
		else
			outIndentationLevel = (int)(tIndentationLength / m_tIndentationWidth);

		// 들여쓰기 제거
		refLine = refLine.substr(tIndentationLength);
	}

	bool CYAMLParser::ReadLine(std::tstring& outLine)
	{
		while (true)
		{
			TCHAR cChar;
			if (0 == m_Channel.Access(&cChar, sizeof(cChar)))
				return false;

			if ('\n' == cChar)
				break;

			outLine.push_back(cChar);
		}

		return true;
	}

	bool CYAMLParser::ParseNode(std::tstring strLine, ST_YAML_NODE& outNode)
	{
		RemoveYamlComment(strLine);
		CalcIndentation(strLine, outNode.nIndentationLevel);

		strLine = Trim(strLine);
		if (strLine.empty())
			return false;

		// array의 시작 항목
		if (strLine.substr(0, 2) == TEXT("- "))
		{
			outNode.strKey = TEXT("-");

			ST_YAML_NODE stChild;
			ParseNode(strLine.substr(2), stChild);
			stChild.nIndentationLevel = outNode.nIndentationLevel + 1;
			outNode.Children.push_back(stChild);
		}
		// key-value 항목
		else if (std::tstring::npos != strLine.find(TEXT(": ")))
		{
			outNode.strKey = Split(strLine, TEXT(": "), &outNode.strValue);
			if (outNode.strKey.empty())
				return false;
		}
		// map의 시작, "key:"
		else if (strLine.back() == TEXT(':'))
		{
			outNode.strKey = TrimRight(strLine, TEXT(":"));
		}
		// array의 value만 존재하는 항목
		else
			outNode.strValue = strLine;

		return true;
	}

	ECODE CYAMLParser::Parse(ST_YAML_NODE& outRoot, std::tstring& outErrMsg)
	{
		ECODE nRet = EC_SUCCESS;
		try
		{
			size_t tIndentationWidth = -1;

			std::stack<ST_YAML_NODE*> NodeStack;
			outRoot.nIndentationLevel = 0;
			NodeStack.push(&outRoot);
			while (!NodeStack.empty())
			{
				ST_YAML_NODE* pParent = NodeStack.top();

				ST_YAML_NODE CurNode;
				{
					std::tstring strLine;
					if (!ReadLine(strLine))
						break;

					if (!ParseNode(strLine, CurNode))
						continue;
				}

				if (NodeStack.size() == 1 && CurNode.nIndentationLevel == 0)
				{
					outRoot = CurNode;
					continue;
				}

				while (CurNode.nIndentationLevel <= pParent->nIndentationLevel)
				{
					NodeStack.pop();
					if (NodeStack.empty())
						throw exception_format(TEXT("NO PARENT node indentation:%d found, "), CurNode.nIndentationLevel);
					pParent = NodeStack.top();
				}

				pParent->Children.push_back(CurNode);
				NodeStack.push(&pParent->Children.back());
			}
		}
		catch (const std::exception& e)
		{
			Log_Error("%s", e.what());
			return nRet;
		}

		return EC_SUCCESS;
	}

	std::tstring EncodeYamlString(std::tstring strContext)
	{
		strContext = StripQuotation(strContext);

		Replace(strContext, TEXT(R"(")"), TEXT(R"(\")"));
		Replace(strContext, TEXT(R"(')"), TEXT(R"('')"));

		strContext = WrapQuotation(strContext);
		return strContext;
	}

	std::tstring DecodeYamlString(std::tstring strContext)
	{
		strContext = StripQuotation(strContext);

		Replace(strContext, TEXT(R"(\")"), TEXT(R"(")"));
		Replace(strContext, TEXT(R"('')"), TEXT(R"(')"));
		return strContext;
	}
}
