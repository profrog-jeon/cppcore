#include "stdafx.h"
#include "LogInstance.h"
#include "Log.h"
#include "KernelObject.h"
#include "Environment.h"
#include "FileSystem.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	ST_LOG_CONTEXT g_stLogContext;

	//////////////////////////////////////////////////////////////////////////
	ST_LOG_CONTEXT::ST_LOG_CONTEXT(void)
		: hFileCS(NULL), dwProcessID(0), dwInputFlag(LOG_DEFAULT), dwOutputFlag(LOG_OUTPUT_CONSOLE|LOG_OUTPUT_DBGWND), dwMaxFileSize(10 * 1000 * 1000), dwMaxFileCount(3), fpCustomOutput(NULL)
	{
		hFileCS			= InitializeCriticalSection();
		dwProcessID		= GetCurrentProcessId();
		strModuleName	= ExtractFileName(GetFileNameA());
	}

	//////////////////////////////////////////////////////////////////////////
	ST_LOG_CONTEXT::~ST_LOG_CONTEXT(void)
	{
	}
}