#pragma once

namespace core
{
	struct ST_DUPLICATED_PROC_CHECKER
	{
		bool bDuplicated;
		ST_DUPLICATED_PROC_CHECKER(LPCSTR pszName = NULL);	// if pszName==NULL, will be used ModuleFileName

		bool IsDuplicated(void) {
			return bDuplicated;
		}
	};
}
