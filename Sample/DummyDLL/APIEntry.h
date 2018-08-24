#pragma once

#include "../../Inc/cppcore.h"

enum E_CMD
{
	CMD_SET_VALUE = 0,
	CMD_GET_VALUE,
	CMD_ANALYZE_FILE,
};

DECLARE_EXPORT HANDLE Create(void);
DECLARE_EXPORT ECODE Operate(HANDLE hContext, E_CMD nCmd, LPCSTR pszInput, LPCSTR pszOutput);
DECLARE_EXPORT void Destroy(HANDLE hContext);
