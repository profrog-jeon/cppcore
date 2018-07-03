#pragma once

#include "../__Common/Define.h"
#include "../__Common/Type.h"
#include "../100_System/Log.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	class CLog
	{
	private:
		CLog(void)	{}
		~CLog(void)	{}

	public:
		void	Init(const char* pszFilePath);
		void	Init(const wchar_t* pszFilePath);
		void	Init(const ST_LOG_INIT_PARAMA& stParam);
		void	Init(const ST_LOG_INIT_PARAMW& stParam);
		void	Debug(const char* pszFormat, ...);
		void	Debug(const wchar_t* pszFormat, ...);
		void	Info(const char* pszFormat, ...);
		void	Info(const wchar_t* pszFormat, ...);
		void	Warn(const char* pszFormat, ...);
		void	Warn(const wchar_t* pszFormat, ...);
		void	Error(const char* pszFormat, ...);
		void	Error(const wchar_t* pszFormat, ...);

	public:
		static CLog* GetInstance(void)
		{
			static CLog instance;
			return &instance;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// Log singleton
	inline CLog* Log()
	{
		return CLog::GetInstance();
	}
}
