#include "stdafx.h"
#include "JSONFunctions.h"

//#define USE_JSON_PARSER_LOG

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	const TCHAR* GetTokenTypeString(E_TOKEN_TYPE nType)
	{
		switch (nType)
		{
			CASE_TO_STR(TT_UNKNOWN);
			CASE_TO_STR(TT_BRACE_OPEN);
			CASE_TO_STR(TT_BRACE_CLOSE);
			CASE_TO_STR(TT_SQURE_BRACKET_OPEN);
			CASE_TO_STR(TT_SQURE_BRACKET_CLOSE);
			CASE_TO_STR(TT_COMMA);
			CASE_TO_STR(TT_COLON);
			CASE_TO_STR(TT_KEY);
			CASE_TO_STR(TT_VALUE);
		}
		return TEXT("UnknownTokenType");
	}

	//////////////////////////////////////////////////////////////////////////
	struct sSpecialCharPosInfo { int nIndex; size_t tPos; size_t tEndPos; size_t tLen; };
	const TCHAR* g_pszOrgSpecialCharArr[] = { TEXT("\""),   TEXT("\\"),   TEXT("/"),   TEXT("\b"),  TEXT("\f"),  TEXT("\n"),  TEXT("\r"),  TEXT("\t") };
	const TCHAR* g_pszJsonSpecialCharArr[] = { TEXT("\\\""), TEXT("\\\\"), TEXT("\\/"), TEXT("\\b"), TEXT("\\f"), TEXT("\\n"), TEXT("\\r"), TEXT("\\t") };
	const size_t g_tSpecialCharCount = sizeof(g_pszOrgSpecialCharArr) / sizeof(g_pszOrgSpecialCharArr[0]);

	//////////////////////////////////////////////////////////////////////////
	// C -> JSON string
	std::tstring ConvertToJsonString(const std::tstring& strValue)
	{
		std::map<size_t, sSpecialCharPosInfo> mapSpecialCharPos;

		// Finding tokens to be changed JSON specialized charactor.
		size_t i;
		for (i = 0; i < g_tSpecialCharCount; i++)
		{
			size_t tPos = 0;
			size_t tIndex = strValue.find(g_pszOrgSpecialCharArr[i], 0);

			while (std::tstring::npos != tIndex)
			{
				sSpecialCharPosInfo info;
				info.nIndex = (int)i;
				info.tPos = tIndex;
				info.tLen = 1;
				info.tEndPos = info.tPos + info.tLen;
				mapSpecialCharPos.insert(std::make_pair(tIndex, info));

				tPos = tIndex + 1;
				tIndex = strValue.find(g_pszOrgSpecialCharArr[i], tPos);
			}
		}

		// Marking a JSON string START marker.
		std::tstring strResult = TEXT("\"");
		if (mapSpecialCharPos.empty())
		{
			strResult += strValue;
		}
		else
		{
			// Replace tokens with JSON special charaters
			// text + CHAR + text + CHAR + text ... + CHAR + text
			std::map<size_t, sSpecialCharPosInfo>::iterator iter = mapSpecialCharPos.begin();
			sSpecialCharPosInfo prePosInfo;

			// Adding a head text and replaced token with JSON special character.
			{
				prePosInfo = iter->second;
				strResult += strValue.substr(0, prePosInfo.tPos) + g_pszJsonSpecialCharArr[prePosInfo.nIndex];
				iter++;
			}

			// Adding a next text and replaced token with JSON special character.
			for (; iter != mapSpecialCharPos.end(); iter++)
			{
				sSpecialCharPosInfo posInfo = iter->second;

				std::tstring strSubString = strValue.substr(prePosInfo.tEndPos, posInfo.tPos - prePosInfo.tEndPos);
				strResult += strSubString + g_pszJsonSpecialCharArr[posInfo.nIndex];

				prePosInfo = posInfo;
			}

			// Adding a last text to tail.
			strResult += strValue.substr(prePosInfo.tEndPos);
		}

		// Marking a JSON string FINISH marker.
		strResult += std::tstring(TEXT("\""));
		return strResult;
	}

	//////////////////////////////////////////////////////////////////////////
	// JSON -> C string
	std::tstring RestoreFromJsonString(const std::tstring& strValue)
	{
		// Checking outter quotation marker.
		if (strValue.length() < 2 ||
			TEXT('\"') != strValue[0] ||
			TEXT('\"') != strValue[strValue.length() - 1])
		{
			// check null string and convert it to empty string
			if (strValue == TEXT("null"))
				return TEXT("");
			else
				return strValue;
		}

		const int nJsonSpecialChar = sizeof(g_pszJsonSpecialCharArr) / sizeof(const char*);
		std::map<size_t, int> mapSpecialCharPos;
		std::tstring strResult;

		// Finding JSON special charactor position.
		int i;
		for (i = 0; i < (int)strValue.length() - 1; i++)
		{
			if (strValue[i] != TEXT('\\'))
				continue;

			int j;
			for (j = 0; j < nJsonSpecialChar; j++)
			{
				if (strValue[i + 1] != g_pszJsonSpecialCharArr[j][1])
					continue;

				mapSpecialCharPos.insert(std::make_pair(i++, j));
				break;
			}
		}

		if (mapSpecialCharPos.empty())
		{
			strResult = strValue;
		}
		else
		{
			// Replace tokens with JSON special charaters
			// text + CHAR + text + CHAR + text ... + CHAR + text
			std::map<size_t, int>::iterator iter = mapSpecialCharPos.begin();
			size_t tPrePos = iter->first;

			// Adding a HEAD text and replaced JSON special character with c++ token.
			strResult = strValue.substr(0, tPrePos) + g_pszOrgSpecialCharArr[iter->second];
			iter++;

			// Adding a NEXT text and replaced JSON special character with c++ token.
			for (; iter != mapSpecialCharPos.end(); iter++)
			{
				size_t tCurPos = iter->first;
				int nCharIndex = iter->second;
				std::tstring strSubString = strValue.substr(tPrePos + 2, tCurPos - tPrePos - 2);
				strResult += strSubString + g_pszOrgSpecialCharArr[nCharIndex];
				tPrePos = tCurPos;
			}

			// Adding a LAST text.
			strResult += strValue.substr(tPrePos + 2);
		}

		// Eleminating outter quotation mark
		if (strResult.length() > 1 &&
			TEXT('\"') == strResult[0] &&
			TEXT('\"') == strResult[strResult.length() - 1])
			strResult = strResult.substr(1, strResult.length() - 2);
		return strResult;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t Scan(const TCHAR* pszContext, CTStringVec& vecToken)
	{
		std::tstring strContext(pszContext);
		size_t tRet = Scan(strContext, vecToken);
		return tRet;
	}

	//////////////////////////////////////////////////////////////////////////
	inline int FindFirstOfQuotation(std::tstring& strContext, int nOffset)
	{
		if (nOffset < 0)
			return -1;

		if ((int)strContext.length() <= nOffset)
			return -1;

		int i;
		for (i = nOffset; i < (int)strContext.length(); i++)
		{
			// Jump a JSON special charactor
			if (strContext[i] == TEXT('\\'))
			{
				i++;
				continue;
			}

			if (strContext[i] == TEXT('\"'))
				return i;
		}
		return -1;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t Scan(std::tstring& strContext, CTStringVec& vecToken)
	{
		try
		{
			std::tstring strSeperator = TEXT(":,[]{}");

			// Find seperators and building index vector
			std::vector<int> vecSeperatorPos;
			size_t i;
			for (i = 0; i < strContext.size(); i++)
			{
				if (strContext.at(i) == TEXT('\"'))
				{
					if (i == 0 || strContext.at(i - 1) != TEXT('\\'))
						vecSeperatorPos.push_back((int)i);
					continue;
				}

				int nDelemeterIndex = (int)strSeperator.find_first_of(strContext.at(i));
				if (nDelemeterIndex < 0)
					continue;
				vecSeperatorPos.push_back((int)i);
				vecSeperatorPos.push_back((int)i + 1);
			}

			// Reserve a quotation ATOMIC
			int nPos = 0;
			size_t tLastSepIndex = 0;
			while (true)
			{
				int nStartIndex = FindFirstOfQuotation(strContext, nPos);
				if (nStartIndex < 0)
					break;
				int nEndIndex = FindFirstOfQuotation(strContext, nStartIndex + 1);
				if (nEndIndex < 0)
					break;

				for (tLastSepIndex; tLastSepIndex < vecSeperatorPos.size(); tLastSepIndex++)
				{
					if (vecSeperatorPos[tLastSepIndex] <= nStartIndex)
						continue;
					if (vecSeperatorPos[tLastSepIndex] > nEndIndex)
						break;
					vecSeperatorPos[tLastSepIndex] = -1;
				}
				nPos = nEndIndex + 1;
			}


			{	// Remove duplicated value
				int nPrePos = -1;
				for (i = 0; i < vecSeperatorPos.size(); i++)
				{
					if (vecSeperatorPos[i] < 0)
						continue;

					if (vecSeperatorPos[i] == nPrePos)
						vecSeperatorPos[i] = -1;
					nPrePos = vecSeperatorPos[i];
				}
			}

			// If there is no seperater included in context, bypasses the one but do `trim`.
			if (vecSeperatorPos.empty())
			{
				std::tstring strTemp = strContext = Trim(strContext);
				if (!strTemp.empty())
					vecToken.push_back(strTemp);
				return vecToken.size();
			}

			// Tokenize context string on based seperator position.
			int nPrePos = -1;
			for (i = 0; i < vecSeperatorPos.size(); i++)
			{
				int nCurPos = vecSeperatorPos[i];
				if (nCurPos < 0)
					continue;

				if (nPrePos < 0)
				{
					nPrePos = nCurPos;
					std::tstring strToken = Trim(strContext.substr(0, nCurPos).c_str());
					if (!strToken.empty())
						vecToken.push_back(strToken);

					continue;
				}

				std::tstring strToken = Trim(strContext.substr(nPrePos, nCurPos - nPrePos).c_str());
				if (!strToken.empty())
					vecToken.push_back(strToken);
				nPrePos = nCurPos;
			}
			{
				std::tstring strToken = Trim(strContext.substr(nPrePos).c_str());
				if (!strToken.empty())
					vecToken.push_back(strToken);
			}

		}
		catch (...)
		{
			// Unexpected error has been occurred!!
			assert(false);
		}

		return vecToken.size();
	}

	//////////////////////////////////////////////////////////////////////////
	bool Parse(CTStringVec& vecToken, CTokenVec& vecJsonToken, std::tstring& strErrMsgDump)
	{
		std::vector<char> parsingStack;

		size_t tLastParsingTokenIndex = 0;
		try
		{
			if (vecToken.empty())
				throw exception_format(TEXT("There is no contents."));

			size_t tStartIndex = 0;
			if ((vecToken[0] == TEXT("{")) || (vecToken[0] == TEXT("[")))
			{
				// good
				// do nothing...
			}
			else if (vecToken.size() >= 3 && vecToken[1] == TEXT(":") && (vecToken[2] == TEXT("{") || (vecToken[2] == TEXT("["))))
			{
				// In this case JSON start with a key ex) Feedback : {...}
				tStartIndex = 2;
			}
			else
			{
				throw exception_format(TEXT("Start token always should be `{`, `[` but `%s`."), vecToken[0].c_str());
			}

			size_t i;
			for (i = tStartIndex; i < vecToken.size(); i++)
			{
				tLastParsingTokenIndex = i;
				std::tstring strTemp = vecToken[i];

				ST_JSON_CHUNK tempToken(TT_UNKNOWN, strTemp);
				{	// Building parsing stack string for debug
					int i;
					for (i = 0; i < (int)parsingStack.size(); i++)
						tempToken.strParsingStack += parsingStack[i];
				}

#ifdef USE_JSON_PARSER_LOG
				{
					int i;
					for (i = 0; i < (int)parsingStack.size(); i++)
						std_cout << parsingStack[i];
					std_cout << "\t" << strTemp << std::endl;
				}
#endif

				if (vecToken[i] == TEXT("{"))
				{
					if (!parsingStack.empty() && parsingStack.back() == TEXT(','))
						parsingStack.pop_back();

					tempToken.nType = TT_BRACE_OPEN;
					parsingStack.push_back('}');
				}
				else if (vecToken[i] == TEXT("["))
				{
					if (!parsingStack.empty() && parsingStack.back() == TEXT(','))
						parsingStack.pop_back();

					tempToken.nType = TT_SQURE_BRACKET_OPEN;
					parsingStack.push_back(TEXT(']'));
				}
				else if (vecToken[i] == TEXT("}"))
				{
					tempToken.nType = TT_BRACE_CLOSE;
					if (parsingStack.empty())
						throw exception_format(TEXT("Unnesessary `}` found!!"));
					if (parsingStack.back() != TEXT('}'))
						throw exception_format(TEXT("A token `%c` is needed in here."), parsingStack.back());
					parsingStack.pop_back();

					if (!parsingStack.empty() && parsingStack.back() == TEXT(':'))
						parsingStack.pop_back();
				}
				else if (vecToken[i] == TEXT("]"))
				{
					tempToken.nType = TT_SQURE_BRACKET_CLOSE;
					if (parsingStack.empty())
						throw exception_format(TEXT("Unnesessary `]` found!!"));
					if (parsingStack.back() != TEXT(']'))
						throw exception_format(TEXT("Unmatched `]` found!!"));
					parsingStack.pop_back();

					if (!parsingStack.empty() && parsingStack.back() == TEXT(':'))
						parsingStack.pop_back();
				}
				else if (vecToken[i] == TEXT(":"))
				{
					tempToken.nType = TT_COLON;
					parsingStack.push_back(':');
				}
				else if (vecToken[i] == TEXT(","))
				{
					tempToken.nType = TT_COMMA;
					parsingStack.push_back(',');
				}
				else
				{
					if (!vecJsonToken.empty())
					{
						if (vecJsonToken.back().nType == TT_VALUE)
							throw exception_format(TEXT("VALUE seperator `,` is required!!"));
						if (vecJsonToken.back().nType == TT_KEY)
							throw exception_format(TEXT("KEY-VALUE seperator `:` is required!!"));
					}

					if (!parsingStack.empty() && parsingStack.back() == TEXT(','))
						parsingStack.pop_back();

					std::tstring strNextToken;
					if (i + 1 < vecToken.size())
						strNextToken = vecToken[i + 1];

					if (strNextToken == TEXT(":"))
					{
						tempToken.nType = TT_KEY;
					}
					else
					{
						tempToken.nType = TT_VALUE;
						if (!parsingStack.empty() && parsingStack.back() == TEXT(':'))
							parsingStack.pop_back();
					}
				}

				vecJsonToken.push_back(tempToken);
			}

			if (!parsingStack.empty())
			{
				std::tstring strErrMsg;

				int i;
				for (i = (int)parsingStack.size() - 1; i >= 0; i--)
				{
					std::tstring strTemp = parsingStack[i] == TEXT(':') ? TEXT("VALUE") : Format(TEXT("%c"), parsingStack[i]);
					strErrMsg += Format(TEXT("`%s` is not found!!\r\n"), strTemp.c_str());
					break;
				}
				throw exception_format(TEXT("%s"), strErrMsg.c_str());
			}
		}
		catch (std::exception& e)
		{
			size_t i;
			for (i = 0; i < tLastParsingTokenIndex + 1 && i < vecToken.size(); i++)
				strErrMsgDump += vecToken[i] + TEXT("\r\n");

			strErrMsgDump += TCSFromMBS(e.what());
			return false;
		}
		catch (...)
		{
			// Unexpected error has been occurred!!
			assert(false);

			size_t i;
			for (i = 0; i < tLastParsingTokenIndex + 1 && i < vecToken.size(); i++)
				strErrMsgDump += vecToken[i] + TEXT("\r\n");

			strErrMsgDump += std::tstring(TEXT("Unexpected error has found!!\r\n"));
			return false;
		}
		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////
	size_t Build(CTokenVec& vecJsonToken, CChunkVec& vecJsonChunk)
	{
		try
		{
			ST_JSON_TOKEN tempJsonChunk;
			std::tstring strPreParsingStack;

			size_t i;
			for (i = 0; i < vecJsonToken.size(); i++)
			{
				ST_JSON_CHUNK& token = vecJsonToken[i];

				if (token.strParsingStack == TEXT("}") && TT_KEY == token.nType)
					tempJsonChunk.strKey = RestoreFromJsonString(token.strValue);
				else if (token.strParsingStack == TEXT("},") && TT_KEY == token.nType)
					tempJsonChunk.strKey = RestoreFromJsonString(token.strValue);
				else if (token.strParsingStack.size() >= 2 && token.strParsingStack[0] == TEXT('}') && token.strParsingStack[1] == TEXT(':'))
					tempJsonChunk.vecToken.push_back(token.strValue);
				else if (token.strParsingStack == TEXT("}") && strPreParsingStack.size() >= 2 && strPreParsingStack[0] == TEXT('}') && strPreParsingStack[1] == TEXT(':'))
				{
					vecJsonChunk.push_back(tempJsonChunk);
					tempJsonChunk.Clear();
				}
				else if (token.strParsingStack.empty() && TT_VALUE == token.nType)
				{
					tempJsonChunk.vecToken.push_back(token.strValue);
					vecJsonChunk.push_back(tempJsonChunk);
					tempJsonChunk.Clear();
				}
				else if (!token.strParsingStack.empty() && token.strParsingStack[0] == TEXT(']'))
				{
					if (((token.strParsingStack.length() == 1) && (TT_VALUE == token.nType)) ||
						((token.strParsingStack.length() == 2) && (token.strParsingStack.at(1) == TEXT(',')) && (TT_VALUE == token.nType)))
					{
						tempJsonChunk.vecToken.push_back(token.strValue);
						vecJsonChunk.push_back(tempJsonChunk);
						tempJsonChunk.Clear();
					}
					else
					{
						if (TT_BRACE_OPEN == token.nType)
						{
							if (token.strParsingStack.length() == 1
								|| token.strParsingStack == TEXT("],"))
								tempJsonChunk.vecToken.push_back(token.strValue);
						}
						if (token.strParsingStack.length() > 1 && token.strParsingStack.at(1) == TEXT('}'))
						{
							tempJsonChunk.vecToken.push_back(token.strValue);
						}
						if (token.strParsingStack.length() == 2 && TT_BRACE_CLOSE == token.nType)
						{
							vecJsonChunk.push_back(tempJsonChunk);
							tempJsonChunk.Clear();
						}
					}
				}
				strPreParsingStack = token.strParsingStack;
			}
		}
		catch (...)
		{
			// Unexpected error has been occurred!!
			assert(false);
		}

		return vecJsonChunk.size();
	}

	//////////////////////////////////////////////////////////////////////////
	size_t ExpandArray(size_t tIndex, CChunkVec& vecJsonChunk)
	{
		std::tstring strErrMsg;

		CTokenVec vecTempJsonToken;
		if (!Parse(vecJsonChunk[tIndex].vecToken, vecTempJsonToken, strErrMsg))
		{
			Log_Error(TEXT("Expanding failure, %s"), strErrMsg.c_str());
			return 0;
		}

		std::tstring strKey = vecJsonChunk[tIndex].strKey;

		CChunkVec vecTempChunkVec;
		size_t tRet = fmt_internal::Build(vecTempJsonToken, vecTempChunkVec);
		if (0 == tRet)
			return 0;

		size_t tPushCount = tRet - 1;

		// insert new parsed chunks into current index, and remove expanded item
		size_t tLastIndex = 0;
		{
			size_t tNewCount = vecJsonChunk.size() + tRet - 1;
			vecJsonChunk.resize(tNewCount);
			tLastIndex = tNewCount - 1;
		}

		// by reverse order for preserving data
		size_t i;
		for (i = 0; i < tPushCount; i++)
		{
			vecJsonChunk[tLastIndex - i] = vecJsonChunk[tLastIndex - i - tPushCount];
		}

		// assign new chunks
		for (i = 0; i < tRet; i++)
		{
			vecJsonChunk[tIndex + i] = vecTempChunkVec[i];
			vecJsonChunk[tIndex + i].strKey = strKey;
		}

		return tRet;
	}
}
