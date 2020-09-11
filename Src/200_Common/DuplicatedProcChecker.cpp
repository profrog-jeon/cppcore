#include "stdafx.h"
#include "DuplicatedProcChecker.h"

namespace core
{
	class DupCheckWorker
	{
		HANDLE m_hMutex;
		bool m_bDuplicated;

	public:
		DupCheckWorker(std::string strName)
			: m_hMutex(NULL)
			, m_bDuplicated(false)
		{
			m_hMutex = CreateMutexA(false, strName.c_str());
			m_bDuplicated = (EC_ALREADY_EXISTS == GetLastError());
		}

		~DupCheckWorker(void)
		{
			CloseMutex(m_hMutex);
		}

		bool IsDuplicated(void) { return m_bDuplicated; }
	};

	ST_DUPLICATED_PROC_CHECKER::ST_DUPLICATED_PROC_CHECKER(LPCSTR pszName)
	{
		std::string strModuleFileName = ExtractFileName(GetFileNameA());

		static DupCheckWorker g_DupCheckWorker(pszName == NULL? strModuleFileName : pszName);
		bDuplicated = g_DupCheckWorker.IsDuplicated();
	}
}
