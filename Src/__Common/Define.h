#pragma once

#ifdef UNICODE
	#ifndef TEXT
	#define TEXT(x) L##x
	#endif
	#ifndef _T
	#define _T(x) L##x
	#endif
#else
	#ifndef TEXT
	#define TEXT(x) x
	#endif
	#ifndef _T
	#define _T(x) x
	#endif
#endif

#ifndef CASE_TO_STR
#define CASE_TO_STR(x)		case x:	return TEXT(#x);
#endif

#ifndef CASE_TO_STR_T
#define CASE_TO_STR_T(x)	case x:	return TEXT(#x);
#endif

#ifndef CASE_TO_STR_A
#define CASE_TO_STR_A(x)	case x:	return #x;
#endif

#ifndef CASE_TO_STR_W
#define CASE_TO_STR_W(x)	case x:	return L#x;
#endif

#ifndef MAKEQWORD
#define MAKEQWORD(l, h)		(((QWORD)h << 32) | ((QWORD) l))
#endif

#ifndef NULL
#define NULL    0
#endif

#define OPT
#define IN
#define OUT
#define INOUT

#ifndef TRUE
#define TRUE				1
#endif

#ifndef FALSE
#define FALSE				0
#endif

#ifndef MAX_PATH
#define MAX_PATH			260
#endif

#ifndef INFINITE
#define INFINITE			0xFFFFFFFF
#define INFINITE32			0xFFFFFFFF
#define INFINITE64			0xFFFFFFFFFFFFFFFFull
#endif

#define MAX(A, B)			((A) > (B)? (A) : (B))
#define MIN(A, B)			((A) < (B)? (A) : (B))
#define DIFF(A, B)			((A) < (B)? ((B)-(A)) : ((A)-(B)))

#define SAFE_DELETE(x)		if( NULL != (x) )	{	delete (x); (x) = NULL;	}

#ifdef _MSC_VER
#define DECLARE_EXPORT	extern "C" __declspec(dllexport)
#else
#define DECLARE_EXPORT	extern "C" __attribute__((visibility("default")))
#endif
