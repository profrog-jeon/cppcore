#include "stdafx.h"
#include "Network.h"
#include "Log.h"
#include "Information.h"
#undef TEXT
#undef GetTimeZoneInformation
#include <Windows.h>
#include <IPTypes.h>
#include <Iphlpapi.h>
#pragma comment(lib, "version.lib")

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	static std::tstring BuildMacString(BYTE btMAC[MAX_ADAPTER_ADDRESS_LENGTH], size_t tMACLength)
	{
		std::tstring strRet = Format(TEXT("%02X-%02X-%02X-%02X-%02X-%02X"), btMAC[0], btMAC[1], btMAC[2], btMAC[3], btMAC[4], btMAC[5]);
		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////	
	ECODE QueryRouteInfo(std::vector<ST_ROUTEINFO>& vecRouteInfo)
	{
		typedef DWORD (WINAPI *FPGetIpForwardTable)(PMIB_IPFORWARDTABLE, PULONG, BOOL);

		DWORD				dwRet				= ERROR_SUCCESS;
		HMODULE				hIpHelper			= NULL;
		PMIB_IPFORWARDTABLE	pInfo				= NULL;

		try
		{
			hIpHelper = ::LoadLibraryA("IPHLPAPI.DLL");
			if (NULL == hIpHelper)
			{
				dwRet = ::GetLastError();
				throw exception_format("IPHlpApi.dll loading error, %u", dwRet);
			}

			FPGetIpForwardTable fpGetIpForwardTable = (FPGetIpForwardTable)::GetProcAddress(hIpHelper, "GetIpForwardTable");
			dwRet = ::GetLastError();
			if( NULL == fpGetIpForwardTable )
				throw exception_format("IPHlpApi.dll obtaining GetIpForwardTable failure, %u", dwRet);

			ULONG uInfoSize = 0;
			dwRet = fpGetIpForwardTable(NULL, &uInfoSize, TRUE);
			if( ERROR_INSUFFICIENT_BUFFER != dwRet )
				throw exception_format("GetIpForwardTable(NULL) calling failure, %u", dwRet);

			pInfo = (PMIB_IPFORWARDTABLE)new BYTE[uInfoSize];
			dwRet = fpGetIpForwardTable(pInfo, &uInfoSize, TRUE);
			if( ERROR_SUCCESS != dwRet )
				throw exception_format("GetIpForwardTable calling failure, %u", dwRet);

			std::vector<ST_ETHERNETINFO> vecEthernet;
			QueryEthernetInfo(vecEthernet);

			std::map<DWORD, ST_ETHERNETINFO*> mapEthernetIP;
			size_t i;
			for(i=0; i<vecEthernet.size(); i++)
				mapEthernetIP.insert(std::make_pair(vecEthernet[i].dwIndex, &vecEthernet[i]));

			for(i=0; i<pInfo->dwNumEntries; i++)
			{
				ST_ROUTEINFO stRouteInfo;
				stRouteInfo.dwDest = pInfo->table[i].dwForwardDest;
				stRouteInfo.dwGenmask = pInfo->table[i].dwForwardMask;
				stRouteInfo.dwGateway = pInfo->table[i].dwForwardNextHop;
				stRouteInfo.dwSrc = 0x0100007F;		// 127.0.0.1
				if( mapEthernetIP.end() != mapEthernetIP.find(pInfo->table[i].dwForwardIfIndex) )
				{
					ST_ETHERNETINFO* pEthernetInfo = mapEthernetIP[pInfo->table[i].dwForwardIfIndex];
					stRouteInfo.strIFName = pEthernetInfo->strName;
					stRouteInfo.dwSrc = pEthernetInfo->dwIPAddress;
				}
				vecRouteInfo.push_back(stRouteInfo);

				///* Convert IPv4 addresses to strings */
				//printf("\n\tRoute[%d] Dest IP: %s\n", i, IPAddressFromA(stRouteInfo.dwDest).c_str());
				//printf("\tRoute[%d] Subnet Mask: %s\n", i, IPAddressFromA(stRouteInfo.dwGenmask).c_str());
				//printf("\tRoute[%d] Next Hop: %s\n", i, IPAddressFromA(stRouteInfo.dwGateway).c_str());
				//printf("\tRoute[%d] If : %s\n", i, IPAddressFromA(stRouteInfo.dwSrc).c_str());
				//printf("\tRoute[%d] Type: %ld - ", i, pInfo->table[i].dwForwardType);
				//switch (pInfo->table[i].dwForwardType)
				//{
				//case MIB_IPROUTE_TYPE_OTHER:
				//	printf("other\n");
				//	break;
				//case MIB_IPROUTE_TYPE_INVALID:
				//	printf("invalid route\n");
				//	break;
				//case MIB_IPROUTE_TYPE_DIRECT:
				//	printf("local route where next hop is final destination\n");
				//	break;
				//case MIB_IPROUTE_TYPE_INDIRECT:
				//	printf("remote route where next hop is not final destination\n");
				//	break;
				//default:
				//	printf("UNKNOWN Type value\n");
				//	break;
				//}

				//printf("\tRoute[%d] Proto: %ld - ", i, pInfo->table[i].dwForwardProto);
				//switch (pInfo->table[i].dwForwardProto)
				//{
				//case MIB_IPPROTO_OTHER:
				//	printf("other\n");
				//	break;
				//case MIB_IPPROTO_LOCAL:
				//	printf("local interface\n");
				//	break;
				//case MIB_IPPROTO_NETMGMT:
				//	printf("static route set through network management \n");
				//	break;
				//case MIB_IPPROTO_ICMP:
				//	printf("result of ICMP redirect\n");
				//	break;
				//case MIB_IPPROTO_EGP:
				//	printf("Exterior Gateway Protocol (EGP)\n");
				//	break;
				//case MIB_IPPROTO_GGP:
				//	printf("Gateway-to-Gateway Protocol (GGP)\n");
				//	break;
				//case MIB_IPPROTO_HELLO:
				//	printf("Hello protocol\n");
				//	break;
				//case MIB_IPPROTO_RIP:
				//	printf("Routing Information Protocol (RIP)\n");
				//	break;
				//case MIB_IPPROTO_IS_IS:
				//	printf("Intermediate System-to-Intermediate System (IS-IS) protocol\n");
				//	break;
				//case MIB_IPPROTO_ES_IS:
				//	printf("End System-to-Intermediate System (ES-IS) protocol\n");
				//	break;
				//case MIB_IPPROTO_CISCO:
				//	printf("Cisco Interior Gateway Routing Protocol (IGRP)\n");
				//	break;
				//case MIB_IPPROTO_BBN:
				//	printf("BBN Internet Gateway Protocol (IGP) using SPF\n");
				//	break;
				//case MIB_IPPROTO_OSPF:
				//	printf("Open Shortest Path First (OSPF) protocol\n");
				//	break;
				//case MIB_IPPROTO_BGP:
				//	printf("Border Gateway Protocol (BGP)\n");
				//	break;
				//case MIB_IPPROTO_NT_AUTOSTATIC:
				//	printf("special Windows auto static route\n");
				//	break;
				//case MIB_IPPROTO_NT_STATIC:
				//	printf("special Windows static route\n");
				//	break;
				//case MIB_IPPROTO_NT_STATIC_NON_DOD:
				//	printf("special Windows static route not based on Internet standards\n");
				//	break;
				//default:
				//	printf("UNKNOWN Proto value\n");
				//	break;
				//}

				//printf("\tRoute[%d] Age: %ld\n", i, pInfo->table[i].dwForwardAge);
				//printf("\tRoute[%d] Metric1: %ld\n", i, pInfo->table[i].dwForwardMetric1);
			}

			delete [] pInfo;
			::FreeLibrary(hIpHelper);
		}
		catch (std::exception& e)
		{
			Log_Debug("%s", e.what());
			if( pInfo )
				delete [] pInfo;

			if( hIpHelper )
				::FreeLibrary(hIpHelper);
			return dwRet;
		}

		return ERROR_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE QueryEthernetInfo(std::vector<ST_ETHERNETINFO>& outInfo)
	{
		typedef DWORD (WINAPI *FPGetAdaptersInfo)(PIP_ADAPTER_INFO,PULONG);

		DWORD				dwRet				= ERROR_SUCCESS;
		HMODULE				hIpHelper			= NULL;
		PIP_ADAPTER_INFO	pInfo				= NULL;

		try
		{
			hIpHelper = ::LoadLibraryA("IPHLPAPI.DLL");
			if (NULL == hIpHelper)
			{
				dwRet = ::GetLastError();
				throw exception_format("IPHlpApi.dll loading error, %u", dwRet);
			}

			FPGetAdaptersInfo fpGetAdaptersInfo = (FPGetAdaptersInfo)::GetProcAddress(hIpHelper, "GetAdaptersInfo");
			dwRet = ::GetLastError();
			if (NULL == fpGetAdaptersInfo)
				throw exception_format("IPHlpApi.dll obtaining GetAdaptersInfo failure, %u", dwRet);

			// Call ::GetAdaptersInfo function
			DWORD dwLen = 0;
			dwRet = (*fpGetAdaptersInfo)(NULL, &dwLen);
			if( ERROR_BUFFER_OVERFLOW != dwRet )
				throw exception_format("GetAdaptersInfo(NULL) calling failure, %u", dwRet);

			pInfo = (PIP_ADAPTER_INFO)new(std::nothrow) char[dwLen];
			dwRet = (*fpGetAdaptersInfo)(pInfo, &dwLen);
			if( ERROR_SUCCESS != dwRet )
				throw exception_format("GetAdaptersInfo calling failure, %u", dwRet);

			PIP_ADAPTER_INFO pThis;
			for(pThis=pInfo; pThis; pThis=pThis->Next)
			{
				//if( MIB_IF_TYPE_ETHERNET != pThis->Type && IF_TYPE_IEEE80211 != pThis->Type && MIB_IF_TYPE_LOOPBACK != pThis->Type )
				//	continue;

				ST_ETHERNETINFO stEthernetInfo;
				stEthernetInfo.dwIndex = pThis->Index;
				stEthernetInfo.strName = TCSFromMBS(pThis->Description);
				stEthernetInfo.dwIPAddress = IPAddressFromA(pThis->IpAddressList.IpAddress.String);
				stEthernetInfo.dwIPMask = IPAddressFromA(pThis->IpAddressList.IpMask.String);
				if( pThis->Address )
					stEthernetInfo.strMAC = BuildMacString(pThis->Address, pThis->AddressLength);

				outInfo.push_back(stEthernetInfo);
			}

			delete [] pInfo;
			::FreeLibrary(hIpHelper);
		}
		catch (std::exception& e)
		{
			Log_Debug("%s", e.what());
			if( pInfo )
				delete [] pInfo;

			if( hIpHelper )
				::FreeLibrary(hIpHelper);
			return dwRet;
		}

		return ERROR_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE QueryDNSInfo(std::vector<std::tstring>& outInfo)
	{
		typedef DWORD (WINAPI *FPGetAdaptersInfo)(PIP_ADAPTER_INFO,PULONG);
		typedef DWORD (WINAPI *FPGetNetworkParams)(PFIXED_INFO, PULONG);

		DWORD				dwRet				= ERROR_SUCCESS;
		HMODULE				hIpHelper			= NULL;
		FIXED_INFO*			pNetworkParams		= NULL;

		try
		{
			hIpHelper = ::LoadLibraryA("IPHLPAPI.DLL");
			if (NULL == hIpHelper)
			{
				dwRet = ::GetLastError();
				throw exception_format("IPHlpApi.dll loading error, %u", dwRet);
			}

			FPGetNetworkParams fpGetNetworkParams = (FPGetNetworkParams)::GetProcAddress(hIpHelper, "GetNetworkParams");
			dwRet = ::GetLastError();
			if (NULL == fpGetNetworkParams)
				throw exception_format("IPHlpApi.dll obtaining GetNetworkParams failure, %u", dwRet);

			ULONG uInfoSize = 0;
			dwRet = fpGetNetworkParams(NULL, &uInfoSize);
			if( ERROR_BUFFER_OVERFLOW != dwRet )
				throw exception_format("GetNetworkParams(NULL) calling failure, %u", dwRet);

			pNetworkParams = (FIXED_INFO*)new BYTE[uInfoSize];
			dwRet = fpGetNetworkParams(pNetworkParams, &uInfoSize);
			if( ERROR_SUCCESS != dwRet )
				throw exception_format("GetNetworkParams calling failure, %u", dwRet);

			IP_ADDR_STRING* pAddrIter;
			for(pAddrIter=&pNetworkParams->DnsServerList; pAddrIter!=NULL; pAddrIter=pAddrIter->Next)
			{
				if( NULL == pAddrIter->IpAddress.String )
					continue;
				outInfo.push_back(TCSFromMBS(pAddrIter->IpAddress.String));
			}

			delete [] pNetworkParams;
			::FreeLibrary(hIpHelper);
		}
		catch (std::exception& e)
		{
			Log_Debug("%s", e.what());
			if( pNetworkParams )
				delete [] pNetworkParams;

			if( hIpHelper )
				::FreeLibrary(hIpHelper);
			return dwRet;
		}

		return ERROR_SUCCESS;
	}
}