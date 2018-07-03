#include "stdafx.h"
#include "Utility.h"
#include "TextFileReader.h"

namespace core
{
    //////////////////////////////////////////////////////////////////////////
    ST_FUNC_LOG::~ST_FUNC_LOG(void)
    {
        Log_Info("---------- %s finished ----------", m_strName.c_str());
    }

	//////////////////////////////////////////////////////////////////////////
	ST_FUNC_CONSOLE_LOG::~ST_FUNC_CONSOLE_LOG(void)
	{
		printf("---------- %s finished ----------\n", m_strName.c_str());
	}
    
    //////////////////////////////////////////////////////////////////////////
	char ReverseByteOrder(char btData)
	{
		return btData;
	}

	//////////////////////////////////////////////////////////////////////////
	WORD ReverseByteOrder(WORD wData)
	{
		return ((wData >>  8) & 0x000000FF)
			|  ((wData <<  8) & 0x0000FF00);
	}

	//////////////////////////////////////////////////////////////////////////
	DWORD ReverseByteOrder(DWORD dwData)
	{
		return ((dwData << 24) & 0xFF000000)
			|  ((dwData <<  8) & 0x00FF0000)
			|  ((dwData >>  8) & 0x0000FF00)
			|  ((dwData >> 24) & 0x000000FF);
	}

	//////////////////////////////////////////////////////////////////////////
	E_BOM_TYPE ReadBOM(HANDLE hFile)
	{
		BYTE btBOM[5] = { 0, };
		DWORD dwReadSize = 0;
		if( !ReadFile(hFile, btBOM, 5, &dwReadSize) )
			return BOM_UNDEFINED;

		if( 0 == dwReadSize )
			return BOM_UNDEFINED;

		ST_BOM_INFO stBOM = { 0, };
		E_BOM_TYPE nType = ReadBOM(btBOM, dwReadSize, stBOM);
		if( BOM_UNDEFINED == nType )
		{
			SetFilePointer(hFile, 0, FILE_BEGIN_);
			return BOM_UNDEFINED;
		}

		SetFilePointer(hFile, stBOM.tSize, FILE_BEGIN_);
		return nType;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE WriteBOM(HANDLE hFile, E_BOM_TYPE nType)
	{
		if( (nType >= BOM_COUNT) || ((int)nType < 0) )
			return EC_INVALID_BOM_TYPE;

		const ST_BOM_INFO* pBOM = GetBOMInfo(nType);
		if( NULL == pBOM )
			return EC_INVALID_BOM_TYPE;

		DWORD dwWritten = 0;
		if( !WriteFile(hFile, pBOM->btSeq, pBOM->tSize, &dwWritten) )
			return GetLastError();

		if( dwWritten != pBOM->tSize )
			return EC_WRITE_FAILURE;

		return EC_SUCCESS;
	}
}

