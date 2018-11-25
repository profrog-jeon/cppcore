#include "stdafx.h"
#include "300_Formatter.h"
#include "JSONSerializer.h"
#include "JSONDeserializer.h"
#include "FileWritter.h"
#include "FileReader.h"
#include "StringWriter.h"
#include "StringReader.h"
#include "BytesWriter.h"
#include "BytesReader.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	bool WriteJsonToFile(const IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
	{
		fmt_internal::CFileWritter channel(strFilename);
		fmt_internal::CJSONSerializer formatter(channel);
		const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
		return formatter.CheckValidity(pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadJsonFromFile(IFormatterObject* pObject, std::tstring strFilename, std::tstring* pStrErrMsg)
	{
		fmt_internal::CFileReader channel(strFilename);
		fmt_internal::CJSONDeserializer formatter(channel);
		pObject->Synchronize(formatter);
		return formatter.CheckValidity(pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool WriteJsonToString(const IFormatterObject* pObject, std::tstring& strOutput, std::tstring* pStrErrMsg)
	{
		fmt_internal::CStringWriter channel(strOutput);
		fmt_internal::CJSONSerializer formatter(channel);
		const_cast<IFormatterObject*>(pObject)->Synchronize(formatter);
		return formatter.CheckValidity(pStrErrMsg);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadJsonFromString(IFormatterObject* pObject, std::tstring strInput, std::tstring* pStrErrMsg)
	{
		fmt_internal::CStringReader channel(strInput);
		fmt_internal::CJSONDeserializer formatter(channel);
		pObject->Synchronize(formatter);
		return formatter.CheckValidity(pStrErrMsg);
	}
}
