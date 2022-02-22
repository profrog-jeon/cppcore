#include "stdafx.h"
#include "Environment.h"
#include "Information.h"
#include "Log.h"
#include "TZFileParser.h"
#include <string.h>

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	ETIMEZONE GetTimeZoneInformationByTZFile(ST_TIME_ZONE_INFORMATION* pOut, LPCTSTR pszFilePath, ST_SYSTEMTIME stGMT)
	{
		// Read TZFile data
		ST_TZ_INFO stTZInfo;
		ECODE dwRet = ParseTZFile(MBSFromTCS(pszFilePath).c_str(), stTZInfo);
		if( EC_SUCCESS != dwRet )
		{
			Log_Debug("GetTimeZoneInformation calling failure");
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
		if( (0 == pOut->StandardDate.wMonth) && (0 == pOut->DaylightDate.wMonth)  )
			return TIME_ZONE_ID_UNKNOWN_;

		// convert windows bias type and return
		pOut->StandardBias = pOut->StandardBias - pOut->Bias;
		pOut->DaylightBias = pOut->DaylightBias - pOut->Bias;
		return nRet;
	}

}