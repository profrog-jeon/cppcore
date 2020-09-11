#include "stdafx.h"
#include "INIParser.h"

#ifdef UNICODE
#define CINIParser		CINIParserW
#else
#define CINIParser		CINIParserA
#endif

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CINIParser::CINIParser(void)
		: m_listString()
		, m_strFilename()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CINIParser::~CINIParser(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CINIParser::Open(CTextFileReader* pReader)
	{
		std::tstring strLine;
		while(EC_SUCCESS == pReader->ReadLine(strLine))
		{
			strLine = TrimRight(strLine);
			m_listString.push_back(strLine);
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CINIParser::Open(std::tstring strContext)
	{
		if( strContext.empty() )
			return EC_SUCCESS;

		int nPos = 0;
		while(true)
		{
			int nNextPos = (int)strContext.find(TEXT("\n"), nPos);
			if( nNextPos < 0 )
				break;

			std::tstring strLine = strContext.substr(nPos, nNextPos - nPos);
			strLine = TrimRight(strLine);
			m_listString.push_back(strLine);
			nPos = nNextPos + 1;
		}

		std::tstring strLine = strContext.substr(nPos);
		if( !strLine.empty() )
		{
			strLine = TrimRight(strLine);
			m_listString.push_back(strLine);
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	void CINIParser::Close(void)
	{
		m_listString.clear();
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CINIParser::Flush(std::tstring& strContext)
	{
		CStringListIt iter;
		for(iter=m_listString.begin(); iter!=m_listString.end(); iter++)
		{
			std::tstring strLine = (*iter) + TEXT("\r\n");
			strContext += strLine;
		}
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CINIParser::Flush(CTextFileWriter* pWriter)
	{
		CStringListIt iter;
		for(iter=m_listString.begin(); iter!=m_listString.end(); iter++)
			pWriter->WriteLine(*iter);

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CINIParser::GetSectionNames(std::vector<std::tstring>& outNames)
	{
		CStringListIt iter;
		for(iter=m_listString.begin(); iter!=m_listString.end(); iter++)
		{
			std::tstring strCur = (*iter);

			if( strCur.empty() )
				continue;

			// Skip comment
			if( strCur.at(0) == TEXT('#')	||  strCur.at(0) == TEXT(';') )
				continue;

			// Section TEST
			if( strCur.at(0) != TEXT('[') )
				continue;

			std::tstring strSectionName = Trim(strCur, TEXT("[]"));
			outNames.push_back(strSectionName);
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring CINIParser::GetValue(std::tstring strSection, std::tstring strKey, std::tstring strDefault)
	{
		std::tstring strSectionTag	= Format(TEXT("[%s]"), strSection.c_str());
		bool bInTheSection = false;

		CStringListIt iter;
		for(iter=m_listString.begin(); iter!=m_listString.end(); iter++)
		{
			std::tstring strCur = (*iter);
			//trimRightT(strCur);

			if( strCur.empty() )
				continue;

			// Skip comment
			if( strCur.at(0) == TEXT('#')	||  strCur.at(0) == TEXT(';') )
				continue;

			// Section TEST
			if( strCur.at(0) == TEXT('[') )
			{
				bInTheSection = (*iter) == strSectionTag;
				continue;
			}

			// Value TEST
			if( bInTheSection )
			{
				size_t tSepIndex = strCur.find(TEXT("="));
				if( std::tstring::npos == tSepIndex )
					continue;

				std::tstring strCurKey = Trim(strCur.substr(0, tSepIndex).c_str());
				if( strCurKey != strKey )
					continue;

				std::tstring strValue = Trim(strCur.substr(tSepIndex+1).c_str());
				size_t tStrValueLen  = strValue.length();

				// strip quotation
				if( tStrValueLen > 1 && strValue.at(0) == TEXT('\"') && strValue.at(tStrValueLen - 1) == TEXT('\"') )
					strValue = Trim(strValue.substr(1, tStrValueLen-2).c_str());

				return strValue;
			}
		}

		return strDefault;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CINIParser::GetValues(std::tstring strSection, std::tstring strKey, std::vector<std::tstring>& refValues)
	{
		std::tstring strSectionTag	= Format(TEXT("[%s]"), strSection.c_str());
		bool bInTheSection = false;

		CStringListIt iter;
		for(iter=m_listString.begin(); iter!=m_listString.end(); iter++)
		{
			std::tstring strCur = (*iter);
			//trimRightT(strCur);

			if( strCur.empty() )
				continue;

			// Skip comment
			if( strCur.at(0) == TEXT('#') ||  strCur.at(0) == TEXT(';') )
				continue;

			// Section TEST
			if( strCur.at(0) == TEXT('[') )
			{
				bInTheSection = (*iter) == strSectionTag;
				continue;
			}

			// Value TEST
			if( bInTheSection )
			{
				size_t tSepIndex = strCur.find(TEXT("="));
				if( std::tstring::npos == tSepIndex )
					continue;

				std::tstring strCurKey = strCur.substr(0, tSepIndex);
				if( strCurKey != strKey )
					continue;

				std::tstring strCurValue = Trim(strCur.substr(tSepIndex+1).c_str());
				refValues.push_back(strCurValue);
			}
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CINIParser::GetSectionValues(std::tstring strSection, CStringMap& refMap)
	{
		std::tstring strSectionTag	= Format(TEXT("[%s]"), strSection.c_str());
		bool bInTheSection = false;

		CStringListIt iter;
		for(iter=m_listString.begin(); iter!=m_listString.end(); iter++)
		{
			std::tstring strCur = (*iter);
			//trimRightT(strCur);

			if( strCur.empty() )
				continue;

			// Skip comment
			if( strCur.at(0) == TEXT('#') ||  strCur.at(0) == TEXT(';') )
				continue;

			// Section TEST
			if( strCur.at(0) == TEXT('[') )
			{
				bInTheSection = (*iter) == strSectionTag;
				continue;
			}

			// Value TEST
			if( bInTheSection )
			{
				size_t tSepIndex = strCur.find(TEXT("="));
				if( std::tstring::npos == tSepIndex )
					continue;

				std::tstring strCurKey = strCur.substr(0, tSepIndex);
				std::tstring strCurValue = strCur.substr(tSepIndex+1);
				Replace(strCurValue, TEXT("\""), TEXT(""));
				strCurValue = Trim(strCurValue);

				refMap.insert(std::make_pair(strCurKey, strCurValue));
			}
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CINIParser::SetValue(std::tstring strSection, std::tstring strKey, std::tstring strValue)
	{
		std::tstring strSectionTag	= Format(TEXT("[%s]"), strSection.c_str());
		std::tstring strReplaceLine = strKey + TEXT("=") + strValue;

		bool bInTheSection = false;

		CStringListIt iter;
		for(iter=m_listString.begin(); iter!=m_listString.end(); iter++)
		{
			std::tstring strCur = TrimRight((*iter));

			if( strCur.empty() )
				continue;

			// Skip comment
			if( strCur.at(0) == TEXT('#') ||  strCur.at(0) == TEXT(';') )
				continue;

			// Section TEST
			if( strCur.at(0) == TEXT('[') )
			{
				bool bPreSectionFlag = bInTheSection;
				bInTheSection = (*iter) == strSectionTag;

				if( bPreSectionFlag && !bInTheSection )
				{
					m_listString.insert(iter, strReplaceLine);
					return EC_SUCCESS;
				}
				continue;
			}

			// Value TEST
			if( bInTheSection )
			{
				size_t tSepIndex = strCur.find(TEXT("="));
				if( std::tstring::npos == tSepIndex )
					continue;

				std::tstring strCurKey = strCur.substr(0, tSepIndex);
				if( strCurKey != strKey )
					continue;

				(*iter) = strReplaceLine;
				return EC_SUCCESS;
			}
		}

		if( !bInTheSection )
			m_listString.push_back(strSectionTag);
		m_listString.push_back(strReplaceLine);
		return EC_SUCCESS;
	}
}

