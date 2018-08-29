#include "stdafx.h"
#include "Environment.h"
#include "Information.h"
#include "Log.h"
#include "TZFileParser.h"
#include <string.h>

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	ECODE GetNetworkInfo(ST_NETWORKINFO& outInfo)
	{
		QueryEthernetInfo(outInfo.vecEthernet);
		QueryRouteInfo(outInfo.vecRoute);
		QueryDNSInfo(outInfo.vecDNS);
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE ParseNetworkInfo(std::tstring strEthName, const ST_NETWORKINFO& stNetworkInfo, ST_ETH_NETWORK& outInfo)
	{
		DWORD dwIPAddress = 0;

		size_t i;
		for(i=0; i<stNetworkInfo.vecEthernet.size(); i++)
		{
			if( strEthName != stNetworkInfo.vecEthernet[i].strName )
				continue;

			dwIPAddress = stNetworkInfo.vecEthernet[i].dwIPAddress;
			outInfo.strEthName = stNetworkInfo.vecEthernet[i].strName;
			outInfo.strIP = IPAddressFrom(stNetworkInfo.vecEthernet[i].dwIPAddress);
			outInfo.strIPMask = IPAddressFrom(stNetworkInfo.vecEthernet[i].dwIPMask);
		}

		if( outInfo.strIP.empty() )
		{
			Log_Error(TEXT("Unknown ethernet name[%s]"), strEthName.c_str());
			return EC_NO_DATA;
		}

		for(i=0; i<stNetworkInfo.vecRoute.size(); i++)
		{
			const ST_ROUTEINFO& stRouteInfo = stNetworkInfo.vecRoute[i];

			if( (stRouteInfo.dwGenmask & dwIPAddress) != stRouteInfo.dwDest )
				continue;

			// for linux
			if( 0 == stRouteInfo.dwGateway )
				continue;

			// for windows
			if( stRouteInfo.dwSrc == stRouteInfo.dwGateway )
				continue;

			outInfo.strIFName = stRouteInfo.strIFName;
			outInfo.strGateway = IPAddressFrom(stRouteInfo.dwGateway);
		}

		if( stNetworkInfo.vecDNS.size() >= 1 )
			outInfo.strDNS0 = stNetworkInfo.vecDNS[0];

		if( stNetworkInfo.vecDNS.size() >= 2 )
			outInfo.strDNS1 = stNetworkInfo.vecDNS[1];

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	void ParsingDNSContext(std::string strContext, std::vector<std::tstring>& outDNS)
	{
		int nPos = 0;
		while(1)
		{
			if( nPos < 0 )
				break;

			std::string strLine = Tokenize(strContext, "\n", nPos);
			if( strLine.empty() )
				continue;

			if( strLine.at(0) == '#' )
				continue;

			std::string strValue;
			std::string strKey = Split(strLine, " ", &strValue);
			if( strKey != "nameserver" )
				continue;

			Trim(strValue);
			outDNS.push_back(TCSFromMBS(strValue));
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void ModifyDNSContext(INOUT std::string& strContext, const std::vector<std::tstring>& newDNS)
	{
		std::string strNewContext;

		// Truncate exist nameserver context
		int nPos = 0;
		while(1)
		{
			if( nPos < 0 )
				break;

			std::string strLine = Tokenize(strContext, "\n", nPos);
			if( strLine.empty() )
				continue;

			if( strLine.at(0) == '#' )
				continue;

			std::string strValue;
			std::string strKey = Split(strLine, " ", &strValue);
			if( strKey == "nameserver" )
				continue;

			strNewContext += strLine + "\n";
		}

		size_t i;
		for(i=0; i<newDNS.size(); i++)
		{
			std::string strDNS = MBSFromTCS(newDNS[i]);
			std::string strNewLine = Format("nameserver %s\n", strDNS.c_str());
			strNewContext += strNewLine;
		}

		strContext = strNewContext;
	}

	//////////////////////////////////////////////////////////////////////////
	ETIMEZONE GetTimeZoneInformationByTZFile(ST_TIME_ZONE_INFORMATION* pOut, LPCSTR pszFilePath, ST_SYSTEMTIME stGMT)
	{
		// Read TZFile data
		ST_TZ_INFO stTZInfo;
		ECODE dwRet = ParseTZFile(pszFilePath, stTZInfo);
		if( EC_SUCCESS != dwRet )
		{
			Log_Error("GetTimeZoneInformation calling failure");
			return TIME_ZONE_ID_INVALID_;
		}

		// Initialize data
		pOut->StandardBias = 0;
		pOut->DaylightBias = 0;

		// Get DST and ST
		DWORD dwUTC = (DWORD)UnixTimeFrom(stGMT);
		ETIMEZONE nRet = TIME_ZONE_ID_UNKNOWN_;
		ST_TZ_TYPE* pLastTZ = NULL;

		CTZTypeMapIt iter;
		for(iter=stTZInfo.mapTransitTime.begin(); iter!=stTZInfo.mapTransitTime.end(); iter++)
		{
			ST_SYSTEMTIME stIterDate = SystemTimeFrom(iter->first);
			if( stIterDate.wYear > stGMT.wYear )
				break;

			ST_TZ_TYPE* pIterTZ = &iter->second;
			if( iter->first <= dwUTC )
				nRet = pIterTZ->IsDST? TIME_ZONE_ID_DAYLIGHT_ : TIME_ZONE_ID_STANDARD_;

			if( stIterDate.wYear == stGMT.wYear )
			{
				int nBiasSec = pLastTZ? pLastTZ->nGMTOffset : pOut->Bias * 60;
				if( pIterTZ->IsDST )
				{
					pOut->DaylightBias = pIterTZ->nGMTOffset / -60;
					// 여기 이상함, GMT가 아닌 로컬 타임인데도 GMT로 계산해서 맞아들어감
					pOut->DaylightDate = SystemTimeFrom(iter->first + nBiasSec);
				}
				else
				{
					pOut->StandardBias = pIterTZ->nGMTOffset / -60;
					pOut->StandardDate = SystemTimeFrom(iter->first + nBiasSec);
				}
			}

			pLastTZ = pIterTZ;
		}

		// check current year has TZ information
		if( (0 == pOut->DaylightDate.wMonth) && (0 == pOut->DaylightDate.wMonth)  )
			return TIME_ZONE_ID_UNKNOWN_;

		// convert windows bias type and return
		pOut->StandardBias = pOut->StandardBias - pOut->Bias;
		pOut->DaylightBias = pOut->DaylightBias - pOut->Bias;
		return nRet;
	}

}