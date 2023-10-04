#include "stdafx.h"
#include "TextFileWriter.h"
#include "Utility.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CTextFileWriter::CTextFileWriter(std::tstring strFileName, E_BOM_TYPE nBOMType)
		: m_hFile(NULL)
		, m_nBOMType(nBOMType)
	{
		m_hFile = CreateFile(strFileName.c_str(), GENERIC_WRITE_, CREATE_ALWAYS_, 0);
		if( NULL == m_hFile )
		{
			Log_Error(TEXT("CreateFile(%s, GENERIC_WRITE_, CREATE_ALWAYS_) failure"), strFileName.c_str());
			return;
		}

		int nRet = WriteBOM(m_hFile, nBOMType);
		if( EC_SUCCESS != nRet )
		{
			Log_Error("WriteBom failure, %n", nRet);
			return;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	int CTextFileWriter::WriteLine(std::tstring strContext)
	{
		DWORD dwWriteSize = 0;
		try
		{
			if( NULL == m_hFile )
				throw exception_format("File is not opened");

			strContext += TEXT("\r\n");
			switch(m_nBOMType)
			{
			case BOM_UTF8:
			{
				std::string strData = UTF8FromTCS(strContext);
				WriteFile(m_hFile, strData.c_str(), 1 * (DWORD)strData.size(), &dwWriteSize);
			}	break;

			case BOM_UTF16:
			{
				std::vector<WORD> strData = UTF16FromTCS(strContext);
				WriteFile(m_hFile, &strData[0], 2 * (DWORD)strData.size(), &dwWriteSize);
			}	break;

			case BOM_UTF32:
			{
				std::vector<DWORD> strData = UTF32FromTCS(strContext);
				WriteFile(m_hFile, &strData[0], 4 * (DWORD)strData.size(), &dwWriteSize);
			}	break;

			case BOM_UTF16_BE:
			{
				std::vector<WORD> strData = UTF16FromTCS(strContext);
				size_t i;
				for(i=0; i<strData.size(); i++)
					strData[i] = ReverseByteOrder(strData[i]);
				WriteFile(m_hFile, &strData[0], 2 * (DWORD)strData.size(), &dwWriteSize);
			}	break;

			case BOM_UTF32_BE:
			{
				std::vector<DWORD> strData = UTF32FromTCS(strContext);
				size_t i;
				for(i=0; i<strData.size(); i++)
					strData[i] = ReverseByteOrder(strData[i]);
				WriteFile(m_hFile, &strData[0], 4 * (DWORD)strData.size(), &dwWriteSize);
			}	break;

			default:
			{
				std::string strData = ANSIFromTCS(strContext);
				WriteFile(m_hFile, strData.c_str(), 1 * (DWORD)strData.size(), &dwWriteSize);
			}	break;
			}
		}
		catch(std::exception& e)
		{
			Log_Error("%s", e.what());
			return -1;
		}

		return (int)dwWriteSize;
	}
}
