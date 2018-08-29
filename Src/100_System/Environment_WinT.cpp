#include "stdafx.h"
#include "Environment.h"
#include "Log.h"
#include "Information.h"
#undef TEXT
#include <Windows.h>
#include <IPTypes.h>
#include <Iphlpapi.h>
#pragma comment(lib, "version.lib")

namespace core
{
	//////////////////////////////////////////////////////////////////////////
#undef LoadLibrary
	HANDLE LoadLibrary(LPCTSTR pszPath)
	{
#ifdef UNICODE
		return ::LoadLibraryW(pszPath);
#else
		return ::LoadLibraryA(pszPath);
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GetErrorString(ECODE nCode)
	{
		const size_t	tBufSize = 256;
		TCHAR			szBuf[tBufSize] = { 0, };
		if( 0 == ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, nCode, 0, szBuf, tBufSize, NULL) )
		{
			Log_Error("FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, ... %u) calling failure, %u", nCode, ::GetLastError());
			return Format(TEXT("Error Code:%u"), nCode);
		}

		return std::tstring(szBuf);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GetUserName(void)
	{
		std::tstring strName = TEXT("unknown");

		DWORD dwLength = 0;
		::GetUserNameA(NULL, &dwLength);
		if( ERROR_INSUFFICIENT_BUFFER == GetLastError() && dwLength > 0)
		{
			strName.resize(dwLength - 1);
#ifdef UNICODE
			::GetUserNameW((TCHAR*)strName.c_str(), &dwLength);
#else
			::GetUserNameA((TCHAR*)strName.c_str(), &dwLength);
#endif
			return strName;
		}

		Log_Error("GetUserName calling failure");
		return TEXT("");
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GetComputerName(void)
	{
		TCHAR szName[MAX_COMPUTERNAME_LENGTH+1]= {0,};
		DWORD dwCchLen = MAX_COMPUTERNAME_LENGTH+1;

#ifdef UNICODE
		if( FALSE == ::GetComputerNameW(szName, &dwCchLen) )
#else
		if( FALSE == ::GetComputerNameA(szName, &dwCchLen) )
#endif
		{
			Log_Error("GetComputerName calling failure");
			return TEXT("");
		}

		return szName;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GenerateGuid(void)
	{
		GUID		guid		= {0,};
		std::tstring	strRet;

		if( SUCCEEDED(::CoCreateGuid(&guid)) )
		{
			strRet = Format(TEXT("%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X")
				, guid.Data1, guid.Data2, guid.Data3
				, guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3]
			, guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
		}
		else
		{
			Log_Warn("CoCreateGuid failure, generating random value");
			strRet = Format(TEXT("%04x%04x-%04x-%04x-%04x-%04x%04x%04x")
				, ::rand()&0xffff, ::rand()&0xffff						// Generates a 64-bit Hex number
				, ::rand()&0xffff										// Generates a 32-bit Hex number
				, ((::rand() & 0x0fff) | 0x4000)						// Generates a 32-bit Hex number of the form 4xxx (4 indicates the UUID version)
				, ::rand() % 0x3fff + 0x8000							// Generates a 32-bit Hex number in the range [0x8000, 0xbfff]
				, ::rand()&0xffff, ::rand()&0xffff, ::rand()&0xffff);	// Generates a 96-bit Hex number
		}

		return strRet;
	}

	//////////////////////////////////////////////////////////////////////////
	#undef GetFileVersionInfo
	ECODE GetFileVersionInfo(LPCTSTR pszFilePath, ST_VERSIONINFO* pVersionInfo)
	{
		try
		{
			DWORD dwHandle = 0;
			DWORD dwLen = ::GetFileVersionInfoSize(pszFilePath, &dwHandle); 
			if( 0 == dwLen )
				throw exception_format("GetFileVersionInfoSize calling failure");

			std::vector<BYTE> vecBuffer;
			vecBuffer.resize(dwLen);
			if( vecBuffer.size() != dwLen )
			{
				::SetLastError(ERROR_NOT_ENOUGH_MEMORY);
				throw exception_format("Memory alloc failure, size:%u", dwLen);
			}
#ifdef UNICODE
			if( !::GetFileVersionInfoW(pszFilePath, dwHandle, dwLen, &vecBuffer[0]) ) 
#else
			if( !::GetFileVersionInfoA(pszFilePath, dwHandle, dwLen, &vecBuffer[0]) ) 
#endif
				throw exception_format("GetFileVersionInfo calling failure");

			VS_FIXEDFILEINFO* pFileInfo = NULL;
			if( FALSE == ::VerQueryValue(&vecBuffer[0], TEXT("\\"), (LPVOID *)&pFileInfo, NULL) )
				throw exception_format("VerQueryValue calling failure");

			pVersionInfo->dwMajor = HIWORD(pFileInfo->dwFileVersionMS);
			pVersionInfo->dwMinor = LOWORD(pFileInfo->dwFileVersionMS);
			pVersionInfo->dwPatch = HIWORD(pFileInfo->dwFileVersionLS);
			pVersionInfo->dwBuild = LOWORD(pFileInfo->dwFileVersionLS);
			return EC_SUCCESS;
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
		}

		return ::GetLastError();
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE GetProductVersionInfo(LPCTSTR pszFilePath, ST_VERSIONINFO* pVersionInfo)
	{
		try
		{
			DWORD dwHandle = 0;
			DWORD dwLen = ::GetFileVersionInfoSize(pszFilePath, &dwHandle); 
			if( 0 == dwLen )
				throw exception_format("GetFileVersionInfoSize calling failure");

			std::vector<BYTE> vecBuffer;
			vecBuffer.resize(dwLen);
			if( vecBuffer.size() != dwLen )
			{
				::SetLastError(ERROR_NOT_ENOUGH_MEMORY);
				throw exception_format("Memory alloc failure, size:%u", dwLen);
			}
#ifdef UNICODE
			if( !::GetFileVersionInfoW(pszFilePath, dwHandle, dwLen, &vecBuffer[0]) ) 
#else
			if( !::GetFileVersionInfoA(pszFilePath, dwHandle, dwLen, &vecBuffer[0]) ) 
#endif
				throw exception_format("GetFileVersionInfo calling failure");

			VS_FIXEDFILEINFO* pFileInfo = NULL;
			if( FALSE == ::VerQueryValue(&vecBuffer[0], TEXT("\\"), (LPVOID *)&pFileInfo, NULL) )
				throw exception_format("VerQueryValue calling failure");

			pVersionInfo->dwMajor = HIWORD(pFileInfo->dwProductVersionMS);
			pVersionInfo->dwMinor = LOWORD(pFileInfo->dwProductVersionMS);
			pVersionInfo->dwPatch = HIWORD(pFileInfo->dwProductVersionLS);
			pVersionInfo->dwBuild = LOWORD(pFileInfo->dwProductVersionLS);
			return EC_SUCCESS;
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
		}

		return ::GetLastError();
	}

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
			Log_Error("%s", e.what());
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
			Log_Error("%s", e.what());
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
			Log_Error("%s", e.what());
			if( pNetworkParams )
				delete [] pNetworkParams;

			if( hIpHelper )
				::FreeLibrary(hIpHelper);
			return dwRet;
		}

		return ERROR_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
#undef OutputDebugString
	void OutputDebugString(const TCHAR* pszFormat, ...)
	{
		const size_t tBuffSize = 2048;
		TCHAR szBuff[tBuffSize] = { 0, };
		va_list list;
		va_start(list, pszFormat);
		SafeSVPrintf(szBuff, tBuffSize, pszFormat, list);
		va_end(list);
#ifdef UNICODE
		::OutputDebugStringW(szBuff);
#else
		::OutputDebugStringA(szBuff);
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	ETIMEZONE GetTimeZoneInformation(ST_TIME_ZONE_INFORMATION* pTimeZone)
	{
		::TIME_ZONE_INFORMATION stTemp = { 0, };
		DWORD dwRet = ::GetTimeZoneInformation(&stTemp);
		if( TIME_ZONE_ID_INVALID == dwRet )
			return TIME_ZONE_ID_INVALID_;

		memset(pTimeZone, 0, sizeof(*pTimeZone));
		pTimeZone->Bias = stTemp.Bias;
		SafeStrCpy(pTimeZone->StandardName, 32, TCSFromWCS(stTemp.StandardName).c_str());
		SafeStrCpy(pTimeZone->DaylightName, 32, TCSFromWCS(stTemp.DaylightName).c_str());
		if( TIME_ZONE_ID_UNKNOWN == dwRet )
			return TIME_ZONE_ID_UNKNOWN_;

		ST_SYSTEMTIME stGMT;
		GetSystemTime(&stGMT);
		MakeAbsoluteTZTime(stGMT.wYear, (ST_SYSTEMTIME&)stTemp.StandardDate, pTimeZone->StandardDate);
		MakeAbsoluteTZTime(stGMT.wYear, (ST_SYSTEMTIME&)stTemp.DaylightDate, pTimeZone->DaylightDate);
		pTimeZone->StandardBias = stTemp.StandardBias;
		pTimeZone->DaylightBias = stTemp.DaylightBias;
		return (ETIMEZONE)dwRet;
	}

	//////////////////////////////////////////////////////////////////////////
	ETIMEZONE GetTimeZoneInformation(const ST_SYSTEMTIME& stGMT, ST_TIME_ZONE_INFORMATION* pOutTimeZone)
	{
		::TIME_ZONE_INFORMATION stTemp = { 0, };
		DWORD dwRet = ::GetTimeZoneInformation(&stTemp);
		if( TIME_ZONE_ID_INVALID == dwRet )
		{
			Log_Error("GetTimeZoneInformation calling failure");
			return TIME_ZONE_ID_INVALID_;
		}

		if( TIME_ZONE_ID_UNKNOWN == dwRet )
			return TIME_ZONE_ID_UNKNOWN_;

		std::tstring strStandardName = TCSFromWCS(stTemp.StandardName);
		std::tstring strDaylightName = TCSFromWCS(stTemp.DaylightName);
		SafeStrCpy(pOutTimeZone->StandardName, 32, strStandardName.c_str());
		SafeStrCpy(pOutTimeZone->DaylightName, 32, strDaylightName.c_str());
		pOutTimeZone->Bias			 = stTemp.Bias			;
		pOutTimeZone->StandardBias	 = stTemp.StandardBias	;
		pOutTimeZone->DaylightBias	 = stTemp.DaylightBias	;

		if( !MakeAbsoluteTZTime(stGMT.wYear, (ST_SYSTEMTIME&)stTemp.StandardDate, pOutTimeZone->StandardDate) )
			memcpy(&pOutTimeZone->StandardDate, &stTemp.StandardDate, sizeof(stTemp.StandardDate));

		if( MakeAbsoluteTZTime(stGMT.wYear, (ST_SYSTEMTIME&)stTemp.DaylightDate, pOutTimeZone->DaylightDate) )
			memcpy(&pOutTimeZone->DaylightDate, &stTemp.DaylightDate, sizeof(stTemp.DaylightDate));

		// if window API returned difference year's absolute date, it is failure.
		if( pOutTimeZone->StandardDate.wYear != stGMT.wYear
		||  pOutTimeZone->DaylightDate.wYear != stGMT.wYear )
			return TIME_ZONE_ID_INVALID_;

		QWORD qwTargetTime = UnixTimeFrom(stGMT);
		QWORD qwDaylightTime = UnixTimeFrom(pOutTimeZone->DaylightDate);
		QWORD qwStandardTime = UnixTimeFrom(pOutTimeZone->StandardDate);

		if( qwDaylightTime < qwStandardTime )
		{
			if( qwTargetTime < qwDaylightTime )
				return TIME_ZONE_ID_STANDARD_;
			if( qwTargetTime < qwStandardTime )
				return TIME_ZONE_ID_DAYLIGHT_;
			return TIME_ZONE_ID_STANDARD_;
		}

		if( qwTargetTime < qwStandardTime )
			return TIME_ZONE_ID_DAYLIGHT_;
		if( qwTargetTime < qwDaylightTime )
			return TIME_ZONE_ID_STANDARD_;
		return TIME_ZONE_ID_DAYLIGHT_;
	}
}