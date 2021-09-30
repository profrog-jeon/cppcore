#include "stdafx.h"
#include "Network.h"
#include "Log.h"
#include "FileSystem.h"
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
#include <netinet/in.h>
#include <resolv.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

namespace core
{	
	//////////////////////////////////////////////////////////////////////////
	static int ReadNlSock(int nSock, char* pBuff, int nBuffSize, int nSeqNum, int nPID)
	{
		int nReadLen = 0;
		int nMsgLen = 0;

		struct nlmsghdr* pNLMsgHeader = NULL;
		do
		{
			/* Recieve response from the kernel */
			if( (nReadLen = ::recv(nSock, pBuff, nBuffSize - nMsgLen, 0)) < 0 )
			{
				Log_Debug("Reading route info has failed, %s", strerror(errno));
				return -1;
			}
			/* Check if the header is valid */
			pNLMsgHeader = (struct nlmsghdr*)pBuff;
			if( (NLMSG_OK(pNLMsgHeader, nReadLen) == 0) || (pNLMsgHeader->nlmsg_type == NLMSG_ERROR) )
			{
				Log_Debug("Invalid route info data");
				return -1;
			}

			/* Check if the its the last message */
			if( pNLMsgHeader->nlmsg_type == NLMSG_DONE )
				break;

			/* Else move the pointer to buffer appropriately */
			pBuff += nReadLen;
			nMsgLen += nReadLen;

			/* Check if its a multi part message */
			if( (pNLMsgHeader->nlmsg_flags & NLM_F_MULTI) == 0 )
				break;	/* return if its not */
		}	while ((pNLMsgHeader->nlmsg_seq != nSeqNum) || (pNLMsgHeader->nlmsg_pid != nPID));

		return nMsgLen;
	}
	
	//////////////////////////////////////////////////////////////////////////
	static DWORD ExtractGenmask(DWORD dwGateway)
	{
		DWORD dwRet = 0;

		size_t i;
		for(i=0; i<4; i++)
		{
			if( 0 == ((dwGateway >> i*8) & 0xFF) )
				continue;
			dwRet |= 0xFF << (i*8);
		}
		return dwRet;
	}

	//////////////////////////////////////////////////////////////////////////
	/* For parsing the route info returned */
	static bool ParseRoutes(struct nlmsghdr* pNLMsgHeader, ST_ROUTEINFO* pOutInfo)
	{
		struct rtmsg* pRTMsg = (struct rtmsg*)NLMSG_DATA(pNLMsgHeader);

		// If the route is not for AF_INET or does not belong to main routing table then return.
		if( (pRTMsg->rtm_family != AF_INET) || (pRTMsg->rtm_table != RT_TABLE_MAIN) )
			return false;

		pOutInfo->dwSrc = 0;
		pOutInfo->dwDest = 0;
		pOutInfo->dwGateway = 0;
		pOutInfo->dwGenmask = 0;

		// get the rtattr field
		int nRTLen = RTM_PAYLOAD(pNLMsgHeader);
		struct rtattr* pRTAttr = (struct rtattr*)RTM_RTA(pRTMsg);
		for(; RTA_OK(pRTAttr, nRTLen); pRTAttr=RTA_NEXT(pRTAttr, nRTLen))
		{
			//printf("type:%u, len:%lu, value:%08X\n", pRTAttr->rta_type, RTA_PAYLOAD(pRTAttr), *(u_int*)RTA_DATA(pRTAttr));
			switch (pRTAttr->rta_type)
			{
			case RTA_OIF:
				{
					char szBuff[128];
					if_indextoname(*(int*)RTA_DATA(pRTAttr), szBuff);
					pOutInfo->strIFName = TCSFromMBS(szBuff);
				}
				break;

			case RTA_GATEWAY:
				pOutInfo->dwGateway = *(u_int*)RTA_DATA(pRTAttr);
				break;

			case RTA_PREFSRC:
				pOutInfo->dwSrc = *(u_int*)RTA_DATA(pRTAttr);
				break;

			case RTA_DST:
				pOutInfo->dwDest = *(u_int*)RTA_DATA(pRTAttr);
				pOutInfo->dwGenmask = ExtractGenmask(*(u_int*)RTA_DATA(pRTAttr));
				break;
			}
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE QueryRouteInfo(std::vector<ST_ROUTEINFO>& outInfo)
	{
		ECODE nRet = EC_SUCCESS;
		const int nMsgBuffSize = 8192;
		char szMsgBuf[nMsgBuffSize];

		int nSock = 0;
		int nLen = 0;
		int nMsgSeq = 0;

		try
		{
			if( (nSock = ::socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE)) < 0 )
			{
				nRet = errno;
				throw exception_format("socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE) has failed, %s", strerror(errno));
			}

			memset(szMsgBuf, 0, nMsgBuffSize);

			/* point the header and the msg structure pointers into the buffer */
			struct nlmsghdr* pNLMsg = (struct nlmsghdr *) szMsgBuf;

			/* Fill in the nlmsg header*/
			pNLMsg->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));  // Length of message.
			pNLMsg->nlmsg_type = RTM_GETROUTE;   // Get the routes from kernel routing table .

			pNLMsg->nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST;    // The message is a request for dump.
			pNLMsg->nlmsg_seq = nMsgSeq++;    // Sequence of the message packet.
			pNLMsg->nlmsg_pid = getpid();    // PID of process sending the request.

			/* Send the request */
			if( send(nSock, pNLMsg, pNLMsg->nlmsg_len, 0) < 0 )
			{
				nRet = errno;
				throw exception_format("socket send(RTM_GETROUTE) has failed, %s", strerror(errno));
			}

			/* Read the response */
			if( (nLen = ReadNlSock(nSock, szMsgBuf, nMsgBuffSize, nMsgSeq, getpid())) < 0 )
			{
				nRet = errno;
				throw exception_format("socket read(RTM_GETROUTE) has failed, %s", strerror(errno));
			}

			/* Parse and print the response */
			//fprintf(stdout, "Destination\tGateway\tInterface\tSource\n");
			for(; NLMSG_OK(pNLMsg, nLen); pNLMsg=NLMSG_NEXT(pNLMsg, nLen))
			{
				ST_ROUTEINFO stRouteInfo;
				if( !ParseRoutes(pNLMsg, &stRouteInfo) )
					continue;

				outInfo.push_back(stRouteInfo);
			}
			close(nSock);
		}
		catch (std::exception& e)
		{
			Log_Debug("%s", e.what());
			if( nSock )
				close(nSock);
			return nRet;
		}		

		return EC_SUCCESS;
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
		struct ifaddrs* pIFAddr = NULL;
		DWORD dwRet = EC_SUCCESS;

		try 
		{
			if( ::getifaddrs(&pIFAddr) < 0 )
				throw exception_format(TEXT("getifaddrs failure, %d"), errno);

			int nSock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
			if( nSock < 0 )
				throw exception_format(TEXT("socket(AF_INET, SOCK_DGRAM, IPPROTO_IP) failure, %d"), errno);

			std::set<std::tstring> setEthernets;

			struct ifaddrs* pIFAddrIter = NULL;
			for (pIFAddrIter=pIFAddr; pIFAddrIter!=NULL; pIFAddrIter=pIFAddrIter->ifa_next)
			{
				if( pIFAddrIter->ifa_addr == NULL )
					continue;

				std::tstring strThisEthName = TCSFromMBS(pIFAddrIter->ifa_name);
				if( setEthernets.end() != setEthernets.find(strThisEthName) )
					continue;
				setEthernets.insert(strThisEthName);

				ST_ETHERNETINFO stEthernetInfo;
				stEthernetInfo.strName = strThisEthName;

				struct ifreq ifr = { 0, };
				SafeStrCpy(ifr.ifr_name, IFNAMSIZ, pIFAddrIter->ifa_name);
				if( ::ioctl(nSock, SIOCGIFADDR, &ifr) >= 0 )
					stEthernetInfo.dwIPAddress = *(DWORD*)&((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr;
				if( ::ioctl(nSock, SIOCGIFNETMASK, &ifr) >= 0 )
					stEthernetInfo.dwIPMask = *(DWORD*)&((struct sockaddr_in *)&ifr.ifr_netmask)->sin_addr;
				if( ::ioctl(nSock, SIOCGIFHWADDR, &ifr) >= 0 )
				{
					stEthernetInfo.strMAC = Format(TEXT("%02X-%02X-%02X-%02X-%02X-%02X"),
						(BYTE)ifr.ifr_hwaddr.sa_data[0], (BYTE)ifr.ifr_hwaddr.sa_data[1], (BYTE)ifr.ifr_hwaddr.sa_data[2],
						(BYTE)ifr.ifr_hwaddr.sa_data[3], (BYTE)ifr.ifr_hwaddr.sa_data[4], (BYTE)ifr.ifr_hwaddr.sa_data[5]);
				}

				outInfo.push_back(stEthernetInfo);
			}

			close(nSock);
			freeifaddrs(pIFAddr);
		}
		catch(std::exception &e)
		{
			Log_Debug("%s", e.what());
			if (pIFAddr != NULL)
				freeifaddrs(pIFAddr);
			return dwRet;
		}

		return dwRet;
	}
}