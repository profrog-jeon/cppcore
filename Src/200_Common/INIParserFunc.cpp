#include "stdafx.h"
#include "INIParserFunc.h"
#include "INIParser.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	enum __internal_E_INI_PARSER_TYPE
	{
		INI_PARSER_TYPE_PLAIN,
		INI_PARSER_TYPE_CIPHER_KEY
	};

	//////////////////////////////////////////////////////////////////////////
	struct __internal_ST_INI_PARSER_HANDLE
	{
		CINIParser* pParser;
		std::tstring strPath;
		__internal_E_INI_PARSER_TYPE nType;
		ST_SYM_CIPHER_KEY stCipherKey;
	};

	//////////////////////////////////////////////////////////////////////////
	HANDLE CreateINIHandle(std::tstring strPath)
	{
		__internal_ST_INI_PARSER_HANDLE* pHandle = NULL;
		CINIParser* pParser = NULL;

		try
		{
			pParser = new(std::nothrow) CINIParser();
			CTextFileReader reader(strPath);
			pParser->Open(&reader);

			pHandle = new __internal_ST_INI_PARSER_HANDLE;
			pHandle->pParser = pParser;
			pHandle->strPath = strPath;
			pHandle->nType = INI_PARSER_TYPE_PLAIN;
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			SAFE_DELETE(pParser);
			return NULL;
		}
		return (HANDLE)pHandle;
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GetINIValue(HANDLE hINI, std::tstring strSection, std::tstring strKey, std::tstring strDefault)
	{
		__internal_ST_INI_PARSER_HANDLE* pHandle = (__internal_ST_INI_PARSER_HANDLE*)hINI;
		return pHandle->pParser->GetValue(strSection, strKey, strDefault);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE SetINIValue(HANDLE hINI, std::tstring strSection, std::tstring strKey, std::tstring strValue)
	{
		__internal_ST_INI_PARSER_HANDLE* pHandle = (__internal_ST_INI_PARSER_HANDLE*)hINI;
		return pHandle->pParser->SetValue(strSection, strKey, strValue);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE FlushINI(HANDLE hINI)
	{
		__internal_ST_INI_PARSER_HANDLE* pHandle = (__internal_ST_INI_PARSER_HANDLE*)hINI;

		if( INI_PARSER_TYPE_PLAIN == pHandle->nType )
		{
			CTextFileWriter writer(pHandle->strPath, BOM_UTF8);
			return pHandle->pParser->Flush(&writer);
		}
		if( INI_PARSER_TYPE_CIPHER_KEY == pHandle->nType )
		{
			return EC_NOT_IMPLEMENTED;
		}
		return EC_INVALID_DATA;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE DestroyINIHandle(HANDLE hINI)
	{
		__internal_ST_INI_PARSER_HANDLE* pHandle = (__internal_ST_INI_PARSER_HANDLE*)hINI;
		SAFE_DELETE(pHandle->pParser);
		SAFE_DELETE(pHandle);
		return EC_SUCCESS;
	}
}
