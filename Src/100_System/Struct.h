#pragma once

#include <vector>

#include "../__Common/Define.h"
#include "../__Common/Type.h"

namespace core
{
	enum E_OS_TYPE
	{
		OS_TYPE_UNDEFINED				= 0,
		OS_TYPE_WINDOWS_2000			= 0x1000,
		OS_TYPE_WINDOWS_XP				,
		OS_TYPE_WINDOWS_SERVER_2003		,
		OS_TYPE_WINDOWS_SERVER_2003_R2	,
		OS_TYPE_WINDOWS_SERVER_2008		,
		OS_TYPE_WINDOWS_SERVER_2008_R2	,
		OS_TYPE_WINDOWS_VISTA			,
		OS_TYPE_WINDOWS_7				,
		OS_TYPE_WINDOWS_SERVER_2012		,
		OS_TYPE_WINDOWS_SERVER_2012_R2	,
		OS_TYPE_WINDOWS_8				,
		OS_TYPE_WINDOWS_8_1				,
		OS_TYPE_WINDOWS_SERVER_2016		,
		OS_TYPE_WINDOWS_10				,
		OS_TYPE_LINUX_X86				= 0x2000,
		OS_TYPE_LINUX_X64				,
	};

	enum E_THREAD_PRIORITY
	{
		THREAD_PRIORITY_ERROR		= -100,
		THREAD_PRIORITY_BELOW_LOW	= -2,
		THREAD_PRIORITY_LOW			= -1,
		THREAD_PRIORITY_MIDDLE		= 0,
		THREAD_PRIORITY_HIGH		= 1,
		THREAD_PRIORITY_ABOVE_HIGH	= 2,
	};

	enum ETIMEZONE
	{
		TIME_ZONE_ID_INVALID_		= 0xFFFFFFFF,
		TIME_ZONE_ID_UNKNOWN_		= 0,	// Daylight saving time is not used in the current time zone
		TIME_ZONE_ID_STANDARD_		= 1,	// The system is operating in the range covered by the StandardDate member
		TIME_ZONE_ID_DAYLIGHT_		= 2,	// The system is operating in the range covered by the DaylightDate member
	};

	struct ST_SYSTEMTIME
	{
		WORD wYear			;
		WORD wMonth			;
		WORD wDayOfWeek		;
		WORD wDay			;
		WORD wHour			;
		WORD wMinute		;
		WORD wSecond		;
		WORD wMilliseconds	;
	};

	struct ST_TIME_ZONE_INFORMATIONA
	{
		int			Bias			;
		char		StandardName[32];
		ST_SYSTEMTIME StandardDate	;
		int			StandardBias	;
		char		DaylightName[32];
		ST_SYSTEMTIME DaylightDate	;
		int			DaylightBias	;
	};

	struct ST_TIME_ZONE_INFORMATIONW
	{
		int			Bias			;
		wchar_t		StandardName[32];
		ST_SYSTEMTIME StandardDate	;
		int			StandardBias	;
		wchar_t		DaylightName[32];
		ST_SYSTEMTIME DaylightDate	;
		int			DaylightBias	;
	};

	struct ST_SYSTEMINFO
	{
		DWORD		dwPageSize					;
		LPVOID		lpMinimumApplicationAddress	;
		LPVOID		lpMaximumApplicationAddress	;
		DWORD		dwActiveProcessorMask		;
		DWORD		dwNumberOfProcessors		;
		DWORD		dwProcessorType				;
		DWORD		dwAllocationGranularity		;
		WORD		wProcessorLevel				;
		WORD		wProcessorRevision			;
	};

	struct ST_VERSIONINFO
	{
		DWORD		dwMajor						;
		DWORD		dwMinor						;
		DWORD		dwPatch						;
		DWORD		dwBuild						;
	};

	struct ST_ROUTEINFOA
	{
		std::string strIFName;
		DWORD dwDest;
		DWORD dwSrc;
		DWORD dwGateway;
		DWORD dwGenmask;
	};

	struct ST_ROUTEINFOW
	{
		std::wstring strIFName;
		DWORD dwDest;
		DWORD dwSrc;
		DWORD dwGateway;
		DWORD dwGenmask;
	};

	struct ST_ETHERNETINFOA
	{
		std::string strName;
		std::string strMAC;
		DWORD dwIndex;
		DWORD dwIPAddress;
		DWORD dwIPMask;
		DWORD dwReserved;
	};	

	struct ST_NETWORKINFOA
	{
		std::vector<ST_ETHERNETINFOA> vecEthernet;
		std::vector<ST_ROUTEINFOA> vecRoute;
		std::vector<std::string> vecDNS;
	};

	struct ST_ETHERNETINFOW
	{
		std::wstring strName;
		std::wstring strMAC;
		DWORD dwIndex;
		DWORD dwIPAddress;
		DWORD dwIPMask;
		DWORD dwReserved;
	};

	struct ST_NETWORKINFOW
	{
		std::vector<ST_ETHERNETINFOW> vecEthernet;
		std::vector<ST_ROUTEINFOW> vecRoute;
		std::vector<std::wstring> vecDNS;
	};

	struct ST_ETH_NETWORKA
	{
		std::string strEthName	;
		std::string strIFName	;
		std::string strIP		;
		std::string strIPMask	;
		std::string strGateway	;
		std::string strDNS0		;
		std::string strDNS1		;
	};

	struct ST_ETH_NETWORKW
	{
		std::wstring strEthName	;
		std::wstring strIFName	;
		std::wstring strIP		;
		std::wstring strIPMask	;
		std::wstring strGateway	;
		std::wstring strDNS0	;
		std::wstring strDNS1	;
	};

	struct ST_BY_HANDLE_FILE_INFORMATION
	{
		DWORD  dwFileAttributes		;
		UINT64 tCreationTime		;
		UINT64 tLastAccessTime		;
		UINT64 tLastWriteTime		;
		DWORD  dwVolumeSerialNumber	;
		DWORD  nFileSizeHigh		;
		DWORD  nFileSizeLow			;
		DWORD  nNumberOfLinks		;
		DWORD  nFileIndexHigh		;
		DWORD  nFileIndexLow		;
	};

	struct ST_FILETRAVERSE_INFOA
	{
		bool bDirectory;
		bool bReserved[7];
		const char* pszFilePath;
	};

	struct ST_FILETRAVERSE_INFOW
	{
		bool bDirectory;
		bool bReserved[7];
		const wchar_t* pszFilePath;
	};

	struct ST_STARTUPINFO
	{
		HANDLE  hStdInput;
		HANDLE  hStdOutput;
		HANDLE  hStdError;
	};

	struct ST_PROCESSINFO
	{
		HANDLE hProcess;
		DWORD dwProcessId;
		DWORD dwThreadId;
	};

	struct ST_PROCESS_INFOA
	{
		DWORD dwPID;
		std::string strName;
	};

	struct ST_PROCESS_INFOW
	{
		DWORD dwPID;
		std::wstring strName;
	};

	struct ST_FILE_FINDDATAA
	{
		bool bIsDirectory;
		bool bIsFile;
		bool bIsLink;
		bool bReserved[5];
		std::string strFileName;
		UINT64 uFileSize;
		UINT64 uCreationTime;
		UINT64 uLastAccessTime;
		UINT64 uLastWriteTime;
	};
	struct ST_FILE_FINDDATAW
	{
		bool bIsDirectory;
		bool bIsFile;
		bool bIsLink;
		bool bReserved[5];
		std::wstring strFileName;
		UINT64 uFileSize;
		UINT64 uCreationTime;
		UINT64 uLastAccessTime;
		UINT64 uLastWriteTime;
	};

#ifdef UNICODE
	typedef ST_ROUTEINFOW				ST_ROUTEINFO				;
	typedef ST_NETWORKINFOW				ST_NETWORKINFO				;
	typedef ST_ETHERNETINFOW			ST_ETHERNETINFO				;
	typedef ST_ETH_NETWORKW				ST_ETH_NETWORK				;
	typedef ST_TIME_ZONE_INFORMATIONW	ST_TIME_ZONE_INFORMATION	;
	typedef ST_FILETRAVERSE_INFOW		ST_FILETRAVERSE_INFO		;
	typedef ST_PROCESS_INFOW			ST_PROCESS_INFO				;
	typedef ST_FILE_FINDDATAW			ST_FILE_FINDDATA			;
#else
	typedef ST_ROUTEINFOA				ST_ROUTEINFO				;
	typedef ST_NETWORKINFOA				ST_NETWORKINFO				;
	typedef ST_ETHERNETINFOA			ST_ETHERNETINFO				;
	typedef ST_ETH_NETWORKA				ST_ETH_NETWORK				;
	typedef ST_TIME_ZONE_INFORMATIONA	ST_TIME_ZONE_INFORMATION	;
	typedef ST_FILETRAVERSE_INFOA		ST_FILETRAVERSE_INFO		;
	typedef ST_PROCESS_INFOA			ST_PROCESS_INFO				;
	typedef ST_FILE_FINDDATAA			ST_FILE_FINDDATA			;
#endif
}
