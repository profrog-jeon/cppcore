#include "stdafx.h"

int MutexTest(LPCSTR pszKey, bool bRelease);
int main(int argc, char* argv[])
{
	if( argc < 2 )
	{
		printf("Not enough arguments.\n");
		return 0;
	}

	if( std::string(argv[1]) == "output_test" )
	{
		printf("%s", argv[2]);
		return 0;
	}

	core::ST_FUNC_LOG stLog(core::GetFileNameA());
	std::string strCommand(argv[1]);
	std::string strSubCommand = argc > 2? argv[2] : "";
	printf("current directory `%s`.\n", core::GetCurrentDirectoryA().c_str());
	printf("command `%s` `%s`.\n", strCommand.c_str(), strSubCommand.c_str());
	if( strCommand == "exitcode" )
		return atoi(argv[2]);

	if( strCommand == "filecheck" )
		return core::IsFileExistA(argv[2])? 0 : -1;

	if( strCommand == "hold" )
	{
        while(1)
        {
            printf("waiting forever...\n");
            core::Sleep(1000);
        }
		return 0;
	}

	if( strCommand == "semaphore_open_close" )
	{
		HANDLE hSemaphore = core::OpenSemaphoreA(argv[2]);
		if( NULL == hSemaphore )
			return -1;
		core::CloseSemaphore(hSemaphore);
		return 0;
	}

	if( strCommand == "semaphore_open_release_close" )
	{
		HANDLE hSemaphore = core::OpenSemaphoreA(argv[2]);
		if( NULL == hSemaphore )
			return -1;
		core::ReleaseSemaphore(hSemaphore);
		core::CloseSemaphore(hSemaphore);
		return 0;
	}

	if( strCommand == "semaphore_create_noclose" )
	{
		HANDLE hSemaphore = core::CreateSemaphoreA(0, argv[2]);
		*(int*)NULL = 1;	// evoke crash
		if( NULL == hSemaphore )
			return -1;
		return 0;
	}

	if( strCommand == "semaphore_open_noclose" )
	{
		HANDLE hSemaphore = core::OpenSemaphoreA(argv[2]);
		if( NULL == hSemaphore )
			return -1;
		return 0;
	}

	if( strCommand == "mutex_trywait" )
		return MutexTest(argv[2], false);

	if( strCommand == "mutex_trywait_and_release" )
		return MutexTest(argv[2], true);

	if( strCommand == "mutex_owner_create" )
	{
		HANDLE hMutex = core::CreateMutexA(true, argv[2]);
		if( NULL == hMutex )
			return -1;
		core::Sleep(0xFFFFFFFF);
		core::CloseMutex(hMutex);
		return 0;
	}

	printf("Undefined command. [%s]\n", strCommand.c_str());
	return 0;
}

int MutexTest(LPCSTR pszKey, bool bRelease)
{
	HANDLE hOpenMutex = core::OpenMutexA(pszKey);
	if( NULL == hOpenMutex )
		return -1;

	bool bSucceed = (EC_SUCCESS == core::WaitForMutex(hOpenMutex, 10));
	if( bRelease && bSucceed )
	{
		core::ReleaseMutex(hOpenMutex);
		printf("release mutex\n");
	}
	core::CloseMutex(hOpenMutex);
	return bSucceed? 0 : -2;
}
