#include "stdafx.h"
#include "Environment.h"
#include "Log.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	static const WORD g_tDatePerMonth[2][14] = 
	{
		{
			/*	0	*/	0,
			/*	1	*/	0,
			/*	2	*/	31,
			/*	3	*/	31 + 28,
			/*	4	*/	31 + 28 + 31,
			/*	5	*/	31 + 28 + 31 + 30,
			/*	6	*/	31 + 28 + 31 + 30 + 31,
			/*	7	*/	31 + 28 + 31 + 30 + 31 + 30,
			/*	8	*/	31 + 28 + 31 + 30 + 31 + 30 + 31,
			/*	9	*/	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31,
			/*	10	*/	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
			/*	11	*/	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
			/*	12	*/	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,
			/*	13	*/	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31,
		},
		{	// Leap year
			/*	0	*/	0,
			/*	1	*/	0,
			/*	2	*/	31,
			/*	3	*/	31 + 29,
			/*	4	*/	31 + 29 + 31,
			/*	5	*/	31 + 29 + 31 + 30,
			/*	6	*/	31 + 29 + 31 + 30 + 31,
			/*	7	*/	31 + 29 + 31 + 30 + 31 + 30,
			/*	8	*/	31 + 29 + 31 + 30 + 31 + 30 + 31,
			/*	9	*/	31 + 29 + 31 + 30 + 31 + 30 + 31 + 31,
			/*	10	*/	31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
			/*	11	*/	31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
			/*	12	*/	31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,
			/*	13	*/	31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31,
		}
	};

	//////////////////////////////////////////////////////////////////////////
	static inline BYTE IsLeapYear(size_t tYear)
	{
		if( tYear % 4 )
			return 0;

		if( tYear % 100 )
			return 1;

		if( tYear % 400 )
			return 0;

		return 1;
	}

	//////////////////////////////////////////////////////////////////////////
	WORD CalcDayOfWeek(WORD wYear, WORD wMonth, WORD wDay)
	{
		UINT64 tNumOfDay = 0;
		size_t tLeapYear = IsLeapYear(wYear);

		// count day of year
		tNumOfDay += wYear * 365;

		// count day of month
		tNumOfDay += g_tDatePerMonth[tLeapYear][wMonth];

		// count day of day
		tNumOfDay += wDay - 1;

		// count leap day
		tNumOfDay += (wYear / 4) - (wYear / 100) + (wYear / 400) - tLeapYear;

		// calc day of week
		return tNumOfDay % 7;
	}

	//////////////////////////////////////////////////////////////////////////
	UINT64 UnixTimeFrom(ST_SYSTEMTIME stTime)
	{
		// exception for invalid date
		if( stTime.wYear < 1970)
			return 0;
		if( stTime.wMonth > 12 )
			return 0;
		if( 0 == stTime.wDay )
			return 0;

		BYTE btIsLeapYear = IsLeapYear(stTime.wYear);

		UINT64 tUnixTime = 0;
		tUnixTime += (stTime.wYear - 1970) * 365;	// count day of year
		tUnixTime += g_tDatePerMonth[btIsLeapYear][stTime.wMonth];	// count day of month
		tUnixTime += stTime.wDay - 1;		// count day of day

		tUnixTime += (stTime.wYear / 4) - (stTime.wYear / 100) + (stTime.wYear / 400) - btIsLeapYear;	// count leap day
		tUnixTime -= (1970 / 4) - (1970 / 100) + (1970 / 400);
		tUnixTime = tUnixTime * 24 + stTime.wHour;		// day -> hr
		tUnixTime = tUnixTime * 60 + stTime.wMinute;	// hr -> min
		tUnixTime = tUnixTime * 60 + stTime.wSecond;	// min -> sec
		return tUnixTime;
	}

	//////////////////////////////////////////////////////////////////////////
	ST_SYSTEMTIME SystemTimeFrom(UINT64 tUnixTime)
	{
		ST_SYSTEMTIME stSystemTime = { 0, };
		stSystemTime.wSecond = tUnixTime % 60;	// count sec
		tUnixTime /= 60;	// sec -> min
		stSystemTime.wMinute = tUnixTime % 60;	// count min
		tUnixTime /= 60;	// min -> hr
		stSystemTime.wHour = tUnixTime % 24;	// count hr
		tUnixTime /= 24;	// hr -> day
		stSystemTime.wDayOfWeek = (tUnixTime + 4) % 7;	// calc day of week

		// calc year
		size_t tCumulatedDate = 0;
		size_t tLeapYear = 0;
		size_t i;
		for(i=1970; tCumulatedDate<=tUnixTime; i++)
		{
			tLeapYear = IsLeapYear(i);
			tCumulatedDate += 365 + tLeapYear;
		}

		size_t tRemainedDate = (size_t)tUnixTime - (tCumulatedDate - 365 - tLeapYear);
		stSystemTime.wYear	= (WORD)(i - 1);

		// calc month
		for(i=1; i<=12; i++)
		{
			if( tRemainedDate >= g_tDatePerMonth[tLeapYear][i+1] )
				continue;

			stSystemTime.wMonth = (WORD)i;
			stSystemTime.wDay   = (WORD)(tRemainedDate - g_tDatePerMonth[tLeapYear][i] + 1);
			break;
		}

		return stSystemTime;
	}

	//////////////////////////////////////////////////////////////////////////
	bool operator!=(const ST_SYSTEMTIME& lhs, const ST_SYSTEMTIME& rhs)
	{
		if( lhs.wYear			!= rhs.wYear		||
			lhs.wMonth			!= rhs.wMonth		||
			lhs.wDay			!= rhs.wDay			||
			lhs.wHour			!= rhs.wHour		||
			lhs.wDayOfWeek		!= rhs.wDayOfWeek	||
			lhs.wMinute			!= rhs.wMinute		||
			lhs.wSecond			!= rhs.wSecond		||
			lhs.wMilliseconds	!= rhs.wMilliseconds	)
			return true;
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool operator==(const ST_SYSTEMTIME& lhs, const ST_SYSTEMTIME& rhs)
	{
		if( lhs.wYear			!= rhs.wYear			)	return false;
		if( lhs.wMonth			!= rhs.wMonth			)	return false;
		if( lhs.wDay			!= rhs.wDay				)	return false;
		if( lhs.wHour			!= rhs.wHour			)	return false;
		if( lhs.wDayOfWeek		!= rhs.wDayOfWeek		)	return false;
		if( lhs.wMinute			!= rhs.wMinute			)	return false;
		if( lhs.wSecond			!= rhs.wSecond			)	return false;
		if( lhs.wMilliseconds	!= rhs.wMilliseconds	)	return false;
		return true;
	}	

	//////////////////////////////////////////////////////////////////////////
	static const union
	{
		DWORD dwData;
		BYTE btData[4];
	}	EndianChecker = { 1 };

	//////////////////////////////////////////////////////////////////////////	
	bool IsBigEndian(void)
	{
		return EndianChecker.btData[3] == 1;
	}

	//////////////////////////////////////////////////////////////////////////	
	bool IsLittleEndian(void)
	{
		return EndianChecker.btData[0] == 1;
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline T& InverseByteOrder2(T& value)
	{
		T ret = ((value & 0xFF00) >> 8) | ((value & 0x00FF) << 8);
		return (value = ret);
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline T& InverseByteOrder4(T& value)
	{
		T ret = 
			((value & 0xFF000000) >> 24) | 
			((value & 0x00FF0000) >>  8) | 
			((value & 0x0000FF00) <<  8) | 
			((value & 0x000000FF) << 24);
		return (value = ret);
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline T& InverseByteOrder8(T& value)
	{
		T ret = 
			((value & 0xFF00000000000000ULL) >> 56) | 
			((value & 0x00FF000000000000ULL) >> 40) | 
			((value & 0x0000FF0000000000ULL) >> 24) | 
			((value & 0x000000FF00000000ULL) >>  8) | 
			((value & 0x00000000FF000000ULL) <<  8) | 
			((value & 0x0000000000FF0000ULL) << 24) | 
			((value & 0x000000000000FF00ULL) << 40) | 
			((value & 0x00000000000000FFULL) << 56);
		return (value = ret);
	}

	//////////////////////////////////////////////////////////////////////////
	WORD    NetworkFromHost(WORD value)		{	return IsBigEndian()? value :InverseByteOrder2(value);	}
	short   NetworkFromHost(short value)	{	return IsBigEndian()? value :InverseByteOrder2(value);	}
	DWORD   NetworkFromHost(DWORD value)	{	return IsBigEndian()? value :InverseByteOrder4(value);	}
	int32_t NetworkFromHost(int32_t value)	{	return IsBigEndian()? value :InverseByteOrder4(value);	}
	QWORD   NetworkFromHost(QWORD value)	{	return IsBigEndian()? value :InverseByteOrder8(value);	}
	int64_t	NetworkFromHost(int64_t value)	{	return IsBigEndian()? value :InverseByteOrder8(value);	}

	//////////////////////////////////////////////////////////////////////////
	WORD	HostFromNetwork(WORD value)		{	return IsBigEndian()? value :InverseByteOrder2(value);	}
	short	HostFromNetwork(short value)	{	return IsBigEndian()? value :InverseByteOrder2(value);	}
	DWORD	HostFromNetwork(DWORD value)	{	return IsBigEndian()? value :InverseByteOrder4(value);	}
	int32_t	HostFromNetwork(int32_t value)	{	return IsBigEndian()? value :InverseByteOrder4(value);	}
	QWORD	HostFromNetwork(QWORD value)	{	return IsBigEndian()? value :InverseByteOrder8(value);	}
	int64_t	HostFromNetwork(int64_t value)	{	return IsBigEndian()? value :InverseByteOrder8(value);	}

	//////////////////////////////////////////////////////////////////////////
	WORD    BigEndianFromHost(WORD value)		{	return IsBigEndian()? value :InverseByteOrder2(value);	}
	short   BigEndianFromHost(short value)		{	return IsBigEndian()? value :InverseByteOrder2(value);	}
	DWORD   BigEndianFromHost(DWORD value)		{	return IsBigEndian()? value :InverseByteOrder4(value);	}
	int32_t BigEndianFromHost(int32_t value)	{	return IsBigEndian()? value :InverseByteOrder4(value);	}
	QWORD   BigEndianFromHost(QWORD value)		{	return IsBigEndian()? value :InverseByteOrder8(value);	}
	int64_t	BigEndianFromHost(int64_t value)	{	return IsBigEndian()? value :InverseByteOrder8(value);	}

	//////////////////////////////////////////////////////////////////////////
	WORD	HostFromBigEndian(WORD value)		{	return IsBigEndian()? value :InverseByteOrder2(value);	}
	short	HostFromBigEndian(short value)		{	return IsBigEndian()? value :InverseByteOrder2(value);	}
	DWORD	HostFromBigEndian(DWORD value)		{	return IsBigEndian()? value :InverseByteOrder4(value);	}
	int32_t	HostFromBigEndian(int32_t value)	{	return IsBigEndian()? value :InverseByteOrder4(value);	}
	QWORD	HostFromBigEndian(QWORD value)		{	return IsBigEndian()? value :InverseByteOrder8(value);	}
	int64_t	HostFromBigEndian(int64_t value)	{	return IsBigEndian()? value :InverseByteOrder8(value);	}

	//////////////////////////////////////////////////////////////////////////
	WORD    LittleEndianFromHost(WORD value)	{	return IsLittleEndian()? value :InverseByteOrder2(value);	}
	short   LittleEndianFromHost(short value)	{	return IsLittleEndian()? value :InverseByteOrder2(value);	}
	DWORD   LittleEndianFromHost(DWORD value)	{	return IsLittleEndian()? value :InverseByteOrder4(value);	}
	int32_t LittleEndianFromHost(int32_t value)	{	return IsLittleEndian()? value :InverseByteOrder4(value);	}
	QWORD   LittleEndianFromHost(QWORD value)	{	return IsLittleEndian()? value :InverseByteOrder8(value);	}
	int64_t	LittleEndianFromHost(int64_t value)	{	return IsLittleEndian()? value :InverseByteOrder8(value);	}

	//////////////////////////////////////////////////////////////////////////
	WORD	HostFromLittleEndian(WORD value)	{	return IsLittleEndian()? value :InverseByteOrder2(value);	}
	short	HostFromLittleEndian(short value)	{	return IsLittleEndian()? value :InverseByteOrder2(value);	}
	DWORD	HostFromLittleEndian(DWORD value)	{	return IsLittleEndian()? value :InverseByteOrder4(value);	}
	int32_t	HostFromLittleEndian(int32_t value)	{	return IsLittleEndian()? value :InverseByteOrder4(value);	}
	QWORD	HostFromLittleEndian(QWORD value)	{	return IsLittleEndian()? value :InverseByteOrder8(value);	}
	int64_t	HostFromLittleEndian(int64_t value)	{	return IsLittleEndian()? value :InverseByteOrder8(value);	}

	//////////////////////////////////////////////////////////////////////////
	WORD&	MakeHostFromBE(WORD& value)			{	return IsBigEndian()? value :InverseByteOrder2(value);	}
	short&	MakeHostFromBE(short& value)		{	return IsBigEndian()? value :InverseByteOrder2(value);	}
	DWORD&	MakeHostFromBE(DWORD& value)		{	return IsBigEndian()? value :InverseByteOrder4(value);	}
	int32_t& MakeHostFromBE(int32_t& value)		{	return IsBigEndian()? value :InverseByteOrder4(value);	}
	QWORD&	MakeHostFromBE(QWORD& value)		{	return IsBigEndian()? value :InverseByteOrder8(value);	}
	int64_t& MakeHostFromBE(int64_t& value)		{	return IsBigEndian()? value :InverseByteOrder8(value);	}

	//////////////////////////////////////////////////////////////////////////
	WORD&	MakeHostFromLE(WORD& value)			{	return IsLittleEndian()? value :InverseByteOrder2(value);	}
	short&	MakeHostFromLE(short& value)		{	return IsLittleEndian()? value :InverseByteOrder2(value);	}
	DWORD&	MakeHostFromLE(DWORD& value)		{	return IsLittleEndian()? value :InverseByteOrder4(value);	}
	int32_t& MakeHostFromLE(int32_t& value)		{	return IsLittleEndian()? value :InverseByteOrder4(value);	}
	QWORD&	MakeHostFromLE(QWORD& value)		{	return IsLittleEndian()? value :InverseByteOrder8(value);	}
	int64_t& MakeHostFromLE(int64_t& value)		{	return IsLittleEndian()? value :InverseByteOrder8(value);	}

	//////////////////////////////////////////////////////////////////////////
	WORD&	MakeBEFromHost(WORD& value)			{	return IsBigEndian()? value :InverseByteOrder2(value);	}
	short&	MakeBEFromHost(short& value)		{	return IsBigEndian()? value :InverseByteOrder2(value);	}
	DWORD&	MakeBEFromHost(DWORD& value)		{	return IsBigEndian()? value :InverseByteOrder4(value);	}
	int32_t& MakeBEFromHost(int32_t& value)		{	return IsBigEndian()? value :InverseByteOrder4(value);	}
	QWORD&	MakeBEFromHost(QWORD& value)		{	return IsBigEndian()? value :InverseByteOrder8(value);	}
	int64_t& MakeBEFromHost(int64_t& value)		{	return IsBigEndian()? value :InverseByteOrder8(value);	}

	//////////////////////////////////////////////////////////////////////////
	WORD&	MakeLEFromHost(WORD& value)			{	return IsLittleEndian()? value :InverseByteOrder2(value);	}
	short&	MakeLEFromHost(short& value)		{	return IsLittleEndian()? value :InverseByteOrder2(value);	}
	DWORD&	MakeLEFromHost(DWORD& value)		{	return IsLittleEndian()? value :InverseByteOrder4(value);	}
	int32_t& MakeLEFromHost(int32_t& value)		{	return IsLittleEndian()? value :InverseByteOrder4(value);	}
	QWORD&	MakeLEFromHost(QWORD& value)		{	return IsLittleEndian()? value :InverseByteOrder8(value);	}
	int64_t& MakeLEFromHost(int64_t& value)		{	return IsLittleEndian()? value :InverseByteOrder8(value);	}

	//////////////////////////////////////////////////////////////////////////
	bool MakeAbsoluteTZTime(WORD wYear, const ST_SYSTEMTIME& stTZTime, ST_SYSTEMTIME& outAbsTime)
	{
		// except for already absolute date
		if( 0 != stTZTime.wYear )
			return false;

		// except for invalid date
		if( 0 == stTZTime.wMonth )
			return false;

		const size_t tLeapYear = IsLeapYear(wYear);
		const WORD wNumOfDay = g_tDatePerMonth[tLeapYear][stTZTime.wMonth+1] - g_tDatePerMonth[tLeapYear][stTZTime.wMonth];
		const WORD wDayOfWeek = stTZTime.wDayOfWeek;
		const WORD wNumOfWeek = stTZTime.wDay;

		WORD wFirstDayOfWeek = CalcDayOfWeek(wYear, stTZTime.wMonth, 1);

		// assign default value
		outAbsTime = stTZTime;
		outAbsTime.wYear = wYear;

		// except for non-existing date of 1st weekday.
		if( wFirstDayOfWeek <= stTZTime.wDayOfWeek )
			outAbsTime.wDay = 7 * (wNumOfWeek - 1) + wDayOfWeek - wFirstDayOfWeek + 1;
		else
			outAbsTime.wDay = 7 * wNumOfWeek + wDayOfWeek - wFirstDayOfWeek + 1;

		// except for non-existing date of 5th week.
		if( outAbsTime.wDay > wNumOfDay )
			outAbsTime.wDay -= 7;

		return true;
	}
}