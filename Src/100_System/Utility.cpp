#include "stdafx.h"
#include "Utility.h"
#include "FileSystem.h"

namespace core
{
	bool operator<(const ST_VERSIONINFO& lhs, const ST_VERSIONINFO& rhs)
	{
		if (lhs.dwMajor < rhs.dwMajor)		return true;
		if (rhs.dwMajor < lhs.dwMajor)		return false;

		if (lhs.dwMinor < rhs.dwMinor)		return true;
		if (rhs.dwMinor < lhs.dwMinor)		return false;

		if (lhs.dwPatch < rhs.dwPatch)		return true;
		if (rhs.dwPatch < lhs.dwPatch)		return false;

		if (lhs.dwBuild < rhs.dwBuild)		return true;
		if (rhs.dwBuild < lhs.dwBuild)		return false;

		return false;	// same
	}

	bool operator<(const core::ST_SYSTEMTIME& lhs, const core::ST_SYSTEMTIME& rhs)
	{
		if (lhs.wYear < rhs.wYear)						return true;
		if (rhs.wYear < lhs.wYear)						return false;

		if (lhs.wMonth < rhs.wMonth)					return true;
		if (rhs.wMonth < lhs.wMonth)					return false;

		if (lhs.wDay < rhs.wDay)						return true;
		if (rhs.wDay < lhs.wDay)						return false;

		if (lhs.wHour < rhs.wHour)						return true;
		if (rhs.wHour < lhs.wHour)						return false;

		if (lhs.wMinute < rhs.wMinute)					return true;
		if (rhs.wMinute < lhs.wMinute)					return false;

		if (lhs.wSecond < rhs.wSecond)					return true;
		if (rhs.wSecond < lhs.wSecond)					return false;

		if (lhs.wMilliseconds < rhs.wMilliseconds)		return true;
		if (rhs.wMilliseconds < lhs.wMilliseconds)		return false;

		return false;
	}

	CCurrentDirectorySettter::~CCurrentDirectorySettter(void)
	{
		core::SetCurrentDirectoryW(m_strOriginalDir.c_str());
	}

	bool CCurrentDirectorySettter::DirChanged(void)
	{
		return m_bDirChanged;
	}
}