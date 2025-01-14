#include "stdafx.h"
#include "ELFParser.h"
#include "Log.h"
#include "FileSystem.h"
#include "KernelObject.h"

namespace core
{
	static LPCSTR g_pszFileVersionName = "FileVersion";
	static LPCSTR g_pszProductVersionName = "ProductVersion";

	//////////////////////////////////////////////////////////////////////////
	CELFParser::CELFParser(void)
		: CExeParserSuper()
		, m_mapSection64()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CELFParser::OpenWorker(HANDLE hFile)
	{
		try
		{
			DWORD dwReadSize = 0;
			ST_ELF_IDENTITY stIdentity = { 0, };
			if (!ReadFile(hFile, &stIdentity, sizeof(stIdentity), &dwReadSize))
				throw exception_format(TEXT("ELF header reading failure, read:%u"), dwReadSize);

			if (0x7F != stIdentity.ei_magic[0]
				|| 'E' != stIdentity.ei_magic[1]
				|| 'L' != stIdentity.ei_magic[2]
				|| 'F' != stIdentity.ei_magic[3])
				throw exception_format("ELF magic is not matched");

			ECODE nRet = EC_NO_DATA;
			if (1 == stIdentity.ei_class)
				nRet = Parse32Bit(hFile, stIdentity);
			if (2 == stIdentity.ei_class)
				nRet = Parse64Bit(hFile, stIdentity);
			if (EC_SUCCESS != nRet)
				throw exception_format("ELF parsing failure, %d", nRet);

			ST_VERSIONINFO stVersion;
			if (EC_SUCCESS == QueryVersion(hFile, g_pszFileVersionName, stVersion))
				m_mapVersionInfo.insert(std::make_pair(g_pszFileVersionName, stVersion));

			if (EC_SUCCESS == QueryVersion(hFile, g_pszProductVersionName, stVersion))
				m_mapVersionInfo.insert(std::make_pair(g_pszProductVersionName, stVersion));
		}
		catch (std::exception& e)
		{
			Log_Debug("%s", e.what());
			return EC_INTERNAL_ERROR;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	void CELFParser::Close(void)
	{

	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CELFParser::QueryVersion(HANDLE hFile, const char* pszSectionName, ST_VERSIONINFO& stVersionInfo)
	{
		ECODE nRet = EC_SUCCESS;
		try
		{
			CELFSectionHeader64It iter = m_mapSection64.find(pszSectionName);

			nRet = EC_NO_DATA;
			if( iter == m_mapSection64.end() )
				throw exception_format("VersionInfo section is not found");

			const ST_ELF_SECTION_HEADER64& shVersionInfo = iter->second;

			SetFilePointer(hFile, (long)shVersionInfo.sh_offset, FILE_BEGIN_);

			int nVersion[4];
			const size_t tVersionStructSize = sizeof(nVersion);
			if( (shVersionInfo.sh_size > 0) && (shVersionInfo.sh_size%tVersionStructSize > 0) )
				throw exception_format("Section VersionInfo size is not matched, try:%lu, stored:%lu", sizeof(nVersion), shVersionInfo.sh_size);

			DWORD dwReadSize = 0;
			if( !ReadFile(hFile, nVersion, sizeof(nVersion), &dwReadSize) )
				throw exception_format("VersionInfo section read failure, try:%lu, read:%lu", sizeof(nVersion), dwReadSize);

			stVersionInfo.dwMajor = nVersion[0];
			stVersionInfo.dwMinor = nVersion[1];
			stVersionInfo.dwPatch = nVersion[2];
			stVersionInfo.dwBuild = nVersion[3];
		}
		catch (std::exception& e)
		{
			Log_Debug("%s", e.what());
			return nRet;
		}
		
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CELFParser::GetFileVersion(ST_VERSIONINFO& outVersionInfo)
	{
		auto iter = m_mapVersionInfo.find(g_pszFileVersionName);
		if (iter == m_mapVersionInfo.end())
			return EC_NO_DATA;
		
		outVersionInfo = iter->second;
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CELFParser::GetProductVersion(ST_VERSIONINFO& outVersionInfo)
	{
		auto iter = m_mapVersionInfo.find(g_pszProductVersionName);
		if (iter == m_mapVersionInfo.end())
			return EC_NO_DATA;

		outVersionInfo = iter->second;
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CELFParser::Parse32Bit(HANDLE hFile, const ST_ELF_IDENTITY& stIdentity)
	{
		DWORD dwRead = 0;

		ST_ELF_HEADER32 stHeader = { 0, };
		if (!ReadFile(hFile, &stHeader, sizeof(stHeader), &dwRead))
			return EC_READ_FAILURE;

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CELFParser::Parse64Bit(HANDLE hFile, const ST_ELF_IDENTITY& stIdentity)
	{
		ECODE nRet = EC_SUCCESS;
		try
		{
			DWORD dwRead = 0;

			ST_ELF_HEADER64 stHeader = { 0, };
			nRet = EC_READ_FAILURE;
			if (!ReadFile(hFile, &stHeader, sizeof(stHeader), &dwRead))
				throw exception_format(TEXT("Reading ELF header has failed."));

			SetFilePointer(hFile, (long)stHeader.e_phoff, FILE_BEGIN_);
			std::vector<ST_ELF_PROGRAM_HEADER64> vecPHTable;
			size_t i;
			for(i=0; i<stHeader.e_phnum; i++)
			{
				ST_ELF_PROGRAM_HEADER64 stPH;
				nRet = EC_READ_FAILURE;
				if (!ReadFile(hFile, &stPH, sizeof(stPH), &dwRead))
					throw exception_format(TEXT("Reading ELF PH has failed."));

				vecPHTable.push_back(stPH);
			}

			SetFilePointer(hFile, (long)stHeader.e_shoff, FILE_BEGIN_);
			std::vector<ST_ELF_SECTION_HEADER64> vecSHTable;
			for(i=0; i<stHeader.e_shnum; i++)
			{
				ST_ELF_SECTION_HEADER64 stSH;
				nRet = EC_READ_FAILURE;
				if (!ReadFile(hFile, &stSH, sizeof(stSH), &dwRead))
					throw exception_format(TEXT("Reading ELF SH has failed."));

				vecSHTable.push_back(stSH);
			}

			std::vector<char> vecStringTable;
			nRet = EC_INVALID_DATA;
			if( vecSHTable.size() < stHeader.e_shstrndx )
				throw exception_format("Invalid ELF string table index");

			nRet = EC_INVALID_DATA;
			if( SHT_STRTAB != vecSHTable[stHeader.e_shstrndx].sh_type )
				throw exception_format("Invalid ELF string table location");

			{
				const ST_ELF_SECTION_HEADER64& shStringTable = vecSHTable[stHeader.e_shstrndx];
				SetFilePointer(hFile, (long)shStringTable.sh_offset, FILE_BEGIN_);
				vecStringTable.resize((size_t)shStringTable.sh_size);

				nRet = EC_READ_FAILURE;
				if (!ReadFile(hFile, &vecStringTable[0], (size_t)shStringTable.sh_size, &dwRead))
					throw exception_format("Reading ELF string table has failed");
			}

			for(i=0; i<vecSHTable.size(); i++)
			{
				const ST_ELF_SECTION_HEADER64& curSectionHeader = vecSHTable[i];
				if(curSectionHeader.sh_name < vecStringTable.size())
				{
					std::string strName = &vecStringTable[curSectionHeader.sh_name];
					m_mapSection64.insert(std::make_pair(strName, curSectionHeader));
				}
				else
					m_mapSection64.insert(std::make_pair("InvalidName", curSectionHeader));
			}
		}
		catch (std::exception& e)
		{
			Log_Debug("%s", e.what());
			return nRet;
		}

		return EC_SUCCESS;
	}
}
