#include "stdafx.h"
#include "DllWrapper.h"

CDllWrapper::CDllWrapper()
	: m_hModule(NULL)
	, m_fpCreate(NULL)
	, m_fpOperate(NULL)
	, m_fpDestroy(NULL)
{
}

CDllWrapper::~CDllWrapper()
{
}

bool CDllWrapper::LoadLibrary(LPCTSTR pszFilePath)
{
	m_hModule = core::LoadLibrary(pszFilePath);
	if( NULL == m_hModule )
		return false;

	m_fpCreate = (FP_Create)core::GetProcAddress(m_hModule, "Create");
	m_fpOperate = (FP_Operate)core::GetProcAddress(m_hModule, "Operate");
	m_fpDestroy = (FP_Destroy)core::GetProcAddress(m_hModule, "Destroy");
	return true;
}

void CDllWrapper::FreeLibrary(void)
{
	if( NULL == m_hModule )
		return;

	core::FreeLibrary(m_hModule);
	m_hModule = NULL;
}

HANDLE CDllWrapper::Create(void)
{
	return m_fpCreate();
}

ECODE CDllWrapper::Operate(HANDLE hContext, E_CMD nCmd, LPCSTR pszInput, LPSTR pszOutput)
{
	return m_fpOperate(hContext, nCmd, pszInput, pszOutput);
}

void CDllWrapper::Destroy(HANDLE hContext)
{
	return m_fpDestroy(hContext);
}