#pragma once

#include "../__Common/Define.h"
#include "../__Common/Type.h"
#include "../__Common/ErrorCode.h"
#include "Struct.h"
#include <vector>

#ifdef UNICODE
#define LoadLibrary							LoadLibraryW
#define GetTimeZoneInformation				GetTimeZoneInformationW
#define GetTimeZoneInformationByTZFile		GetTimeZoneInformationByTZFileW
#define GetErrorString						GetErrorStringW
#define GetUserName							GetUserNameW
#define GetComputerName						GetComputerNameW
#define GenerateGuid						GenerateGuidW
#define GetFileVersionInfo					GetFileVersionInfoW
#define GetProductVersionInfo				GetProductVersionInfoW
#define OutputDebugString					OutputDebugStringW
#else
#define LoadLibrary							LoadLibraryA
#define GetTimeZoneInformation				GetTimeZoneInformationA
#define GetTimeZoneInformationByTZFile		GetTimeZoneInformationByTZFileA
#define GetErrorString						GetErrorStringA
#define GetUserName							GetUserNameA
#define GetComputerName						GetComputerNameA
#define GenerateGuid						GenerateGuidA
#define GetFileVersionInfo					GetFileVersionInfoA
#define GetProductVersionInfo				GetProductVersionInfoA
#define OutputDebugString					OutputDebugStringA
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
	ECODE			GetSystemInfo(ST_SYSTEMINFO* pSystemInfo);

	ECODE			QueryPerformanceCounter(QWORD* pOutValue);
	ECODE			QueryPerformanceFrequency(QWORD* pOutValue);
	
	//$ cat /etc/issue.net 
	//Ubuntu 16.04.1 LTS

	//$ cat /etc/issue.net 
	//CentOS release 6.8 (Final)
	//Kernel \r on an \m_

	bool			operator==(const ST_SYSTEMTIME& lhs, const ST_SYSTEMTIME& rhs);
	bool			operator!=(const ST_SYSTEMTIME& lhs, const ST_SYSTEMTIME& rhs);	
	E_OS_TYPE		GetOSType(void);

	typedef void	(*FP_TerminateSignalCallback)(void);
	ECODE			RegisterTerminateSignalCallback(FP_TerminateSignalCallback fpCallback);

	HANDLE			LoadLibraryA(const char* pszPath);
	HANDLE			LoadLibraryW(const wchar_t* pszPath);
	void*			GetProcAddress(HANDLE hModule, const char* pszProcName);
	bool			FreeLibrary(HANDLE hModule);
	
	ETIMEZONE		GetTimeZoneInformationA(ST_TIME_ZONE_INFORMATIONA* pTimeZone);
	ETIMEZONE		GetTimeZoneInformationW(ST_TIME_ZONE_INFORMATIONW* pTimeZone);
	ETIMEZONE		GetTimeZoneInformationA(const ST_SYSTEMTIME& stGMT, ST_TIME_ZONE_INFORMATIONA* pTimeZone);
	ETIMEZONE		GetTimeZoneInformationW(const ST_SYSTEMTIME& stGMT, ST_TIME_ZONE_INFORMATIONW* pTimeZone);
	ETIMEZONE		GetTimeZoneInformationByTZFileA(ST_TIME_ZONE_INFORMATIONA* pOut, LPCSTR pszFilePath, ST_SYSTEMTIME stUTC);
	ETIMEZONE		GetTimeZoneInformationByTZFileW(ST_TIME_ZONE_INFORMATIONW* pOut, LPCWSTR pszFilePath, ST_SYSTEMTIME stUTC);

	std::string		GetErrorStringA(ECODE nErrCode);
	std::wstring	GetErrorStringW(ECODE nErrCode);
	std::string		GetUserNameA(void);
	std::wstring	GetUserNameW(void);
	std::string		GetComputerNameA(void);
	std::wstring	GetComputerNameW(void);
	std::string		GenerateGuidA(void);
	std::wstring	GenerateGuidW(void);

	ECODE			GetFileVersionInfoA(const char* pszFilePath, ST_VERSIONINFO* pVersionInfo);
	ECODE			GetFileVersionInfoW(const wchar_t* pszFilePath, ST_VERSIONINFO* pVersionInfo);
	ECODE			GetProductVersionInfoA(const char* pszFilePath, ST_VERSIONINFO* pVersionInfo);
	ECODE			GetProductVersionInfoW(const wchar_t* pszFilePath, ST_VERSIONINFO* pVersionInfo);

	void			OutputDebugStringA(const char* pszFormat, ...);
	void			OutputDebugStringW(const wchar_t* pszFormat, ...);

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

	UINT64			UnixTimeFrom_NativeAPI(ST_SYSTEMTIME stTime);
	ST_SYSTEMTIME	SystemTimeFrom_NativeAPI(UINT64 tUnixTime);

	bool			MakeAbsoluteTZTime(WORD wYear, const ST_SYSTEMTIME& stTZTime, ST_SYSTEMTIME& outAbsTime);

	bool			IpFromDomain(std::string strDomain, WORD wPort, std::string& outIPAddr);
	bool			IpFromDomain(std::wstring strDomain, WORD wPort, std::string& outIPAddr);
}
