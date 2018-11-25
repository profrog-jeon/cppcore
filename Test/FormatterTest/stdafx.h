#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>

#include "gtest/gtest.h"
#include "../../Src/000_String/000_String.h"
#include "../../Src/100_System/100_System.h"
#include "../../Src/200_Common/200_Common.h"
#include "../../Src/300_Formatter/300_Formatter.h"

#ifdef UNICODE
#define std_cout	std::wcout
#else
#define std_cout	std::cout
#endif

using namespace core;
