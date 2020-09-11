#pragma once

#include <string>
#include <map>
#include <vector>
#include <list>

#include "../__Common/Type.h"
#include "../__Common/Define.h"
#include "TextFileReader.h"
#include "TextFileWriter.h"

namespace core
{
	typedef std::multimap<std::string, std::string>					CStringMapA;
	typedef std::multimap<std::string, std::string>::iterator		CStringMapItA;
	typedef std::multimap<std::wstring, std::wstring>				CStringMapW;
	typedef std::multimap<std::wstring, std::wstring>::iterator		CStringMapItW;

	class CINIParserA
	{
	private:
		typedef			std::list<std::string>				CStringList;
		typedef			std::list<std::string>::iterator	CStringListIt;

		CStringList		m_listString;

	protected:
		std::string		m_strFilename;

	public:
						CINIParserA(void);
		virtual			~CINIParserA(void);

		ECODE			Open(CTextFileReader* pReader);
		ECODE			Open(std::string strContext);
		void			Close(void);

		ECODE			Flush(std::string& strContext);
		ECODE			Flush(CTextFileWriter* pWriter);

		ECODE			GetSectionNames(std::vector<std::string>& outNames);
		std::string		GetValue(std::string strSection, std::string strKey, std::string strDefault);
		ECODE			GetValues(std::string strSection, std::string strKey, std::vector<std::string>& outValuesVec);
		ECODE			GetSectionValues(std::string strSection, CStringMapA& outValuesMap);

		ECODE			SetValue(std::string strSection, std::string strKey, std::string strValue);
	};

	class CINIParserW
	{
	private:
		typedef			std::list<std::wstring>				CStringList;
		typedef			std::list<std::wstring>::iterator	CStringListIt;

		CStringList		m_listString;

	protected:
		std::wstring	m_strFilename;

	public:
		CINIParserW(void);
		virtual			~CINIParserW(void);

		ECODE			Open(CTextFileReader* pReader);
		ECODE			Open(std::wstring strContext);
		void			Close(void);

		ECODE			Flush(std::wstring& strContext);
		ECODE			Flush(CTextFileWriter* pWriter);

		ECODE			GetSectionNames(std::vector<std::wstring>& outNames);
		std::wstring	GetValue(std::wstring strSection, std::wstring strKey, std::wstring strDefault);
		ECODE			GetValues(std::wstring strSection, std::wstring strKey, std::vector<std::wstring>& outValuesVec);
		ECODE			GetSectionValues(std::wstring strSection, CStringMapW& outValuesMap);

		ECODE			SetValue(std::wstring strSection, std::wstring strKey, std::wstring strValue);
	};

#ifdef UNICODE
	typedef CINIParserW		CINIParser;
	typedef CStringMapW		CStringMap;
	typedef CStringMapItW	CStringMapIt;
#else
	typedef CINIParserA		CINIParser;
	typedef CStringMapA		CStringMap;
	typedef CStringMapItA	CStringMapIt;
#endif

}
