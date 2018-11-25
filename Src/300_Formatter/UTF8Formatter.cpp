#include "stdafx.h"
#include "300_Formatter.h"
#include "JSONSerializer.h"
#include "JSONDeserializer.h"
#include "FileWritter.h"
#include "FileReader.h"
#include "StringWriter.h"
#include "StringReader.h"
#include "BytesReader.h"
#include "UTF8FileReader.h"
#include "UTF8FileWritter.h"
#include "UTF8StringReader.h"
#include "UTF8StringWritter.h"

namespace core
{
	namespace UTF8
	{
		//////////////////////////////////////////////////////////////////////////
		bool WriteJsonToFile(const IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename.c_str());
			fmt_internal::CJSONSerializer formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteJsonToFile(const IFormatterObject* pObject, std::tstring strFilename, bool bWriteBOM)
		{
			fmt_internal::CUTF8FileWritter channel(strFilename.c_str(), bWriteBOM);
			fmt_internal::CJSONSerializer formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(NULL);
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadJsonFromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8FileReader channel(strFilename.c_str());
			fmt_internal::CJSONDeserializer formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}

		//////////////////////////////////////////////////////////////////////////
		bool WriteJsonToString(const IFormatterObject* pObject, std::string& strOutput)
		{
			fmt_internal::CUTF8StringWriter channel(strOutput);
			fmt_internal::CJSONSerializer formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		bool ReadJsonFromString(IFormatterObject* pObject, std::string strInput, std::tstring* pStrErrMsg)
		{
			fmt_internal::CUTF8StringReader channel(strInput);
			fmt_internal::CJSONDeserializer formatter(channel);
			const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
			return formatter.CheckValidity(pStrErrMsg);
		}
	}
}
