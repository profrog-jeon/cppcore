#include "stdafx.h"
#include "TextFileReader.h"
#include "Utility.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CTextFileReader::CTextFileReader(std::tstring strFileName, E_BOM_TYPE nEncodeType)
		: m_MemMappedFile()
		, m_pContext(NULL)
		, m_nBOMType(nEncodeType)
		, m_tFileSize(0)
		, m_tReadPos(0)
	{
		ECODE nRet = m_MemMappedFile.Create(strFileName, PAGE_READWRITE_, FILE_MAP_READ_);
		if( EC_SUCCESS != nRet)
		{
			Log_Error(TEXT("MemMappedFile.Create(%s) failure, %d"), strFileName.c_str(), nRet);
			return;
		}

		m_pContext = m_MemMappedFile.Ptr();
		m_tFileSize = m_MemMappedFile.Size();

		ST_BOM_INFO stBomInfo;
		if( BOM_UNDEFINED != m_nBOMType )
			ReadBOM(m_pContext, m_tFileSize, stBomInfo);
		else
			m_nBOMType = ReadBOM(m_pContext, m_tFileSize, stBomInfo);

		if(BOM_UNDEFINED != m_nBOMType)
			m_tReadPos = stBomInfo.tSize;
	}

	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	static size_t ReadLineFromFile(std::vector<T>& vecContext, LPCBYTE pContext, size_t tContextSize, size_t& tPos, bool bBigEndian = false)
	{
		if (tContextSize == tPos)
			return 0;

		const size_t tCharSize = sizeof(T);
		const size_t tMargin = bBigEndian ? 0 : (tCharSize - 1);	// 0A 00 00 00 의 마지막 3바이트를 위한..

		size_t tCrIndex = -1;
		for (size_t i = tPos; i < tContextSize; i++)
		{
			if (pContext[i] != '\n')
				continue;
			tCrIndex = i;
			break;
		}
		
		const size_t tPrePos = tPos;
		if (tCrIndex == -1)
			tPos = tContextSize;
		else
			tPos = tCrIndex + 1 + tMargin;

		const size_t tLength = tPos - tPrePos;
		const size_t tCharCount = tLength / tCharSize;
		vecContext.resize(tCharCount, 0);
		memcpy((LPBYTE)vecContext.data(), pContext + tPrePos, tLength);
		return tLength;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CTextFileReader::ReadLine(std::tstring& strContext)
	{
		size_t tReadSize = 0;

		try
		{
			switch(m_nBOMType)
			{
			case BOM_UTF8:
			{
				std::vector<char> vecBuffer;
				if (0 < (tReadSize = ReadLineFromFile(vecBuffer, m_pContext, m_tFileSize, m_tReadPos)))
				{
					vecBuffer.push_back(0);
					strContext = TCSFromUTF8(&vecBuffer[0]);
				}
			}
			break;

			case BOM_UTF16:
			{
				std::vector<WORD> vecBuffer;
				if (0 < (tReadSize = ReadLineFromFile(vecBuffer, m_pContext, m_tFileSize, m_tReadPos)))
					strContext = TCSFromUTF16(&vecBuffer[0], vecBuffer.size());
			}
			break;

			case BOM_UTF32:
			{
				std::vector<DWORD> vecBuffer;
				if (0 < (tReadSize = ReadLineFromFile(vecBuffer, m_pContext, m_tFileSize, m_tReadPos)))
					strContext = TCSFromUTF32(&vecBuffer[0], vecBuffer.size());
			}
			break;

			case BOM_UTF16_BE:
			{
				std::vector<WORD> vecBuffer;
				if (0 < (tReadSize = ReadLineFromFile(vecBuffer, m_pContext, m_tFileSize, m_tReadPos, true)))
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
				if (0 < (tReadSize = ReadLineFromFile(vecBuffer, m_pContext, m_tFileSize, m_tReadPos, true)))
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
				if (0 < (tReadSize = ReadLineFromFile(vecBuffer, m_pContext, m_tFileSize, m_tReadPos)))
				{
					vecBuffer.push_back(0);
					strContext = TCSFromANSI(&vecBuffer[0]);
				}
			}
			break;
			}

			if (0 == tReadSize)
				return EC_NO_DATA;
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			return EC_INVALID_DATA;
		}

		return EC_SUCCESS;
	}
}
