#include "stdafx.h"
#include "PEParser.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	bool CPEParser::Parse(std::tstring strFilePath)
	{
		HANDLE hFile = NULL;
		try
		{
			hFile = CreateFile(strFilePath.c_str(), GENERIC_READ_, OPEN_EXISTING_, 0);
			if (NULL == hFile)
				throw exception_format(TEXT("%s open failure"), strFilePath.c_str());

			m_tFileSize = (size_t)core::GetFileSize(hFile);
			if (0 == m_tFileSize)
				throw exception_format(TEXT("%s file size is zero"), strFilePath.c_str());

			m_hFileMapping = CreateFileMapping(hFile, PAGE_READONLY_, FILE_MAP_READ_, m_tFileSize);
			if (NULL == m_hFileMapping)
				throw exception_format("CreateFileMapping failure(m_hFile, PAGE_READONLY_, GENERIC_READ_, %lu)", m_tFileSize);

			m_pFileContents = (LPCBYTE)MapViewOfFile(m_hFileMapping, 0, (size_t)m_tFileSize);
			if (NULL == m_pFileContents)
				throw exception_format("MapViewOfFile(%llu) has failed", m_tFileSize);

			CloseFile(hFile);
			hFile = NULL;

			if (!Parse())
				throw exception_format("Parsing failure.");
		}
		catch (std::exception& e)
		{
			Log_Error("%s", e.what());
			if (hFile)
				CloseFile(hFile);
			Close();
			return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CPEParser::GetSection(std::tstring strName, ST_SECTION_DATA & data)
	{
		std::string strNameA = MBSFromTCS(strName);

		size_t i;
		for (i = 0; i < m_vecImageSectionHeader.size(); i++)
		{
			if (m_vecImageSectionHeader[i].Name != strNameA)
				continue;

			const ST_IMAGE_SECTION_HEADER& header = m_vecImageSectionHeader[i];

			size_t tEndOfAddress = header.PointerToRawData + header.SizeOfRawData;
			if (m_tFileSize < tEndOfAddress)
				return EC_INVALID_DATA;

			data.dwFileAddress = header.PointerToRawData;
			data.pData = m_pFileContents + header.PointerToRawData;
			data.tSize = header.SizeOfRawData;
			return EC_SUCCESS;
		}

		return EC_NO_DATA;
	}
}
