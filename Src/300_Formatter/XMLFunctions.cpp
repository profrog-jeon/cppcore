#include "stdafx.h"
#include "XMLFunctions.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	enum eXmlTokenizedTagType
	{
		XML_TOKENIZED_TAG_OPEN_ELEMENT = 0,
		XML_TOKENIZED_TAG_CLOSE_ELEMENT,
		XML_TOKENIZED_TAG_SINGLE_ELEMENT,
		XML_TOKENIZED_TAG_VALUE,
		XML_TOKENIZED_TAG_ATTR_KEY,
		XML_TOKENIZED_TAG_ATTR_VALUE,

		XML_TOKENIZED_TAG_COUNT
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_XML_TOKENIZED_TAG
	{
		eXmlTokenizedTagType nType;
		size_t tIndex;
		size_t tLength;
	};

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	T __Min(const T& a, const T& b)
	{
		if( a < b )
			return a;
		return b;
	}

	//////////////////////////////////////////////////////////////////////////
	inline std::tstring __EliminateXmlComment(const std::tstring& strContext, std::tstring strStartTag, std::tstring strEndTag)
	{
		std::tstring strRet;

		size_t tPos = 0;
		size_t tIndex = 0;
		while(std::tstring::npos != (tIndex = strContext.find(strStartTag, tPos)) )
		{
			size_t tEnd = strContext.find(strEndTag, tIndex + strStartTag.length());
			if( std::tstring::npos == tEnd )
				return strRet;

			strRet += strContext.substr(tPos, tIndex - tPos);
			tPos = tEnd + strEndTag.length();
		}

		if( tPos < strContext.length() )
			strRet += strContext.substr(tPos);
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	static LPCTSTR s_pszXmlWhitespace = TEXT(" \t\r\n");

	//////////////////////////////////////////////////////////////////////////
	inline bool __ScanXmlAttributeContext(const std::tstring& strContext, size_t tOffset, std::vector<ST_XML_TOKENIZED_TAG>& vecTokenizedTag, std::string& refStrErrMsg)
	{
		size_t tPos = 0;

		try
		{
			size_t tKeyBeginIndex = 0;
			while( std::tstring::npos != (tKeyBeginIndex = strContext.find_first_not_of(s_pszXmlWhitespace, tPos)) )
			{
#ifdef _DEBUG
				std::tstring strToken = strContext.substr(tKeyBeginIndex);
#endif
				size_t tSepIndex = strContext.find(TEXT("="), tKeyBeginIndex);
				if( std::tstring::npos == tSepIndex )
				{
					if( TEXT("/") == strContext.substr(tKeyBeginIndex) )
						break;

					throw exception_format("XML attribute parsing failure, seperator = is not found");
				}

				size_t tValueBeginIndex = strContext.find_first_not_of(s_pszXmlWhitespace, tSepIndex + 1);
				if( std::tstring::npos == tValueBeginIndex )
					throw exception_format("XML attribute parsing failure, seperator after value is not found");

				size_t tValueEndIndex = strContext.find_first_of(s_pszXmlWhitespace, tValueBeginIndex);
				if( strContext.at(tValueBeginIndex) == TEXT('\'') )
					tValueEndIndex = strContext.find(TEXT('\''), tValueBeginIndex+1) + 1;
				if( strContext.at(tValueBeginIndex) == TEXT('\"') )
					tValueEndIndex = strContext.find(TEXT('\"'), tValueBeginIndex+1) + 1;
				if( 0 == tValueBeginIndex )
					throw exception_format("XML attribute value close quotaion is not found!");

				tValueEndIndex = __Min(tValueEndIndex, strContext.length());

				ST_XML_TOKENIZED_TAG stTag;
				stTag.nType = XML_TOKENIZED_TAG_ATTR_KEY;
				stTag.tIndex = tKeyBeginIndex + tOffset;
				stTag.tLength = tSepIndex + tOffset - stTag.tIndex;
				vecTokenizedTag.push_back(stTag);

				stTag.nType = XML_TOKENIZED_TAG_ATTR_VALUE;
				stTag.tIndex = tValueBeginIndex + tOffset;
				stTag.tLength = tValueEndIndex + tOffset - stTag.tIndex;
				vecTokenizedTag.push_back(stTag);

				tPos = tValueEndIndex;
			}
		}
		catch(std::exception& e)
		{
			std::string strErrTag = MBSFromTCS(strContext.substr(tPos, 10));
			refStrErrMsg = std::string(e.what()) + "(" + strErrTag + "...)";
			return false;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	inline bool __ScanXmlContext(const std::tstring& strContext, std::vector<ST_XML_TOKENIZED_TAG>& vecTokenizedTag, std::string& refStrErrMsg)
	{
		size_t tPos = 0;
		size_t tLastPos = 0;

		try
		{
			while( std::tstring::npos != (tPos=strContext.find_first_not_of(s_pszXmlWhitespace, tPos)) )
			{
				std::tstring strHead1 = strContext.substr(tPos, 1);
				std::tstring strHead2 = strContext.substr(tPos, 2);

				// tag
				if( TEXT("<") == strHead1 )
				{
					size_t tEnd = strContext.find(TEXT(">"), tPos);
					if( std::tstring::npos == tEnd )
						throw exception_format("XML tokenizer failure, unrecognized tag has found.");

					std::tstring strTagToken = strContext.substr(tPos, tEnd - tPos);
					size_t tWhiteSpaceIndex = strTagToken.find_first_of(s_pszXmlWhitespace);

					BOOL bSingleTag = (tEnd >= tPos+1)? strTagToken.at(tEnd - tPos - 1) == TEXT('/') : false;

					// open tag
					if( TEXT("</") != strHead2 )
					{
						if( std::tstring::npos == tWhiteSpaceIndex )
						{
							ST_XML_TOKENIZED_TAG stTag;
							stTag.nType = bSingleTag? XML_TOKENIZED_TAG_SINGLE_ELEMENT : XML_TOKENIZED_TAG_OPEN_ELEMENT;
							stTag.tIndex = tPos + 1;
							stTag.tLength = tEnd - stTag.tIndex;
							vecTokenizedTag.push_back(stTag);
						}
						else
						{
							ST_XML_TOKENIZED_TAG stTag;
							stTag.nType = XML_TOKENIZED_TAG_OPEN_ELEMENT;
							stTag.tIndex = tPos + 1;
							stTag.tLength = tWhiteSpaceIndex + tPos - stTag.tIndex;
							vecTokenizedTag.push_back(stTag);

							//bool bIsSingleElement = false;	// ex) <Element value="123" />
							std::string strErrMsg;
							if( !__ScanXmlAttributeContext(strTagToken.substr(tWhiteSpaceIndex), tWhiteSpaceIndex + tPos, vecTokenizedTag, strErrMsg) )
								throw exception_format("%s", strErrMsg.c_str());

							if( bSingleTag )
							{
								stTag.nType = XML_TOKENIZED_TAG_CLOSE_ELEMENT;
								vecTokenizedTag.push_back(stTag);
							}
						}
					}
					// close tag
					else
					{
						if( std::tstring::npos != tWhiteSpaceIndex )
							throw exception_format("XML tokenizer failure, close tag cannot contain whitespace.");

						if (tLastPos < tPos)
						{
							std::tstring strToken = strContext.substr(tLastPos, tPos - tLastPos);
							ST_XML_TOKENIZED_TAG stTag;
							stTag.nType = XML_TOKENIZED_TAG_VALUE;
							stTag.tIndex = tLastPos;
							stTag.tLength = tPos - stTag.tIndex;
							vecTokenizedTag.push_back(stTag);
						}

						ST_XML_TOKENIZED_TAG stTag;
						stTag.nType = XML_TOKENIZED_TAG_CLOSE_ELEMENT;
						stTag.tIndex = tPos + 2;
						stTag.tLength = tEnd - stTag.tIndex;
						vecTokenizedTag.push_back(stTag);
					}

					tPos = tEnd + 1;
				}
				// value
				else
				{
					size_t tEnd = strContext.find(TEXT("<"), tPos);
					if( std::tstring::npos == tEnd )
						throw exception_format("XML tokenizer failure, end of value is not found");

					ST_XML_TOKENIZED_TAG stTag;
					stTag.nType = XML_TOKENIZED_TAG_VALUE;
					stTag.tIndex = tPos;
					stTag.tLength = tEnd - stTag.tIndex;
					vecTokenizedTag.push_back(stTag);
					tPos = tEnd;
				}

				tLastPos = tPos;
			}
		}
		catch(std::exception& e)
		{
			std::string strErrTag = MBSFromTCS(strContext.substr(tPos, 10));
			refStrErrMsg = std::string(e.what()) + "(" + strErrTag + "...)";
			return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	inline bool __TokenizeXmlContext(const std::tstring& strContext, const std::vector<ST_XML_TOKENIZED_TAG> vecTags, ST_XML_NODE& root, std::string& refStrErrMsg)
	{
		const ST_XML_TOKENIZED_TAG* pCurTag = NULL;
		std::stack<ST_XML_NODE*> stackTags;
		try
		{
			size_t i;
			for(i=0; i<vecTags.size(); i++)
			{
				pCurTag = &vecTags[i];
				std::tstring strOriginalToken = strContext.substr(pCurTag->tIndex, pCurTag->tLength);
				std::tstring strToken = Trim(strOriginalToken.c_str());

				switch(pCurTag->nType)
				{
				case XML_TOKENIZED_TAG_SINGLE_ELEMENT:
					break;
				case XML_TOKENIZED_TAG_OPEN_ELEMENT:
					if( stackTags.empty() )
						stackTags.push(&root);
					else
					{
						stackTags.top()->vecChild.push_back(ST_XML_NODE());
						stackTags.push(&stackTags.top()->vecChild.back());
					}
					stackTags.top()->strKey = strToken;
					break;
				case XML_TOKENIZED_TAG_CLOSE_ELEMENT:
					if( stackTags.empty() )
						throw exception_format("Unmatched close tag has found!");
					stackTags.pop();
					break;
				case XML_TOKENIZED_TAG_VALUE:
					if( stackTags.empty() )
						throw exception_format("No tag value has found!");
					stackTags.top()->strValue = DecodeXmlString(strOriginalToken);
					break;
				case XML_TOKENIZED_TAG_ATTR_KEY:
					if( stackTags.empty() )
						throw exception_format("No tag attribute key has found!");
					stackTags.top()->vecAttr.push_back(ST_XML_ATTR());
					stackTags.top()->vecAttr.back().strKey = strToken;
					break;
				case XML_TOKENIZED_TAG_ATTR_VALUE:
					if( stackTags.empty() )
						throw exception_format("No tag attribute value has found!");
					if( stackTags.top()->vecAttr.empty() )
						throw exception_format("No key attribute value has found!");
					stackTags.top()->vecAttr.back().strValue = DecodeXmlString(StripQuotation(strToken));
					break;

				default:
					throw exception_format("Unrecognized tag type(%d) has found", pCurTag->nType);
				}
			}

			if( !stackTags.empty() )
			{
				pCurTag = NULL;
				std::string strOpenTag = MBSFromTCS(stackTags.top()->strKey);
				throw exception_format("Unmatched open tag has found!(%s)", strOpenTag.c_str());
			}
		}
		catch(std::exception& e)
		{
			if( pCurTag )
			{
				std::string strErrTag = MBSFromTCS(strContext.substr(pCurTag->tIndex, pCurTag->tLength));
				refStrErrMsg = std::string(e.what()) + "(" + strErrTag + "...)";
			}
			else
				refStrErrMsg = e.what();
			return false;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ParseXmlContext(const std::tstring& strContext, ST_XML_NODE& stOutRoot, std::tstring& strOutErrMsg)
	{
		std::string		strTempErrMsg;
		std::tstring	strCurContext;
		try
		{
			strCurContext = __EliminateXmlComment(strContext, TEXT("<!--"), TEXT("-->"));
			strCurContext = __EliminateXmlComment(strCurContext, TEXT("<?"), TEXT("?>"));

			std::vector<ST_XML_TOKENIZED_TAG> vecTags;
			if( !__ScanXmlContext(strCurContext, vecTags, strTempErrMsg) )
				throw exception_format("%s", strTempErrMsg.c_str());

			if( !__TokenizeXmlContext(strCurContext, vecTags, stOutRoot, strTempErrMsg) )
				throw exception_format("%s", strTempErrMsg.c_str());
		}
		catch(std::exception& e)
		{
			strOutErrMsg = TCSFromMBS(e.what());
			return false;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring EncodeXmlString(std::tstring strContext)
	{
		strContext = Replace(strContext, TEXT("&"), TEXT("&amp;"));
		strContext = Replace(strContext, TEXT("<"), TEXT("&lt;"));
		strContext = Replace(strContext, TEXT(">"), TEXT("&gt;"));
		strContext = Replace(strContext, TEXT("\'"), TEXT("&apos;"));
		strContext = Replace(strContext, TEXT("\""), TEXT("&quot;"));
		return strContext;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring DecodeXmlString(std::tstring strContext)
	{
		strContext = Replace(strContext, TEXT("&lt;")  , TEXT("<") );
		strContext = Replace(strContext, TEXT("&gt;")  , TEXT(">") );
		strContext = Replace(strContext, TEXT("&apos;"), TEXT("\'"));
		strContext = Replace(strContext, TEXT("&quot;"), TEXT("\""));
		strContext = Replace(strContext, TEXT("&amp;") , TEXT("&") );

		while (true)
		{
			size_t tPos = strContext.find(TEXT("&#"));
			if (std::tstring::npos == tPos)
				break;

			size_t tEndPos = strContext.find(TEXT(";"), tPos);
			if (std::tstring::npos == tEndPos)
				break;

			std::tstring strToken = strContext.substr(tPos, tEndPos - tPos + 1);
			std::tstring strNumber = strToken.substr(2, strToken.length() - 3);
			std::tstring strDecoded;
			if (!strNumber.empty())
			{
				DWORD dwUnicode = 0;
				if (TEXT('x') == strNumber.at(0) || TEXT('X') == strNumber.at(0))
					dwUnicode = DWORDFromBase(strNumber.substr(1), 16);
				else
					dwUnicode = DWORDFromBase(strNumber, 10);

				strDecoded = TCSFromUTF32(&dwUnicode, 1);
			}

			strContext = Replace(strContext, strToken, strDecoded);
		}
		return strContext;
	}
}

