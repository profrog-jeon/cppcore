#pragma once

#include <string>
#include <map>
#include <vector>
#include <list>

#include "../__Common/Type.h"
#include "../__Common/Define.h"
#include "IChannel.h"

namespace fmt_internal
{
	typedef std::map<std::tstring, std::tstring>				CStringMap;
	typedef std::map<std::tstring, std::tstring>::iterator		CStringMapIt;

	class CINIFormatter
	{
	private:
		typedef			std::list<std::tstring>				CStringList;
		typedef			std::list<std::tstring>::iterator	CStringListIt;

		CStringList		m_listString;

	protected:
		std::tstring	m_strFilename;

	public:
						CINIFormatter(void);
		virtual			~CINIFormatter(void);

		DWORD			Open(std::tstring strContext);
		void			Close(void);

		DWORD			Flush(core::IChannel& channel);

		DWORD			GetSectionNames(std::vector<std::tstring>& outNames);
		DWORD			GetValue(LPCTSTR pszSection, LPCTSTR pszKey, std::tstring& refValue);
		DWORD			GetValues(LPCTSTR pszSection, LPCTSTR pszKey, std::vector<std::tstring>& refValues);
		DWORD			GetSectionValues(LPCTSTR pszSection, CStringMap& refMap);

		DWORD			SetValue(LPCTSTR pszSection, LPCTSTR pszKey, LPCTSTR pszValue);
	};
}
