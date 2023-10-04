#include "stdafx.h"
#include "Utility.h"
#include "FileSystem.h"

namespace core
{
	CCurrentDirectorySettter::CCurrentDirectorySettter(std::tstring strWorkingDir)
		: m_strOriginalDir()
	{
		m_strOriginalDir = GetCurrentDirectoryW();
		core::SetCurrentDirectory(strWorkingDir.c_str());
	}
}