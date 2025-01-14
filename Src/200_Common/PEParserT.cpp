#include "stdafx.h"
#include "PEParser.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	bool CPEParser::Parse(std::tstring strFilePath)
	{
		return CExeParserSuper::Open(strFilePath) == EC_SUCCESS;
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
