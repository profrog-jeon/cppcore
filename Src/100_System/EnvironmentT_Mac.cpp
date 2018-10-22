#include "stdafx.h"
#include "Environment.h"
#include "System_Mac.h"
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
	ECODE QueryRouteInfo(std::vector<ST_ROUTEINFO>& outInfo)
	{
		return EC_NOT_IMPLEMENTED;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE QueryDNSInfo(std::vector<std::tstring>& outInfo)
	{
		ECODE nRet = EC_SUCCESS;
		try
		{
			FILE* pFile = fopen("/etc/resolv.conf", "rt");
			if( NULL == pFile )
			{
				nRet = errno;
				throw exception_format("opening resolv.conf has failed, %s", strerror(errno));
			}

			std::string strResolvContents;
			while(!feof(pFile))
			{
				char szBuff[256] = { 0, };
				char* pRet = fgets(szBuff, 256, pFile);
				strResolvContents += szBuff;
			}
			fclose(pFile);

			ParsingDNSContext(strResolvContents, outInfo);
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			return nRet;
		}
		
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE QueryEthernetInfo(std::vector<ST_ETHERNETINFO>& outInfo)
	{
		return EC_NOT_IMPLEMENTED;
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
			SafeSPrintf(szUUID, 37, "%04x%04x-%04x-%04x-%04x-%04x%04x%04x",
				GetCurrentProcessId(), GetCurrentThread(),
				::rand()&0xffff,						// Generates a 32-bit Hex number
				((::rand() & 0x0fff) | 0x4000),			// Generates a 32-bit Hex number of the form 4xxx (4 indicates the UUID version)
				::rand() % 0x3fff + 0x8000,				// Generates a 32-bit Hex number in the range [0x8000, 0xbfff]
				::rand()&0xffff, ::rand()&0xffff, ::rand()&0xffff);		// Generates a 96-bit Hex number
		}
		else
		{
			size_t unused __attribute__((unused));
			unused = ::read(fd, szUUID, 36);
			szUUID[36]=0x00;
			close(fd);
		}

		return TCSFromMBS(szUUID);
	}

	//////////////////////////////////////////////////////////////////////////
	void OutputDebugString(const TCHAR* pszFormat, ...)
	{
		// [TODO]
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE GetFileVersionInfo(LPCTSTR pszFilePath, ST_VERSIONINFO* pVersionInfo)
	{
		// [TODO]
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE GetProductVersionInfo(LPCTSTR pszFilePath, ST_VERSIONINFO* pVersionInfo)
	{
		// [TODO]
		return EC_SUCCESS;
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

		return GetTimeZoneInformationByTZFile(pTimeZone, "/etc/localtime", stUTC);
	}
}
