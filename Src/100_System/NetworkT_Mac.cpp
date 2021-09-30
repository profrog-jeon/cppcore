#include "stdafx.h"
#include "Network.h"
#include "System_Mac.h"
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
#include <pwd.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <dlfcn.h>
#include <netinet/in.h>
#include <resolv.h>

namespace core
{
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
			Log_Debug("%s", e.what());
			return nRet;
		}
		
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE QueryEthernetInfo(std::vector<ST_ETHERNETINFO>& outInfo)
	{
		return EC_NOT_IMPLEMENTED;
	}
}
