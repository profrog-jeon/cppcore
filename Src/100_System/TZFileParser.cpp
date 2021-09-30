#include "stdafx.h"
#include "TZFileParser.h"
#include "FileSystem.h"
#include "Environment.h"
#include "Log.h"
#include <string.h>

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	struct INTERNAL_TZ_FILEHEADER
	{
		char szMarker[4];
		BYTE btVersion;
		BYTE btZero[15];

		DWORD dwGMTCount	;
		DWORD dwStdCount	;
		DWORD dwLeapCount	;
		DWORD dwTimeCount	;
		DWORD dwTypeCount	;
		DWORD dwCharLength	;
	};

	//////////////////////////////////////////////////////////////////////////
#pragma pack(push, 1)
	struct INTERNAL_TZ_TYPE
	{
		int nGMTOffset;
		BYTE IsDST;
		BYTE btAbbrind;
	};
#pragma pack(pop)

	//////////////////////////////////////////////////////////////////////////
	struct INTERNAL_TZ_LEAPSEC
	{
		DWORD dwLeapSec;
		DWORD dwTotalSec;
	};

	//////////////////////////////////////////////////////////////////////////
	struct INTERNAL_TZFILE_DATA
	{
		char szAbbreviation[TZ_MAX_CHARS];

		std::vector<INTERNAL_TZ_TYPE> vecType;
		std::vector<unsigned char> vecIsStd;
		std::vector<unsigned char> vecIsGmt;

		std::vector<DWORD> vecTransitTime;
		std::vector<unsigned char> vecTransitTimeIndex;
		
		std::vector<INTERNAL_TZ_LEAPSEC> vecLeapSec;
		
	};

	//////////////////////////////////////////////////////////////////////////
	static inline ECODE InternalParseTZFile(LPCTSTR pszFileName, INTERNAL_TZFILE_DATA& outData)
	{
		INTERNAL_TZ_FILEHEADER header;

		FILE* pFile = NULL;
		try
		{
			pFile = fopenT(pszFileName, TEXT("rb"));
			if( NULL == pFile )
				throw exception_format(TEXT("fopen(%s) has failed."), pszFileName);

			size_t tRead = fread(&header, 1, sizeof(header), pFile);
			if( sizeof(header) != tRead )
				throw exception_format(TEXT("TZFile(%s) header reading failure"), pszFileName);

			if( memcmp(header.szMarker, "TZif", 4) )
				throw exception_format(TEXT("TZFile(%s) marker is not matched"), pszFileName);

			MakeHostFromBE(header.dwGMTCount	);
			MakeHostFromBE(header.dwStdCount	);
			MakeHostFromBE(header.dwLeapCount	);
			MakeHostFromBE(header.dwTimeCount	);
			MakeHostFromBE(header.dwTypeCount	);
			MakeHostFromBE(header.dwCharLength	);

			if( header.dwTimeCount )
			{
				outData.vecTransitTime.resize(header.dwTimeCount);
				outData.vecTransitTimeIndex.resize(header.dwTimeCount);
				fread(&outData.vecTransitTime[0], sizeof(outData.vecTransitTime[0]), header.dwTimeCount, pFile);
				fread(&outData.vecTransitTimeIndex[0], 1, header.dwTimeCount, pFile);
			}

			if( header.dwTypeCount )
			{
				outData.vecType.resize(header.dwTypeCount);
				fread(&outData.vecType[0], sizeof(outData.vecType[0]), header.dwTypeCount, pFile);
			}

			if( header.dwCharLength >= TZ_MAX_CHARS )
				throw exception_format(TEXT("TZFile(%s) char len:%u exceeded max:%u is not matched"), pszFileName, header.dwCharLength, TZ_MAX_CHARS);
			fread(outData.szAbbreviation, 1, header.dwCharLength, pFile);

			if( header.dwLeapCount )
			{
				outData.vecLeapSec.resize(header.dwLeapCount);
				fread(&outData.vecLeapSec[0], sizeof(outData.vecLeapSec[0]), header.dwLeapCount, pFile);
			}

			if( header.dwStdCount )
			{
				outData.vecIsStd.resize(header.dwStdCount);
				fread(&outData.vecIsStd[0], 1, header.dwStdCount, pFile);
			}

			if( header.dwGMTCount )
			{
				outData.vecIsGmt.resize(header.dwGMTCount);
				fread(&outData.vecIsGmt[0], 1, header.dwGMTCount, pFile);
			}

			fclose(pFile);
			pFile = NULL;
		}
		catch (std::exception& e)
		{
			Log_Debug("%s", e.what());
			if( pFile )
				fclose(pFile);
			return EC_INVALID_DATA;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE ParseTZFile(LPCTSTR pszFileName, ST_TZ_INFO& outInfo)
	{
		INTERNAL_TZFILE_DATA stTZData;
		ECODE nRet = InternalParseTZFile(pszFileName, stTZData);
		if( EC_SUCCESS != nRet )
			return nRet;

		if( stTZData.vecType.empty() )
			return EC_INVALID_DATA;

		outInfo.vecType.resize(stTZData.vecType.size());
		memset(&outInfo.vecType[0], 0, sizeof(outInfo.vecType[0]) * outInfo.vecType.size());
		memcpy(outInfo.szAbbreviation, stTZData.szAbbreviation, TZ_MAX_CHARS);

		size_t i;
		for(i=0; i<stTZData.vecType.size(); i++)
		{
			outInfo.vecType[i].nGMTOffset	= HostFromBigEndian(stTZData.vecType[i].nGMTOffset);
			outInfo.vecType[i].IsDST		= stTZData.vecType[i].IsDST			;
			outInfo.vecType[i].btAbbrind	= stTZData.vecType[i].btAbbrind		;
		}

		for(i=0; i<stTZData.vecIsGmt.size(); i++)
			outInfo.vecType[i].IsGMT		= stTZData.vecIsGmt[i]	;

		for(i=0; i<stTZData.vecIsStd.size(); i++)
			outInfo.vecType[i].IsSTD		= stTZData.vecIsStd[i]	;

		for(i=0; i<stTZData.vecTransitTime.size(); i++)
		{
			size_t tIndex = stTZData.vecTransitTimeIndex[i];
			if( tIndex >= outInfo.vecType.size() )
				return EC_INVALID_DATA;

			outInfo.mapTransitTime.insert(
				std::make_pair(
					HostFromBigEndian(stTZData.vecTransitTime[i]), 
					outInfo.vecType[tIndex]
				)
			);
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	void PrintOut(ST_TZ_INFO& info)
	{
		CTZTypeMapIt iter;
		for(iter=info.mapTransitTime.begin(); iter!=info.mapTransitTime.end(); iter++)
		{
			ST_SYSTEMTIME stTime = SystemTimeFrom(iter->first);
			printf("%5s %04u-%02u-%02u %02u:%02u:%02u offset:%d %s\n"
				, info.szAbbreviation + iter->second.btAbbrind
				, stTime.wYear, stTime.wMonth, stTime.wDay
				, stTime.wHour, stTime.wMinute, stTime.wSecond
				, iter->second.nGMTOffset / -3600
				, iter->second.IsDST? "DST" : "STD");
		}
	}
}
