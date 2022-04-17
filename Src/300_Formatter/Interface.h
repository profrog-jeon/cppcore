#pragma once

#include <assert.h>
#include <string>
#include <map>
#include <set>
#include <list>
#include <vector>

#include "../000_String/000_String.h"
#include "FmtTypes.h"
#include "PairSuper.h"

#define IFormatterA				CFormatterSuperA
#define IFormatterW				CFormatterSuperW

#ifdef UNICODE	
	#define IFormatter				CFormatterSuperW
	#define IFormatterT				CFormatterSuperW
	#define IFormatterObject		IFormatterObjectW
	#define IFormatterObjectT		IFormatterObjectW
#else
	#define IFormatter				CFormatterSuperA
	#define IFormatterT				CFormatterSuperA
	#define IFormatterObject		IFormatterObjectA
	#define IFormatterObjectT		IFormatterObjectA
#endif

namespace core
{
	class CFormatterSuperA;
	class CFormatterSuperW;
	struct IFormatterObjectA
	{
		virtual ~IFormatterObjectA()	{}
		virtual void OnSync(CFormatterSuperA& formatter) = 0;
	};
	struct IFormatterObjectW
	{
		virtual ~IFormatterObjectW()	{}
		virtual void OnSync(CFormatterSuperW& formatter) = 0;
	};
}
