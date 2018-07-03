#pragma once

#include "../__Common/Define.h"
#include "../__Common/Type.h"
#include "Struct.h"
#include <vector>

#ifdef UNICODE
#define GetUserName					GetUserNameW
#define GetComputerName				GetComputerNameW
#define GetErrorString				GetErrorStringW
#define GenerateGuid				GenerateGuidW
#else
#define GetUserName					GetUserNameA
#define GetComputerName				GetComputerNameA
#define GetErrorString				GetErrorStringA
#define GenerateGuid				GenerateGuidA
#endif

namespace core
{
	void			Sleep(DWORD dwMilliSec);

	ECODE			GetLastError(void);
	void			SetLastError(ECODE nCode);
	void			GetLocalTime(ST_SYSTEMTIME* pOutTime);
	bool			SetLocalTime(const ST_SYSTEMTIME* pInTime);
	void			GetSystemTime(ST_SYSTEMTIME* pOutTime);	// GMT time
	bool			SetSystemTime(const ST_SYSTEMTIME* pInTime);	// GMT time
	HANDLE			GetCurrentThread(void);
	HANDLE			GetCurrentProcess(void);
	DWORD			GetCurrentThreadId(void);
	DWORD			GetCurrentProcessId(void);
	DWORD			GetTickCount(void);						// A time value of milli-second.
	
	bool			operator==(const ST_SYSTEMTIME& lhs, const ST_SYSTEMTIME& rhs);
	bool			operator!=(const ST_SYSTEMTIME& lhs, const ST_SYSTEMTIME& rhs);	

	HANDLE			LoadLibrary(const char* pszPath);
	HANDLE			LoadLibrary(const wchar_t* pszPath);
	void*			GetProcAddress(HANDLE hModule, const char* pszProcName);
	bool			FreeLibrary(HANDLE hModule);

	ETIMEZONE		GetTimeZoneInformation(ST_TIME_ZONE_INFORMATIONA* pTimeZone);
	ETIMEZONE		GetTimeZoneInformation(ST_TIME_ZONE_INFORMATIONW* pTimeZone);
	ETIMEZONE		GetTimeZoneInformation(const ST_SYSTEMTIME& stGMT, ST_TIME_ZONE_INFORMATIONA* pTimeZone);
	ETIMEZONE		GetTimeZoneInformation(const ST_SYSTEMTIME& stGMT, ST_TIME_ZONE_INFORMATIONW* pTimeZone);
	ETIMEZONE		GetTimeZoneInformationByTZFile(ST_TIME_ZONE_INFORMATIONA* pOut, LPCSTR pszFilePath, ST_SYSTEMTIME stUTC);
	ETIMEZONE		GetTimeZoneInformationByTZFile(ST_TIME_ZONE_INFORMATIONW* pOut, LPCSTR pszFilePath, ST_SYSTEMTIME stUTC);

	std::string		GetErrorStringA(ECODE nErrCode);
	std::wstring	GetErrorStringW(ECODE nErrCode);
	std::string		GetUserNameA(void);
	std::wstring	GetUserNameW(void);
	std::string		GetComputerNameA(void);
	std::wstring	GetComputerNameW(void);
	std::string		GenerateGuidA(void);
	std::wstring	GenerateGuidW(void);

	#undef			OutputDebugString
	void			OutputDebugString(const char* pszFormat, ...);
	void			OutputDebugString(const wchar_t* pszFormat, ...);

	bool			IsBigEndian(void);
	bool			IsLittleEndian(void);
	WORD			NetworkFromHost(WORD value);
	short			NetworkFromHost(short value);
	DWORD			NetworkFromHost(DWORD value);
	int32_t			NetworkFromHost(int32_t value);
	QWORD			NetworkFromHost(QWORD value);
	int64_t			NetworkFromHost(int64_t value);
	WORD			HostFromNetwork(WORD value);
	short			HostFromNetwork(short value);
	DWORD			HostFromNetwork(DWORD value);
	int32_t			HostFromNetwork(int32_t value);
	QWORD			HostFromNetwork(QWORD value);
	int64_t			HostFromNetwork(int64_t value);

	WORD			BigEndianFromHost(WORD value);
	short			BigEndianFromHost(short value);
	DWORD			BigEndianFromHost(DWORD value);
	int32_t			BigEndianFromHost(int32_t value);
	QWORD			BigEndianFromHost(QWORD value);
	int64_t			BigEndianFromHost(int64_t value);
	WORD			HostFromBigEndian(WORD value);
	short			HostFromBigEndian(short value);
	DWORD			HostFromBigEndian(DWORD value);
	int32_t			HostFromBigEndian(int32_t value);
	QWORD			HostFromBigEndian(QWORD value);
	int64_t			HostFromBigEndian(int64_t value);

	WORD			LittleEndianFromHost(WORD value);
	short			LittleEndianFromHost(short value);
	DWORD			LittleEndianFromHost(DWORD value);
	int32_t			LittleEndianFromHost(int32_t value);
	QWORD			LittleEndianFromHost(QWORD value);
	int64_t			LittleEndianFromHost(int64_t value);
	WORD			HostFromLittleEndian(WORD value);
	short			HostFromLittleEndian(short value);
	DWORD			HostFromLittleEndian(DWORD value);
	int32_t			HostFromLittleEndian(int32_t value);
	QWORD			HostFromLittleEndian(QWORD value);
	int64_t			HostFromLittleEndian(int64_t value);

	WORD&			MakeHostFromBE(WORD& value);
	short&			MakeHostFromBE(short& value);
	DWORD&			MakeHostFromBE(DWORD& value);
	int32_t&		MakeHostFromBE(int32_t& value);
	QWORD&			MakeHostFromBE(QWORD& value);
	int64_t&		MakeHostFromBE(int64_t& value);
	WORD&			MakeBEFromHost(WORD& value);
	short&			MakeBEFromHost(short& value);
	DWORD&			MakeBEFromHost(DWORD& value);
	int32_t&		MakeBEFromHost(int32_t& value);
	QWORD&			MakeBEFromHost(QWORD& value);
	int64_t&		MakeBEFromHost(int64_t& value);

	WORD&			MakeHostFromLE(WORD& value);
	short&			MakeHostFromLE(short& value);
	DWORD&			MakeHostFromLE(DWORD& value);
	int32_t&		MakeHostFromLE(int32_t& value);
	QWORD&			MakeHostFromLE(QWORD& value);
	int64_t&		MakeHostFromLE(int64_t& value);
	WORD&			MakeLEFromHost(WORD& value);
	short&			MakeLEFromHost(short& value);
	DWORD&			MakeLEFromHost(DWORD& value);
	int32_t&		MakeLEFromHost(int32_t& value);
	QWORD&			MakeLEFromHost(QWORD& value);
	int64_t&		MakeLEFromHost(int64_t& value);

	WORD			CalcDayOfWeek(WORD wYear, WORD wMonth, WORD wDay);
	UINT64			UnixTimeFrom(ST_SYSTEMTIME stTime);
	ST_SYSTEMTIME	SystemTimeFrom(UINT64 tUnixTime);

	bool			MakeAbsoluteTZTime(WORD wYear, const ST_SYSTEMTIME& stTZTime, ST_SYSTEMTIME& outAbsTime);
}
