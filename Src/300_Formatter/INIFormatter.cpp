#include "stdafx.h"
#include "INIFormatter.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	CINIFormatter::CINIFormatter(void)
		: m_listString()
		, m_strFilename()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CINIFormatter::~CINIFormatter(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	DWORD CINIFormatter::Open(std::tstring strContext)
	{
		if( strContext.empty() )
			return EC_SUCCESS;

		int nPos = 0;
		while(true)
		{
			int nNextPos = (int)strContext.find(TEXT("\n"), nPos);
			if( nNextPos < 0 )
				break;

			std::tstring strLine = TrimRight(strContext.substr(nPos, nNextPos - nPos).c_str());
			m_listString.push_back(strLine);
			nPos = nNextPos + 1;
		}

		std::tstring strLine = TrimRight(strContext.substr(nPos).c_str());
		if( !strLine.empty() )
		{
			m_listString.push_back(strLine);
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	void CINIFormatter::Close(void)
	{
		m_listString.clear();
	}

	//////////////////////////////////////////////////////////////////////////
	DWORD CINIFormatter::Flush(core::IChannel& channel)
	{
		CStringListIt iter;
		for(iter=m_listString.begin(); iter!=m_listString.end(); iter++)
		{
			std::tstring strLine = (*iter) + TEXT("\r\n");
			if( strLine.at(0) == TEXT('[') && (iter!=m_listString.begin()) )
				strLine = TEXT("\r\n") + strLine;
			channel.Access((void*)strLine.c_str(), strLine.size() * sizeof(TCHAR));
		}
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	DWORD CINIFormatter::GetSectionNames(std::vector<std::tstring>& outNames)
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
	DWORD CINIFormatter::GetValue(LPCTSTR pszSection, LPCTSTR pszKey, std::tstring& refValue)
	{
		std::tstring strSection	= Format(TEXT("[%s]"), pszSection);
		std::tstring strKey		= pszKey;
		std::tstring strLastSection;

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
				strLastSection = (*iter);
				continue;
			}

			if( !strLastSection.empty() && strLastSection != strSection )
				continue;

			if( strLastSection.empty() && strSection != TEXT("[DEFAULT]") )
				continue;

			// Value TEST
			{
				size_t tSepIndex = strCur.find(TEXT("="));
				if( std::tstring::npos == tSepIndex )
					continue;

				std::tstring strCurKey = Trim(strCur.substr(0, tSepIndex).c_str());
				if( strCurKey != strKey )
					continue;

				std::tstring strValue = Trim(strCur.substr(tSepIndex+1).c_str());
				size_t tStrValueLen  = strValue.length();

				// strap quotation
				if( tStrValueLen > 1 && strValue.at(0) == TEXT('\"') && strValue.at(tStrValueLen - 1) == TEXT('\"') )
					strValue = Trim(strValue.substr(1, tStrValueLen-2).c_str());
				
				refValue = strValue;
				return EC_SUCCESS;
			}
		}

		return EC_NO_DATA;
	}

	//////////////////////////////////////////////////////////////////////////
	DWORD CINIFormatter::GetValues(LPCTSTR pszSection, LPCTSTR pszKey, std::vector<std::tstring>& refValues)
	{
		std::tstring strSection	= Format(TEXT("[%s]"), pszSection);
		std::tstring strKey		= pszKey;

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
				bInTheSection = (*iter) == strSection;
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
	DWORD CINIFormatter::GetSectionValues(LPCTSTR pszSection, CStringMap& refMap)
	{
		std::tstring strSection	= Format(TEXT("[%s]"), pszSection);

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
				bInTheSection = (*iter) == strSection;
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
				strCurValue = Replace(strCurValue, TEXT("\""), TEXT(""));
				strCurValue = Trim(strCurValue);

				refMap.insert(std::make_pair(strCurKey, strCurValue));
			}
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	DWORD CINIFormatter::SetValue(LPCTSTR pszSection, LPCTSTR pszKey, LPCTSTR pszValue)
	{
		std::tstring strSection	= Format(TEXT("[%s]"), pszSection);
		std::tstring strKey		= pszKey;

		std::tstring strReplaceLine = strKey + TEXT("=") + pszValue;

		bool bInTheSection = false;

		CStringListIt iter;
		for(iter=m_listString.begin(); iter!=m_listString.end(); iter++)
		{
			std::tstring strCur = TrimRight((*iter).c_str());
			if( strCur.empty() )
				continue;

			// Skip comment
			if( strCur.at(0) == TEXT('#') ||  strCur.at(0) == TEXT(';') )
				continue;

			// Section TEST
			if( strCur.at(0) == TEXT('[') )
			{
				bool bPreSectionFlag = bInTheSection;
				bInTheSection = (*iter) == strSection;

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
				if( strCurKey != pszKey )
					continue;

				(*iter) = strReplaceLine;
				return EC_SUCCESS;
			}
		}

		if( !bInTheSection )
			m_listString.push_back(strSection);
		m_listString.push_back(strReplaceLine);
		return EC_SUCCESS;
	}
}

