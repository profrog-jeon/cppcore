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
	static inline LPBYTE ReverseByteOrderWorker(LPBYTE pDest, size_t tSize)
	{
		size_t i = 0;
		const size_t tHalfIndex = tSize / 2;
		for (i = 0; i < tHalfIndex; i++)
		{
			size_t o = tSize - i - 1;
			BYTE temp = pDest[i];
			pDest[i] = pDest[o];
			pDest[o] = temp;
		}
		return pDest;
	}

    //////////////////////////////////////////////////////////////////////////
	BYTE ReverseByteOrder(BYTE btData)
	{
		return btData;
	}

	//////////////////////////////////////////////////////////////////////////
	WORD ReverseByteOrder(WORD wData)
	{
		return *(WORD*)ReverseByteOrderWorker((LPBYTE)&wData, 2);
	}

	//////////////////////////////////////////////////////////////////////////
	DWORD ReverseByteOrder(DWORD dwData)
	{
		return *(DWORD*)ReverseByteOrderWorker((LPBYTE)&dwData, 4);
	}

	LPBYTE ReverseByteOrder(LPBYTE pDest, size_t tSize)
	{
		return ReverseByteOrderWorker(pDest, tSize);;
	}

	//////////////////////////////////////////////////////////////////////////
	std::vector<BYTE>& ReverseByteOrder(std::vector<BYTE>& refData)
	{
		ReverseByteOrderWorker(refData.data(), refData.size());
		return refData;
	}

	//////////////////////////////////////////////////////////////////////////
	std::vector<BYTE>& ReverseByteOrder(const std::vector<BYTE>& inData, std::vector<BYTE>& outData)
	{
		outData = inData;
		ReverseByteOrderWorker(outData.data(), outData.size());
		return outData;
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

	//////////////////////////////////////////////////////////////////////////
	double GetCurrentSecond(void)
	{
		QWORD now;
		QueryPerformanceCounter(&now);

		QWORD freq;
		QueryPerformanceFrequency(&freq);

		const double dMilliSecond = (double)now / freq;
		return dMilliSecond;
	}
}

