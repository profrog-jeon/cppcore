#include "stdafx.h"
#include "APIEntry.h"

extern HANDLE g_hModule;

int nSomeVariable = 15;
int SomeFunc(int a, int b)
{
	return a + b;
}

ECODE GetName(char* pszBuffer, size_t tBufferSize)
{
	std::string strModuleName = core::GetModuleFileNameA(g_hModule);
	core::SafeStrCpy(pszBuffer, tBufferSize, strModuleName.c_str());
	return EC_SUCCESS;
}
