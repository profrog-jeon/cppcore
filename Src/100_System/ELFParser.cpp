#include "stdafx.h"
#include "ELFParser.h"
#include "Log.h"
#include "FileSystem.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CELFParser::CELFParser(void)
		: m_strFilePath()
		, m_mapSection64()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	#define READ_BIN_FROM_FILE(TARGET)	\
	{\
		size_t tRead = fread(&TARGET, 1, sizeof(TARGET), pFile);\
		if( tRead != sizeof(TARGET) )\
			throw exception_format("ELF %s read failure, try:%lu, read:%lu", #TARGET, sizeof(TARGET), tRead);\
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CELFParser::Parse(const char* pszFilePath)
	{
		FILE* pFile = NULL;

		try
		{
			pFile = fopenA(pszFilePath, "rb");
			if( NULL == pFile )
				throw exception_format("file open failure, %s", pszFilePath);

			ST_ELF_IDENTITY stIdentity = { 0, };
			READ_BIN_FROM_FILE(stIdentity);

			if( 0x7F != stIdentity.ei_magic[0]
			||  'E'  != stIdentity.ei_magic[1]
			||  'L'  != stIdentity.ei_magic[2]
			||  'F'  != stIdentity.ei_magic[3] )
				throw exception_format("ELF magic is not matched");

            ECODE nRet = EC_SUCCESS;
			if( 1 == stIdentity.ei_class )
				nRet = Parse32Bit(pFile, stIdentity);

			if( 2 == stIdentity.ei_class )
				nRet = Parse64Bit(pFile, stIdentity);

			if( EC_SUCCESS != nRet )
				throw exception_format("ELF parsing failure, %d", nRet);

			fclose(pFile);
		}
		catch (std::exception& e)
		{
			e;	// Declaration to resolve unused variable warning
			//Log_Error("%s", e.what());
			if( pFile )
				fclose(pFile);
			return EC_INTERNAL_ERROR;
		}

		m_strFilePath = pszFilePath;
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CELFParser::QueryELFVersion(const char* pszSectioName, ST_VERSIONINFO& stVersionInfo)
	{
		ECODE nRet = EC_SUCCESS;
		FILE* pFile = NULL;
		try
		{
			CELFSectionHeader64It iter = m_mapSection64.find(pszSectioName);

			nRet = EC_NO_DATA;
			if( iter == m_mapSection64.end() )
				throw exception_format("VersionInfo section is not found");

			const ST_ELF_SECTION_HEADER64& shVersionInfo = iter->second;

			nRet = EC_INVALID_FILE;
			pFile = fopenA(m_strFilePath.c_str(), "rb");
			if( NULL == pFile )
				throw exception_format("File(%s) open failure", m_strFilePath.c_str());

			fseek(pFile, (long)shVersionInfo.sh_offset, SEEK_SET);

			int nVersion[4];
			const size_t tVersionStructSize = sizeof(nVersion);
			if( (shVersionInfo.sh_size > 0) && (shVersionInfo.sh_size%tVersionStructSize > 0) )
				throw exception_format("Section VersionInfo size is not matched, try:%lu, stored:%lu", sizeof(nVersion), shVersionInfo.sh_size);

			size_t tRead = fread(nVersion, 1, sizeof(nVersion), pFile);
			if( tRead != sizeof(nVersion) )
				throw exception_format("VersionInfo section read failure, try:%lu, read:%lu", sizeof(nVersion), tRead);

			stVersionInfo.dwMajor = nVersion[0];
			stVersionInfo.dwMinor = nVersion[1];
			stVersionInfo.dwPatch = nVersion[2];
			stVersionInfo.dwBuild = nVersion[3];
			fclose(pFile);
		}
		catch (std::exception& e)
		{
			e;	// Declaration to resolve unused variable warning
			//Log_Error("%s", e.what());	// skip for unexpected console output, caused by ConsolFramework
			if( pFile )
				fclose(pFile);
			return nRet;
		}
		
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CELFParser::QueryFileVersion(ST_VERSIONINFO& stVersionInfo)
	{
		return QueryELFVersion("FileVersion", stVersionInfo);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CELFParser::QueryProductVersion(ST_VERSIONINFO& stVersionInfo)
	{
		return QueryELFVersion("ProductVersion", stVersionInfo);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CELFParser::Parse32Bit(FILE* pFile, const ST_ELF_IDENTITY& stIdentity)
	{
		ST_ELF_HEADER32 stHeader = { 0, };
		READ_BIN_FROM_FILE(stHeader);

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CELFParser::Parse64Bit(FILE* pFile, const ST_ELF_IDENTITY& stIdentity)
	{
		try
		{
			ST_ELF_HEADER64 stHeader = { 0, };
			READ_BIN_FROM_FILE(stHeader);

			fseek(pFile, (long)stHeader.e_phoff, SEEK_SET);
			std::vector<ST_ELF_PROGRAM_HEADER64> vecPHTable;
			size_t i;
			for(i=0; i<stHeader.e_phnum; i++)
			{
				ST_ELF_PROGRAM_HEADER64 stPH;
				READ_BIN_FROM_FILE(stPH);

				vecPHTable.push_back(stPH);
			}

			fseek(pFile, (long)stHeader.e_shoff, SEEK_SET);
			std::vector<ST_ELF_SECTION_HEADER64> vecSHTable;
			for(i=0; i<stHeader.e_shnum; i++)
			{
				ST_ELF_SECTION_HEADER64 stPH;
				READ_BIN_FROM_FILE(stPH);

				vecSHTable.push_back(stPH);
			}

			std::vector<char> vecStringTable;
			if( vecSHTable.size() < stHeader.e_shstrndx )
				throw exception_format("Invalid ELF string table index");

			if( SHT_STRTAB != vecSHTable[stHeader.e_shstrndx].sh_type )
				throw exception_format("Invalid ELF string table location");

			{
				const ST_ELF_SECTION_HEADER64& shStringTable = vecSHTable[stHeader.e_shstrndx];
				fseek(pFile, (long)shStringTable.sh_offset, SEEK_SET);
				vecStringTable.resize((size_t)shStringTable.sh_size);
				fread(&vecStringTable[0], (size_t)shStringTable.sh_size, 1, pFile);
			}

			for(i=0; i<vecSHTable.size(); i++)
			{
				const ST_ELF_SECTION_HEADER64& curSectionHeader = vecSHTable[i];
				if( vecStringTable.size() <= curSectionHeader.sh_name )
					m_mapSection64.insert(std::make_pair("InvalidName", curSectionHeader));
				else
				{
					std::string strName = &vecStringTable[curSectionHeader.sh_name];
					m_mapSection64.insert(std::make_pair(strName, curSectionHeader));
				}
			}
		}
		catch (std::exception& e)
		{
			e;	// Declaration to resolve unused variable warning
			//Log_Error("%s", e.what());
			return EC_INTERNAL_ERROR;
		}

		return EC_SUCCESS;
	}
}
