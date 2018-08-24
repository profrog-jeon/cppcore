#pragma once

#include "../DummyDLL/APIEntry.h"

class CDllWrapper
{
	HANDLE m_hModule;

	typedef HANDLE (*FP_Create)(void);
	typedef ECODE (*FP_Operate)(HANDLE hContext, E_CMD nCmd, LPCSTR pszInput, LPSTR pszOutput);
	typedef void (*FP_Destroy)(HANDLE hContext);

	FP_Create m_fpCreate;
	FP_Operate m_fpOperate;
	FP_Destroy m_fpDestroy;

public:
	CDllWrapper();
	~CDllWrapper();

	bool LoadLibrary(LPCTSTR pszFilePath);
	void FreeLibrary(void);

	HANDLE Create(void);
	ECODE Operate(HANDLE hContext, E_CMD nCmd, LPCSTR pszInput, LPSTR pszOutput);
	void Destroy(HANDLE hContext);
};

