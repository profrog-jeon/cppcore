#include "stdafx.h"
#include "DllWrapper.h"

int main(int nArgc, char* pszArgs[])
{
	CDllWrapper Dll;
	if( !Dll.LoadLibrary(TEXT("DummyDLL.dll")) )
		return -1;

	HANDLE hContext = Dll.Create();
	Dll.Operate(hContext, CMD_SET_VALUE, "File:SomeFile.exe", NULL);
	Dll.Destroy(hContext);
	Dll.FreeLibrary();
	return 0;
}

int process_test_main(int nArgc, char* pszArgs[])
{
	std::tstring strOutput;
	int nRet = ShellExecuteByPipe(TEXT("DummyProcess.exe"), strOutput);
	return 0;
}

