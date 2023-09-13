#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "../__Common/Define.h"
#include "../__Common/Type.h"
#include "../__Common/ErrorCode.h"

#include "../000_String/000_String.h"
#include "../100_System/100_System.h"
#include "../200_Common/200_Common.h"

#include "300_Formatter.h"
#include "FmtTypes.h"
#include "Interface.h"
#include "FormatterSuper.h"
#include "HelperFunc.h"

#include <string.h>
#include <map>
#include <list>
#include <stack>

static const size_t g_tSecTokenLeng = 1024*1024;

using namespace core;

#ifdef UNICODE
#define sToken		sTokenW				
#define sChunk		sChunkW				
#define CChunkVec	std::vector<sChunkW>
#define CTokenVec	std::vector<sTokenW>
#define CJSONDeserializer	CJSONDeserializerW
#else
#define sToken		sTokenA				
#define sChunk		sChunkA				
#define CTokenVec	std::vector<sTokenA>
#define CChunkVec	std::vector<sChunkA>
#define CJSONDeserializer	CJSONDeserializerA
#endif
