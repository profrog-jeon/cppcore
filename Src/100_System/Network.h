#pragma once

#include "../__Common/Define.h"
#include "../__Common/Type.h"
#include "../__Common/ErrorCode.h"
#include "Struct.h"
#include <vector>

#ifdef UNICODE
#define QueryRouteInfo						QueryRouteInfoW
#define QueryEthernetInfo					QueryEthernetInfoW
#define QueryDNSInfo						QueryDNSInfoW
#define GetNetworkInfo						GetNetworkInfoW
#define ParseNetworkInfo					ParseNetworkInfoW
#define ParsingDNSContext					ParsingDNSContextW
#define ModifyDNSContext					ModifyDNSContextW
#else
#define QueryRouteInfo						QueryRouteInfoA
#define QueryEthernetInfo					QueryEthernetInfoA
#define QueryDNSInfo						QueryDNSInfoA
#define GetNetworkInfo						GetNetworkInfoA
#define ParseNetworkInfo					ParseNetworkInfoA
#define ParsingDNSContext					ParsingDNSContextA
#define ModifyDNSContext					ModifyDNSContextA
#endif

namespace core
{
	ECODE			QueryRouteInfoA(std::vector<ST_ROUTEINFOA>& outInfo);
	ECODE			QueryRouteInfoW(std::vector<ST_ROUTEINFOW>& outInfo);
	ECODE			QueryEthernetInfoA(std::vector<ST_ETHERNETINFOA>& outInfo);
	ECODE			QueryEthernetInfoW(std::vector<ST_ETHERNETINFOW>& outInfo);
	ECODE			QueryDNSInfoA(std::vector<std::string>& outInfo);
	ECODE			QueryDNSInfoW(std::vector<std::wstring>& outInfo);
	ECODE			GetNetworkInfoA(ST_NETWORKINFOA& outInfo);
	ECODE			GetNetworkInfoW(ST_NETWORKINFOW& outInfo);
	ECODE			ParseNetworkInfoA(std::string strEthName, const ST_NETWORKINFOA& stNetworkInfo, ST_ETH_NETWORKA& outInfo);
	ECODE			ParseNetworkInfoW(std::wstring strEthName, const ST_NETWORKINFOW& stNetworkInfo, ST_ETH_NETWORKW& outInfo);

	void			ParsingDNSContextA(std::string strContext, std::vector<std::string>& outDNS);
	void			ParsingDNSContextW(std::string strContext, std::vector<std::wstring>& outDNS);
	void			ModifyDNSContextA(std::string& strContext, const std::vector<std::string>& newDNS);
	void			ModifyDNSContextW(std::string& strContext, const std::vector<std::wstring>& newDNS);
}
