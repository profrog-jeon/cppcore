#pragma once


namespace core
{
	class exception_format : public std::exception
	{
	private:
		std::string		m_strWhatA;
		std::wstring	m_strWhatW;

	public:
		exception_format(const char* pszFormat, ...);
		exception_format(const wchar_t* pszFormat, ...);
		~exception_format(void) throw()	{}

		const char*	what(void) const throw()		{		return m_strWhatA.c_str();		}
		const wchar_t* whatW(void) const throw()	{		return m_strWhatW.c_str();		}

		const TCHAR* whatT(void) const throw()
		{
#ifdef UNICODE
			return m_strWhatW.c_str();
#else
			return m_strWhatA.c_str();
#endif
		}
	};
}

