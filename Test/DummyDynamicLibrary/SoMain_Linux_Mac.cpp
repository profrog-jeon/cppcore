#include "stdafx.h"

HANDLE g_hModule = NULL;

__attribute__((constructor)) void on_load(void)
{
	g_hModule = (HANDLE)on_load;
}