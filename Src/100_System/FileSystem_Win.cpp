#include <Windows.h>
#include "stdafx.h"
#include "FileSystem.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	void FindClose(HANDLE hFindFile)
	{
		::FindClose(hFindFile);
	}
}
