#include "stdafx.h"
#include <Windows.h>
#include "FileSystem.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	void FindClose(HANDLE hFindFile)
	{
		::FindClose(hFindFile);
	}
}
