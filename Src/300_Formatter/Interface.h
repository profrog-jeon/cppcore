#pragma once

#include <assert.h>
#include <string>
#include <map>
#include <set>
#include <list>
#include <vector>

#include "../000_String/000_String.h"
#include "FmtTypes.h"

#ifdef UNICODE
	#define CFormatterSuper			CFormatterSuperW	
	#define IFormatter				IFormatterW
	#define IFormatterT				IFormatterW
	#define IFormatterObject		IFormatterObjectW
	#define IFormatterObjectT		IFormatterObjectW
#else
	#define CFormatterSuper			CFormatterSuperA
	#define IFormatter				IFormatterA
	#define IFormatterT				IFormatterA
	#define IFormatterObject		IFormatterObjectA
	#define IFormatterObjectT		IFormatterObjectA
#endif

namespace core
{
	struct IFormatterA;
	struct IFormatterObjectA
	{
		virtual ~IFormatterObjectA()	{}
		virtual void OnSync(IFormatterA& formatter) = 0;
	};

	struct IFormatterW;
	struct IFormatterObjectW
	{
		virtual ~IFormatterObjectW()	{}
		virtual void OnSync(IFormatterW& formatter) = 0;
	};
}
