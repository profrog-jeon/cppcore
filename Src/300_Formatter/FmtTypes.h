#pragma once

#include "../__Common/Type.h"

#include <string>
#include <vector>

#ifdef UNICODE
#define fmt_internal				fmt_internalW
typedef std::vector<std::wstring>	CTStringVec;
#else
#define fmt_internal				fmt_internalA
typedef std::vector<std::string>	CTStringVec;
#endif

