#include "stdafx.h"
#include "Environment.h"
#include "Information.h"
#ifdef __linux__
#include "System_Linux.h"
#else
#include "System_Mac.h"
#endif
#include "Log.h"
#include "ELFParser.h"
#include "FileSystem.h"
#include "TZFileParser.h"
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
#include <pwd.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <dlfcn.h>
#include <netinet/in.h>
#include <resolv.h>

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	std::tstring GetErrorString(ECODE nCode)
	{
		return TCSFromMBS(strerror(nCode));
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GetUserName(void)
	{
		const char* pszUserName = "unknown";
		passwd* pPasswd = getpwuid(geteuid());
		if( pPasswd )
			pszUserName = pPasswd->pw_name;
		return TCSFromMBS(pszUserName);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GetComputerName(void)
	{
		char szHostName[HOST_NAME_MAX+1] = { 0x00, };
		if( ::gethostname(szHostName, HOST_NAME_MAX+1) < 0 )
		{
			Log_Error("gethostname failure, %d(%s)", errno, strerror(errno));
			return TEXT("null");
		}

		return TCSFromMBS(szHostName);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GenerateGuid(void)
	{
		char szUUID[37] = { 0x00, };
		int fd = ::open("/proc/sys/kernel/random/uuid", O_RDONLY);
		if( fd < 0 )
		{
			SafeSPrintf(szUUID, 37, "%04X%04X-%04X-%04X-%04X-%04X%04X%04X",
				GetCurrentProcessId(), GetCurrentThread(),
				::rand()&0xffff,						// Generates a 32-bit Hex number
				((::rand() & 0x0fff) | 0x4000),			// Generates a 32-bit Hex number of the form 4xxx (4 indicates the UUID version)
				::rand() % 0x3fff + 0x8000,				// Generates a 32-bit Hex number in the range [0x8000, 0xbfff]
				::rand()&0xffff, ::rand()&0xffff, ::rand()&0xffff);		// Generates a 96-bit Hex number

			return TCSFromMBS(szUUID);
		}

		size_t unused __attribute__((unused));
		unused = ::read(fd, szUUID, 36);
		szUUID[36]=0x00;
		close(fd);
		return MakeUpper(TCSFromMBS(szUUID));
	}

	//////////////////////////////////////////////////////////////////////////
	void OutputDebugString(const TCHAR* pszFormat, ...)
	{
		// [TODO]
	}

	//////////////////////////////////////////////////////////////////////////
	ETIMEZONE GetTimeZoneInformation(ST_TIME_ZONE_INFORMATION* pTimeZone)
	{
		memset(pTimeZone, 0, sizeof(*pTimeZone));
		
		tzset();

		pTimeZone->Bias = timezone / 60;
		SafeStrCpy(pTimeZone->StandardName, 32, TCSFromMBS(tzname[0]).c_str());
		SafeStrCpy(pTimeZone->DaylightName, 32, TCSFromMBS(tzname[1]).c_str());

		if( 0 == daylight )
			return TIME_ZONE_ID_UNKNOWN_;

		ST_SYSTEMTIME stUTC;
		GetSystemTime(&stUTC);

		tzset();
		memset(pTimeZone, 0, sizeof(*pTimeZone));

		pTimeZone->Bias = timezone / 60;\
		SafeStrCpy(pTimeZone->StandardName, 32, TCSFromMBS(tzname[0]).c_str());
		SafeStrCpy(pTimeZone->DaylightName, 32, TCSFromMBS(tzname[1]).c_str());

		if( 0 == daylight )
			return TIME_ZONE_ID_UNKNOWN_;

		return GetTimeZoneInformationByTZFile(pTimeZone, TEXT("/etc/localtime"), stUTC);
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE LoadLibrary(LPCTSTR pszPath)
	{
		std::string strPath = MBSFromTCS(pszPath);
		HANDLE hRet = dlopen(strPath.c_str(), RTLD_NOW|RTLD_LOCAL);
		if( hRet )
			return hRet;
		Log_Error("dlopen(%s) has failed, %s", strPath.c_str(), dlerror());
		errno = EINVAL;
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE GetFileVersionInfo(LPCTSTR pszFilePath, ST_VERSIONINFO* pVersionInfo)
	{
#ifdef __linux__
		CELFParser ELF;
		ECODE nRet = ELF.Open(pszFilePath);
		if( EC_SUCCESS != nRet )
			return nRet;

		return ELF.QueryFileVersion(*pVersionInfo);
#else
		return EC_SUCCESS;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE GetProductVersionInfo(LPCTSTR pszFilePath, ST_VERSIONINFO* pVersionInfo)
	{
#ifdef __linux__
		CELFParser ELF;
		ECODE nRet = ELF.Open(pszFilePath);
		if( EC_SUCCESS != nRet )
			return nRet;

		return ELF.QueryProductVersion(*pVersionInfo);
#else
		return EC_SUCCESS;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	bool IpFromDomain(std::tstring strDomain, WORD wPort, std::string& out)
	{
		try
		{
			addrinfo* result = NULL;
			addrinfo* ptr = NULL;
			addrinfo hints;
			memset(&hints, 0, sizeof(hints));
			sockaddr_in* sockaddr_ipv4;
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;

			DWORD dwRes = ::getaddrinfo(MBSFromTCS(strDomain).c_str(), std::to_string(wPort).c_str(), &hints, &result);
			if (dwRes != 0)
				throw exception_format(TEXT("Get IP Address Failed, %d"), GetLastError());

			for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
			{
				if (ptr->ai_family != AF_INET)
					continue;

				sockaddr_ipv4 = (sockaddr_in*)ptr->ai_addr;
				out = IPAddressFromA(*(DWORD*)&sockaddr_ipv4->sin_addr);
			}

			::freeaddrinfo(result);
			return true;
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			return false;
		}
	}
}
