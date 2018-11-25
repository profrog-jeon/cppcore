#include "stdafx.h"
#include "300_Formatter.h"
#include "JSONSerializer.h"
#include "JSONDeserializer.h"
#include "FileWritter.h"
#include "FileReader.h"
#include "StringWriter.h"
#include "StringReader.h"
#include "BytesReader.h"
#include "ASCIIFileReader.h"
#include "ASCIIFileWritter.h"
#include "ASCIIStringReader.h"
#include "ASCIIStringWritter.h"

namespace core
{
	namespace ASCII
	{
		//////////////////////////////////////////////////////////////////////////
		bool WriteJsonToFile(const IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CASCIIFileWritter channel(strFilename.c_str());
			fmt_internal::CJSONSerializer formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadJsonFromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CASCIIFileReader channel(strFilename.c_str());
			fmt_internal::CJSONDeserializer formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteJsonToString(const IFormatterObject* pObject, std::string& strOutput)
		{
			fmt_internal::CASCIIStringWriter channel(strOutput);
			fmt_internal::CJSONSerializer formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadJsonFromString(IFormatterObject* pObject, std::string strInput, std::tstring* pStrErrMsg)
		{
			fmt_internal::CASCIIStringReader channel(strInput);
			fmt_internal::CJSONDeserializer formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

	}
}
