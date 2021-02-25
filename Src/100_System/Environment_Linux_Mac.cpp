#include "stdafx.h"
#include "Environment.h"
#ifdef __APPLE__
#include "System_Mac.h"
#else
#include <sys/sysinfo.h>
#endif
#include "Log.h"
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <dlfcn.h>
#include <pthread.h>

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	void Sleep(DWORD dwMilliSec)
	{
		if( dwMilliSec < 1000000 )
			::usleep(dwMilliSec * 1000);
		else
		{
			DWORD dwSec = dwMilliSec / 1000;
			DWORD dwMicroSec = (dwMilliSec - dwSec * 1000) * 1000;
			::sleep(dwSec);
			::usleep(dwMicroSec);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE GetLastError(void)
	{
		return errno;
	}

	//////////////////////////////////////////////////////////////////////////
	void SetLastError(ECODE nCode)
	{
		errno = nCode;
	}

	//////////////////////////////////////////////////////////////////////////
	static inline ST_SYSTEMTIME SystemTimeFromTM(struct tm *pstTM, suseconds_t uSec)
	{
		ST_SYSTEMTIME stRet = { 0, };
		stRet.wYear			= pstTM->tm_year+1900;
		stRet.wMonth		= pstTM->tm_mon+1;
		stRet.wDayOfWeek	= pstTM->tm_wday;
		stRet.wDay			= pstTM->tm_mday;
		stRet.wHour			= pstTM->tm_hour;
		stRet.wMinute		= pstTM->tm_min;
		stRet.wSecond		= pstTM->tm_sec;
		stRet.wMilliseconds	= uSec/1000;
		return stRet;
	}
	
	//////////////////////////////////////////////////////////////////////////
	void GetLocalTime(ST_SYSTEMTIME* pOutTime)
	{
		timeval stTimeVal = { 0, };
		::gettimeofday(&stTimeVal, NULL);
		tm* pTM = ::localtime(&stTimeVal.tv_sec);
		*pOutTime = SystemTimeFromTM(pTM, stTimeVal.tv_usec);
	}

	//////////////////////////////////////////////////////////////////////////
	bool SetLocalTime(const ST_SYSTEMTIME* pInTime)
	{
		UINT64 uUnixTime = UnixTimeFrom(*pInTime);

		ST_TIME_ZONE_INFORMATION stTimeZone = { 0, };
		GetTimeZoneInformation(&stTimeZone);

		struct timeval stTimeVal;
		stTimeVal.tv_sec = (long)uUnixTime + stTimeZone.Bias * 60;
		stTimeVal.tv_usec= pInTime->wMilliseconds * 1000;

		if( ::settimeofday(&stTimeVal,NULL) < 0 )
			return false;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	void GetSystemTime(ST_SYSTEMTIME* pOutTime)
	{
		timeval stTimeVal = { 0, };
		::gettimeofday(&stTimeVal, NULL);
		tm* pTM = ::gmtime(&stTimeVal.tv_sec);
		*pOutTime = SystemTimeFromTM(pTM, stTimeVal.tv_usec);
	}

	//////////////////////////////////////////////////////////////////////////
	bool SetSystemTime(const ST_SYSTEMTIME* pInTime)
	{
		UINT64 uUnixTime = UnixTimeFrom(*pInTime);		

		struct timeval stTimeVal;
		stTimeVal.tv_sec = (long)uUnixTime;
		stTimeVal.tv_usec= pInTime->wMilliseconds * 1000;

		if( ::settimeofday(&stTimeVal,NULL) < 0 )
			return false;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE GetCurrentThread(void)
	{
		return (HANDLE)::pthread_self();
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE GetCurrentProcess(void)
	{
		return (HANDLE)(size_t)::getpid();
	}

	//////////////////////////////////////////////////////////////////////////
	DWORD GetCurrentThreadId(void)
	{
	#ifdef __APPLE__
		return syscall(SYS_thread_selfid);
	#else
		return syscall(SYS_gettid);
	#endif
	}
	
	//////////////////////////////////////////////////////////////////////////
	DWORD GetCurrentProcessId(void)
	{
		return ::getpid();
	}
	
	//////////////////////////////////////////////////////////////////////////
	DWORD GetTickCount(void)
	{
		timespec stTimeSpec = { 0, };
		if( ::clock_gettime(CLOCK_MONOTONIC, &stTimeSpec) )
		{
			timeval stTimeVal;
			::gettimeofday(&stTimeVal, NULL);
			return (DWORD)(stTimeVal.tv_sec*1000 + stTimeVal.tv_usec/1000);
		}

		return stTimeSpec.tv_sec * 1000 + stTimeSpec.tv_nsec / 1000000;
	}
	
	//////////////////////////////////////////////////////////////////////////
	ECODE GetSystemInfo(ST_SYSTEMINFO* pSystemInfo)
	{
		pSystemInfo->dwNumberOfProcessors	= get_nprocs(); 
		
		//SYSTEM_INFO stSystemInfo = { 0, };
		//::GetSystemInfo(&stSystemInfo);
		
		//pSystemInfo->dwPageSize						= stSystemInfo.dwPageSize					;
		//pSystemInfo->lpMinimumApplicationAddress	= stSystemInfo.lpMinimumApplicationAddress	;
		//pSystemInfo->lpMaximumApplicationAddress	= stSystemInfo.lpMaximumApplicationAddress	;
		//pSystemInfo->dwActiveProcessorMask			= stSystemInfo.dwActiveProcessorMask		;
		//pSystemInfo->dwNumberOfProcessors			= stSystemInfo.dwNumberOfProcessors			;
		//pSystemInfo->dwProcessorType				= stSystemInfo.dwProcessorType				;
		//pSystemInfo->dwAllocationGranularity		= stSystemInfo.dwAllocationGranularity		;
		//pSystemInfo->wProcessorLevel				= stSystemInfo.wProcessorLevel				;
		//pSystemInfo->wProcessorRevision				= stSystemInfo.wProcessorRevision			;
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE QueryPerformanceCounter(QWORD* pOutValue)
	{
		struct timeval stTimeVal = { 0, };
		::gettimeofday(&stTimeVal, NULL);

		QWORD qwMicroTime = (QWORD)stTimeVal.tv_usec + (QWORD)stTimeVal.tv_sec * 1000 * 1000;
		*pOutValue = qwMicroTime;
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE QueryPerformanceFrequency(QWORD* pOutValue)
	{
		*pOutValue = 1000 * 1000;
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	E_OS_TYPE GetOSType(void)
	{
		struct utsname uts;
		memset(&uts, 0x00, sizeof(struct utsname));
		::uname(&uts);

		// #uname -a: Linux profrog-ubuntu 4.4.0-31-generic #50-Ubuntu SMP Wed Jul 13 00:07:12 UTC 2016 x86_64 x86_64 x86_64 GNU/Linux
		//printf("%s\n", uts.sysname);	// Linux
		//printf("%s\n", uts.nodename);	// profrog-ubuntu
		//printf("%s\n", uts.release);	// 4.4.0-31-generic
		//printf("%s\n", uts.version);	// #50-Ubuntu SMP Wed Jul 13 00:07:12 UTC 2016
		//printf("%s\n", uts.machine);	// x86_64
		//printf("%s\n", uts.domainname);	// (none)
		if( 0 == ::strcasecmp(uts.machine, "x86_64") )
			return OS_TYPE_LINUX_X64;

		return OS_TYPE_LINUX_X86;
	}

	//////////////////////////////////////////////////////////////////////////
	FP_TerminateSignalCallback g_fpTerminationSignalCallback = NULL;
	static void SignalCtrlHandler(int sig)
	{
		if( g_fpTerminationSignalCallback )
			g_fpTerminationSignalCallback();
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE RegisterTerminateSignalCallback(FP_TerminateSignalCallback fpCallback)
	{
		if( NULL == fpCallback )
			return EC_INVALID_ARGUMENT;

		::signal(SIGINT, SignalCtrlHandler);
		::signal(SIGTERM, SignalCtrlHandler);
		g_fpTerminationSignalCallback = fpCallback;
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	void* GetProcAddress(HANDLE hModule, const char* pszProcName)
	{
		void* pRet = (void*)dlsym(hModule, pszProcName);
		if( pRet )
			return pRet;

		errno = EINVAL;
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	bool FreeLibrary(HANDLE hModule)
	{
		int nRet = ::dlclose(hModule);
		if( nRet )
		{
			Log_Error("dlclose error,%d, %s\n", nRet, ::dlerror());
			return false;
		}
		return true;
	}
}