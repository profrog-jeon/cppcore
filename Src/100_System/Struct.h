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
		size_t tPID;
		std::string strName;
	};

	struct ST_PROCESS_INFOW
	{
		size_t tPID;
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
	typedef ST_TIME_ZONE_INFORMATIONW	ST_TIME_ZONE_INFORMATION	;
	typedef ST_FILETRAVERSE_INFOW		ST_FILETRAVERSE_INFO		;
	typedef ST_PROCESS_INFOW			ST_PROCESS_INFO				;
	typedef ST_FILE_FINDDATAW			ST_FILE_FINDDATA			;
#else
	typedef ST_TIME_ZONE_INFORMATIONA	ST_TIME_ZONE_INFORMATION	;
	typedef ST_FILETRAVERSE_INFOA		ST_FILETRAVERSE_INFO		;
	typedef ST_PROCESS_INFOA			ST_PROCESS_INFO				;
	typedef ST_FILE_FINDDATAA			ST_FILE_FINDDATA			;
#endif
}
