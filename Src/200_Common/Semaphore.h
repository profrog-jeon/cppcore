#pragma once

namespace core
{
	class CSemaphore
	{
		HANDLE m_hSemaphore;

	public:
		CSemaphore(void);
		~CSemaphore(void);

		ECODE Create(int nInitialCount, const char* pszName = NULL);
		ECODE Create(int nInitialCount, const wchar_t* pszName = NULL);
		ECODE Open(const char* pszName);
		ECODE Open(const wchar_t* pszName);
		void Close(void);

		ECODE Release(int nReleaseCount = 1);
		ECODE Wait(DWORD dwTimeOut);
	};
}
