#include "APIEntry.h"

using namespace core;

struct ST_CONTEXT
{
	std::tstring strTimeStamp;
};

HANDLE Create(void)
{
	ST_CONTEXT* pContext = new ST_CONTEXT;

	ST_SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	pContext->strTimeStamp = Format(TEXT("%04d%02d%02dT%02d:%02d:%02d")
		, stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);
	return pContext;
}

ECODE Operate(HANDLE hContext, E_CMD nCmd, LPCSTR pszInput, LPCSTR pszOutput)
{
	ST_CONTEXT* pContext = (ST_CONTEXT*)hContext;

	switch( nCmd )
	{
	case CMD_SET_VALUE:
		printf("[CMD] SET_VALUE\n");
		break;

	case CMD_GET_VALUE:
		printf("[CMD] GET_VALUE\n");
		break;

	case CMD_ANALYZE_FILE:
		printf("[CMD] ANALYZE_FILE\n");
		break;
	}

	if( pszInput )
		printf("[INPUT] %s\n", pszInput);
	return EC_SUCCESS;
}

void Destroy(HANDLE hContext)
{
	ST_CONTEXT* pContext = (ST_CONTEXT*)hContext;
	delete pContext;
}