#include "stdafx.h"
#include "TextFileReader.h"
#include "Utility.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CTextFileReader::CTextFileReader(std::tstring strFileName, E_BOM_TYPE nEncodeType)
		: m_hFile(NULL)
		, m_nBOMType(nEncodeType)
		, m_qwFileSize(0)
	{
		m_hFile = CreateFile(strFileName.c_str(), GENERIC_READ_, OPEN_EXISTING_, 0);
		if( NULL == m_hFile )
		{
			Log_Error(TEXT("fopenT failure at %s"), strFileName.c_str());
			return;
		}

		if( BOM_UNDEFINED != m_nBOMType )
			ReadBOM(m_hFile);
		else
			m_nBOMType = ReadBOM(m_hFile);
		m_qwFileSize = GetFileSize(m_hFile);
	}

	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	static int ReadLineFromFile(std::vector<T>& vecContext, HANDLE hFile, bool bBigEndian = false)
	{
		int nReadSize = 0;

		size_t tCharSize = sizeof(T);
		size_t tReservedSize = 256;
		vecContext.resize(tReservedSize);

		T cNewLineChar = bBigEndian? ReverseByteOrder((T)'\n') : '\n';

		size_t i;
		for(i=0; ; i++)
		{
			if( i >= tReservedSize )
			{
				// A long line exceeding 10MB will be excepted
				if( tReservedSize > 10 * 1024 * 1024 )
					break;

				tReservedSize *= 2;
				vecContext.resize(tReservedSize);
			}

			DWORD dwReadSize = 0;
			if( !ReadFile(hFile, &vecContext[i], tCharSize, &dwReadSize) )
				break;

			if( dwReadSize != tCharSize )
				break;

			nReadSize += (int)tCharSize;
			if( vecContext[i] == cNewLineChar )
				break;
		}

		vecContext.resize(nReadSize / tCharSize);
		return nReadSize;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CTextFileReader::ReadLine(std::tstring& strContext)
	{
		int nReadSize = 0;

		try
		{
			if( NULL == m_hFile )
				throw exception_format("File is not opened");

			if( (QWORD)Seek(0, FILE_CURRENT_) >= m_qwFileSize )
				return EC_ENDOFFILE;

			switch(m_nBOMType)
			{
			case BOM_UTF8:
			{
				std::vector<char> vecBuffer;
				if( (nReadSize = ReadLineFromFile(vecBuffer, m_hFile)) > 0 )
					strContext = TCSFromUTF8(&vecBuffer[0]);
			}
			break;

			case BOM_UTF16:
			{
				std::vector<WORD> vecBuffer;
				if( (nReadSize = ReadLineFromFile(vecBuffer, m_hFile)) > 0 )
					strContext = TCSFromUTF16(&vecBuffer[0], vecBuffer.size());
			}
			break;

			case BOM_UTF32:
			{
				std::vector<DWORD> vecBuffer;
				if( (nReadSize = ReadLineFromFile(vecBuffer, m_hFile)) > 0 )
					strContext = TCSFromUTF32(&vecBuffer[0], vecBuffer.size());
			}
			break;

			case BOM_UTF16_BE:
			{
				std::vector<WORD> vecBuffer;
				if( (nReadSize = ReadLineFromFile(vecBuffer, m_hFile, true)) > 0 )
				{
					size_t i;
					for(i=0; i<vecBuffer.size(); i++)
						vecBuffer[i] = ReverseByteOrder(vecBuffer[i]);
					strContext = TCSFromUTF16(&vecBuffer[0], vecBuffer.size());
				}
			}
			break;

			case BOM_UTF32_BE:
			{
				std::vector<DWORD> vecBuffer;
				if( (nReadSize = ReadLineFromFile(vecBuffer, m_hFile, true)) > 0 )
				{
					size_t i;
					for(i=0; i<vecBuffer.size(); i++)
						vecBuffer[i] = ReverseByteOrder(vecBuffer[i]);
					strContext = TCSFromUTF32(&vecBuffer[0], vecBuffer.size());
				}						
			}
			break;

			case BOM_UNDEFINED:
			default:
			{
				std::vector<char> vecBuffer;
				if( (nReadSize = ReadLineFromFile(vecBuffer, m_hFile)) > 0 )
					strContext = TCSFromASCII(&vecBuffer[0]);
			}
			break;
			}
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			return EC_INVALID_DATA;
		}

		return EC_SUCCESS;
	}
}
