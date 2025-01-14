#include "stdafx.h"
#include "PEParser.h"
#include <math.h>

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	CPEParser::CPEParser(void)
		: CExeParserSuper()
		, m_hFileMapping(NULL)
		, m_pFileContents(NULL)
		, m_tFileSize(0)
		, m_DosHeader()
		, m_ImageNTHeader()
		, m_ImageOptionalHeader32()
		, m_ImageOptionalHeader64()
		, m_vecImageSectionHeader()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CPEParser::~CPEParser(void)
	{
		Close();
	}

	//////////////////////////////////////////////////////////////////////////
	bool CPEParser::Parse(LPCBYTE pContents, size_t tContentsSize)
	{
		m_pFileContents = pContents;
		m_tFileSize = tContentsSize;
		return Parse();
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CPEParser::OpenWorker(HANDLE hFile)
	{
		ECODE nRet = EC_SUCCESS;
		try
		{
			m_tFileSize = (size_t)core::GetFileSize(hFile);

			nRet = EC_NO_DATA;
			if (0 == m_tFileSize)
				throw exception_format(TEXT("file size is zero"));

			m_hFileMapping = CreateFileMapping(hFile, PAGE_READONLY_, FILE_MAP_READ_, m_tFileSize);
			nRet = GetLastError();
			if (NULL == m_hFileMapping)
				throw exception_format("CreateFileMapping failure(m_hFile, PAGE_READONLY_, GENERIC_READ_, %lu)", m_tFileSize);

			m_pFileContents = (LPCBYTE)MapViewOfFile(m_hFileMapping, 0, (size_t)m_tFileSize);
			nRet = GetLastError();
			if (NULL == m_pFileContents)
				throw exception_format("MapViewOfFile(%llu) has failed", m_tFileSize);

			nRet = EC_INVALID_DATA;
			if (!Parse())
				throw exception_format("Parsing failure.");
		}
		catch (std::exception& e)
		{
			Log_Debug("%s", e.what());
			Close();

			if (EC_SUCCESS != nRet)
				return nRet;
			return EC_INVALID_DATA;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	void CPEParser::Close(void)
	{
		if (m_pFileContents)
		{
			UnmapViewOfFile(m_hFileMapping, m_pFileContents);
			m_pFileContents = NULL;
		}

		if (m_hFileMapping)
		{
			CloseFileMappingHandle(m_hFileMapping);
			m_hFileMapping = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	inline static bool CopyTo(void* pDest, size_t tDestSize, LPCBYTE pSrc, size_t tSrcSize, size_t& tPos)
	{
		if (tPos > tSrcSize)
			return false;

		if ((tPos + tDestSize) > tSrcSize)
			return false;

		::memcpy(pDest, &pSrc[tPos], tDestSize);
		tPos += tDestSize;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline static bool CopyTo(T& refDest, LPCBYTE pSrc, size_t tSrcSize, size_t& tPos)
	{
		if (tPos > tSrcSize)
			return false;

		size_t tDestSize = sizeof(T);
		if ((tPos + tDestSize) > tSrcSize)
			return false;

		::memcpy(&refDest, &pSrc[tPos], tDestSize);
		tPos += tDestSize;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline static bool CopyTo(std::vector<T>& refDest, LPCBYTE pSrc, size_t tSrcSize, size_t& tPos)
	{
		if (tPos > tSrcSize)
			return false;

		size_t tDestSize = refDest.size() * sizeof(T);
		if ((tPos + tDestSize) > tSrcSize)
			return false;

		::memcpy(&refDest[0], &pSrc[tPos], tDestSize);
		tPos += tDestSize;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	inline static ST_IMAGE_SECTION_HEADER* GetResourceSection(std::vector<ST_IMAGE_SECTION_HEADER>& vecDest, const ST_IMAGE_DATA_DIRECTORY& stResourceDirInfo)
	{
		size_t i;
		for (i = 0; i < vecDest.size(); i++)
		{
			if (stResourceDirInfo.VirtualAddress != vecDest[i].VirtualAddress)
				continue;

			return &vecDest[i];
		}
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	bool CPEParser::Parse(void)
	{
		try
		{
			size_t tPos = 0;
			if (!CopyTo(m_DosHeader, m_pFileContents, m_tFileSize, tPos))
				throw exception_format("IMAGE_DOS_HEADER parsing failure");

			if ('M' != m_DosHeader.e_magic[0] || 'Z' != m_DosHeader.e_magic[1])
				throw exception_format("File header is not MZ");

			tPos = m_DosHeader.e_lfanew;
			if (!CopyTo(m_ImageNTHeader, m_pFileContents, m_tFileSize, tPos))
				throw exception_format("IMAGE_NT_HEADERS32 parsing failure");

			if ('P' != m_ImageNTHeader.Signature[0] || 'E' != m_ImageNTHeader.Signature[1]
			|| 0 != m_ImageNTHeader.Signature[2] || 0 != m_ImageNTHeader.Signature[3])
				throw exception_format("File DosHeader is not PE");

			if (Is32Bit())
			{
				if (sizeof(ST_IMAGE_OPTIONAL_HEADER32) != m_ImageNTHeader.FileHeader.SizeOfOptionalHeader)
					throw exception_format("IMAGE_OPTIONAL_HEADER32 size is not matched, actual:%u, expected:%u", m_ImageNTHeader.FileHeader.SizeOfOptionalHeader, sizeof(m_ImageOptionalHeader32));

				if (!CopyTo(m_ImageOptionalHeader32, m_pFileContents, m_tFileSize, tPos))
					throw exception_format("IMAGE_OPTIONAL_HEADER32 parsing failure");

				if (0x010B != m_ImageOptionalHeader32.Magic)
					throw exception_format("m_ImageOptionalHeader32 magic:%02X is not %02X", m_ImageOptionalHeader32.Magic, 0x010B);
			}
			else if (Is64Bit())
			{
				if (sizeof(ST_IMAGE_OPTIONAL_HEADER64) != m_ImageNTHeader.FileHeader.SizeOfOptionalHeader)
					throw exception_format("IMAGE_OPTIONAL_HEADER32 size is not matched, actual:%u, expected:%u", m_ImageNTHeader.FileHeader.SizeOfOptionalHeader, sizeof(m_ImageOptionalHeader64));

				if (!CopyTo(m_ImageOptionalHeader64, m_pFileContents, m_tFileSize, tPos))
					throw exception_format("IMAGE_OPTIONAL_HEADER64 parsing failure");

				if (0x020B != m_ImageOptionalHeader64.Magic)
					throw exception_format("IMAGE_OPTIONAL_HEADER64 magic:%02X is not %02X", m_ImageOptionalHeader64.Magic, 0x020B);
			}

			if (0 == m_ImageNTHeader.FileHeader.NumberOfSections)
				throw exception_format("ImageNTHeader32.FileHeader.NumberOfSections is ZERO");

			m_vecImageSectionHeader.resize(m_ImageNTHeader.FileHeader.NumberOfSections);
			size_t i;
			for (i = 0; i < m_ImageNTHeader.FileHeader.NumberOfSections; i++)
			{
				ST_IMAGE_SECTION_HEADER stImageSectionHeader;
				if (!CopyTo(stImageSectionHeader, m_pFileContents, m_tFileSize, tPos))
					throw exception_format("IMAGE_SECTION_HEADER parsing failure");

				if (".rsrc" == std::string(stImageSectionHeader.Name))
				{
					if (!ParseResourceSection(stImageSectionHeader))
						throw exception_format("ResourceSection parsing failure");
				}

				if (".rdata" == std::string(stImageSectionHeader.Name))
				{
					if (!ParseRDataSection(stImageSectionHeader))
						throw exception_format("ResourceSection parsing failure");
				}

				m_vecImageSectionHeader[i] = stImageSectionHeader;
			}
		}
		catch (std::exception& e)
		{
			Log_Debug("%s - %s", __FUNCTION__, e.what());
			return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	inline static bool ParseEntry(LPCBYTE pSrc, size_t tSrcSize, size_t& tPos, ST_RESOURCE_ENTRY& outEntry)
	{
		if (!CopyTo(outEntry.dir, pSrc, tSrcSize, tPos))
			return false;

		size_t i;
		for (i = 0; i < outEntry.dir.NumberOfNamedEntries; i++)
		{
			ST_IMAGE_RESOURCE_DIRECTORY_ENTRY stEntry;
			if (!CopyTo(stEntry, pSrc, tSrcSize, tPos))
				return false;
			outEntry.Entries.push_back(stEntry);
		}
		for (i = 0; i < outEntry.dir.NumberOfIdEntries; i++)
		{
			ST_IMAGE_RESOURCE_DIRECTORY_ENTRY stEntry;
			if (!CopyTo(stEntry, pSrc, tSrcSize, tPos))
				return false;
			outEntry.Entries.push_back(stEntry);
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	inline static std::string ParseString(LPCBYTE pSrc, size_t tSrcSize, size_t& tPos)
	{
		std::string strResult;

		do
		{
			WORD wLength = 0;
			if (!CopyTo(wLength, pSrc, tSrcSize, tPos))
				break;

			if (0 == wLength)
				break;

			std::vector<WORD> vecUnicode;
			vecUnicode.resize(wLength);
			if (!CopyTo(vecUnicode, pSrc, tSrcSize, tPos))
				break;

			std::wstring strResultW;
			strResultW.resize(wLength);
			size_t i;
			for (i = 0; i < wLength; i++)
				((LPWSTR)strResultW.c_str())[i] = vecUnicode[i];

			strResult = MBSFromWCS(strResultW);
		} while (0);

		return strResult;
	}

	//////////////////////////////////////////////////////////////////////////
	bool CPEParser::ParseRDataSection(const ST_IMAGE_SECTION_HEADER& section)
	{
		try
		{
			ST_IMAGE_DATA_DIRECTORY stDataDirectory = m_ImageOptionalHeader32.DataDirectory[CORE_IMAGE_DIRECTORY_ENTRY_DEBUG];

			const size_t tCount = stDataDirectory.Size / sizeof(ST_IMAGE_DEBUG_DIRECTORY);

			size_t i;
			for (i = 0; i < tCount; i++)
			{
				size_t tReadPos = stDataDirectory.VirtualAddress - section.VirtualAddress + section.PointerToRawData + sizeof(ST_IMAGE_DEBUG_DIRECTORY) * i;

				ST_IMAGE_DEBUG_DIRECTORY stImageDebugDirectory = { 0, };
				if (!CopyTo(stImageDebugDirectory, m_pFileContents, m_tFileSize, tReadPos))
					throw exception_format("Reading IMAGE_DEBUG_DIRECTORY has failed.");

				if (CORE_IMAGE_DEBUG_TYPE_CODEVIEW == stImageDebugDirectory.Type)
				{
					tReadPos = stImageDebugDirectory.PointerToRawData;
					ST_IMAGE_DEBUG_PDB_INFO stPDBInfo = { 0, };
					if (!CopyTo(&stPDBInfo, stImageDebugDirectory.SizeOfData, m_pFileContents, m_tFileSize, tReadPos))
						throw exception_format("Reading IMAGE_DEBUG_DIRECTORY has failed.");

					//if (0 != memcmp(stPDBInfo.Signature, "RSDS", 4))
					//	throw exception_format("Invalid PDB info.");
				}
			}
		}
		catch (std::exception& e)
		{
			Log_Debug("%s", e.what());
			return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////	
	struct ST_PE_RESOURCE_DATA
	{
		DWORD Type;
		DWORD NameID;
		DWORD Language;
		std::vector<BYTE> Data;
	};

	//////////////////////////////////////////////////////////////////////////
	bool CPEParser::ParseResourceSection(const ST_IMAGE_SECTION_HEADER& section)
	{
		try
		{
			size_t tPos = section.PointerToRawData;
			if (!ParseEntry(m_pFileContents, m_tFileSize, tPos, m_ResourceTypeDirectory))
				throw exception_format("Resource Type entry parsing failure");

			std::vector<ST_RESOURCE_ENTRY> vecNameID;
			vecNameID.resize(m_ResourceTypeDirectory.Entries.size());
			size_t i;
			for (i = 0; i < m_ResourceTypeDirectory.Entries.size(); i++)
			{
				// If DataIsDirector is ZERO, there is no DIRECTORY. so skip in this case
				if (0 == m_ResourceTypeDirectory.Entries[i].DataIsDirectory)
					continue;

				tPos = section.PointerToRawData + m_ResourceTypeDirectory.Entries[i].OffsetToDirectory;
				if (!ParseEntry(m_pFileContents, m_tFileSize, tPos, vecNameID[i]))
					throw exception_format("Resource Name entry parsing failure");
			}

			std::vector<ST_RESOURCE_ENTRY> vecLanID;
			for (i = 0; i < vecNameID.size(); i++)
			{
				size_t j;
				for (j = 0; j < vecNameID[i].Entries.size(); j++)
				{
					tPos = section.PointerToRawData + vecNameID[i].Entries[j].OffsetToDirectory;

					ST_RESOURCE_ENTRY stLanguage;
					if (!ParseEntry(m_pFileContents, m_tFileSize, tPos, stLanguage))
						throw exception_format("Resource Language entry parsing failure");
					vecLanID.push_back(stLanguage);
				}
			}
		}
		catch (std::exception& e)
		{
			Log_Debug("%s - %s", __FUNCTION__, e.what());
			return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool CPEParser::Is64Bit(void)
	{
		return (CORE_IMAGE_FILE_MACHINE_AMD64 == m_ImageNTHeader.FileHeader.Machine)
			|| (CORE_IMAGE_FILE_MACHINE_IA64 == m_ImageNTHeader.FileHeader.Machine);
	}

	//////////////////////////////////////////////////////////////////////////
	bool CPEParser::Is32Bit(void)
	{
		return (CORE_IMAGE_FILE_MACHINE_I386 == m_ImageNTHeader.FileHeader.Machine);
	}

	//////////////////////////////////////////////////////////////////////////
	E_PE_TYPE CPEParser::GetPEType(void)
	{
		if (CORE_IMAGE_FILE_DLL & m_ImageNTHeader.FileHeader.Characteristics)
			return PE_TYPE_DLL;

		if (Is32Bit())
		{
			if (CORE_IMAGE_SUBSYSTEM_NATIVE == m_ImageOptionalHeader32.Subsystem)
				return PE_TYPE_SYS;
		}
		else
		{
			if (CORE_IMAGE_SUBSYSTEM_NATIVE == m_ImageOptionalHeader64.Subsystem)
				return PE_TYPE_SYS;
		}

		if (CORE_IMAGE_FILE_EXECUTABLE_IMAGE & m_ImageNTHeader.FileHeader.Characteristics)
			return PE_TYPE_EXE;

		return PE_TYPE_UNDEFINED;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CPEParser::GetFileSize(void)
	{
		size_t tFileSize = m_ImageOptionalHeader32.SizeOfHeaders;
		if (Is64Bit())
			tFileSize = m_ImageOptionalHeader64.SizeOfHeaders;

		size_t i;
		for (i = 0; i < m_vecImageSectionHeader.size(); i++)
			tFileSize += m_vecImageSectionHeader[i].SizeOfRawData;

		// Certificate size
		if (Is32Bit())
			tFileSize += m_ImageOptionalHeader32.DataDirectory[CORE_IMAGE_DIRECTORY_ENTRY_SECURITY].Size;
		if (Is64Bit())
			tFileSize += m_ImageOptionalHeader64.DataDirectory[CORE_IMAGE_DIRECTORY_ENTRY_SECURITY].Size;
		return tFileSize;
	}

	//////////////////////////////////////////////////////////////////////////
	struct CORE_VS_VERSIONINFO
	{
		WORD Length;
		WORD Type;
		WORD ValueLength;
		WORD Name[0x11];
	};

	//////////////////////////////////////////////////////////////////////////	
	struct CORE_VS_FIXEDFILEINFO
	{
		DWORD   dwSignature;            /* e.g. 0xfeef04bd */
		DWORD   dwStrucVersion;         /* e.g. 0x00000042 = "0.42" */
		DWORD   dwFileVersionMS;        /* e.g. 0x00030075 = "3.75" */
		DWORD   dwFileVersionLS;        /* e.g. 0x00000031 = "0.31" */
		DWORD   dwProductVersionMS;     /* e.g. 0x00030010 = "3.10" */
		DWORD   dwProductVersionLS;     /* e.g. 0x00000031 = "0.31" */
		DWORD   dwFileFlagsMask;        /* = 0x3F for version "0.42" */
		DWORD   dwFileFlags;            /* e.g. VFF_DEBUG | VFF_PRERELEASE */
		DWORD   dwFileOS;               /* e.g. VOS_DOS_WINDOWS16 */
		DWORD   dwFileType;             /* e.g. VFT_DRIVER */
		DWORD   dwFileSubtype;          /* e.g. VFT2_DRV_KEYBOARD */
		DWORD   dwFileDateMS;           /* e.g. 0 */
		DWORD   dwFileDateLS;           /* e.g. 0 */
	};

	//////////////////////////////////////////////////////////////////////////	
	ECODE CPEParser::GetFileVersion(ST_VERSIONINFO& outVersion)
	{
		ECODE nRet = EC_SUCCESS;
		try
		{
			std::vector<ST_RESOURCE_ID> vecResourceID;
			nRet = GetResourceID(CORE_RT_VERSION, vecResourceID);
			if (EC_SUCCESS != nRet)
				throw exception_format("Retrieving version resource ID failure.");

			std::vector<BYTE> vecVersionRes;
			nRet = GetResource(CORE_RT_VERSION, vecResourceID[0], vecVersionRes);
			if (EC_SUCCESS != nRet)
				throw exception_format("Retrieving version resource failure.");

			size_t tPos = 0;
			//CORE_VS_VERSIONINFO* pVersionInfo = (CORE_VS_VERSIONINFO*)&vecVersionRes[tPos];
			tPos += sizeof(CORE_VS_VERSIONINFO);

			CORE_VS_FIXEDFILEINFO* pFixedFileInfo = (CORE_VS_FIXEDFILEINFO*)&vecVersionRes[tPos];
			tPos += sizeof(CORE_VS_FIXEDFILEINFO);

			outVersion.dwMajor = (pFixedFileInfo->dwFileVersionMS >> 16) & 0xFFFF;
			outVersion.dwMinor = (pFixedFileInfo->dwFileVersionMS >> 0) & 0xFFFF;
			outVersion.dwPatch = (pFixedFileInfo->dwFileVersionLS >> 16) & 0xFFFF;
			outVersion.dwBuild = (pFixedFileInfo->dwFileVersionLS >> 0) & 0xFFFF;
		}
		catch (std::exception& e)
		{
			Log_Debug("%s", e.what());
			return nRet;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////	
	ECODE CPEParser::GetProductVersion(ST_VERSIONINFO& outVersion)
	{
		ECODE nRet = EC_SUCCESS;
		try
		{
			std::vector<ST_RESOURCE_ID> vecResourceID;
			nRet = GetResourceID(CORE_RT_VERSION, vecResourceID);
			if (EC_SUCCESS != nRet)
				throw exception_format("Retrieving version resource ID failure.");

			std::vector<BYTE> vecVersionRes;
			nRet = GetResource(CORE_RT_VERSION, vecResourceID[0], vecVersionRes);
			if (EC_SUCCESS != nRet)
				throw exception_format("Retrieving version resource failure.");

			size_t tPos = 0;
			//CORE_VS_VERSIONINFO* pVersionInfo = (CORE_VS_VERSIONINFO*)&vecVersionRes[tPos];
			tPos += sizeof(CORE_VS_VERSIONINFO);

			CORE_VS_FIXEDFILEINFO* pFixedFileInfo = (CORE_VS_FIXEDFILEINFO*)&vecVersionRes[tPos];
			tPos += sizeof(CORE_VS_FIXEDFILEINFO);

			outVersion.dwMajor = (pFixedFileInfo->dwProductVersionMS >> 16) & 0xFFFF;
			outVersion.dwMinor = (pFixedFileInfo->dwProductVersionMS >> 0) & 0xFFFF;
			outVersion.dwPatch = (pFixedFileInfo->dwProductVersionLS >> 16) & 0xFFFF;
			outVersion.dwBuild = (pFixedFileInfo->dwProductVersionLS >> 0) & 0xFFFF;
		}
		catch (std::exception& e)
		{
			Log_Debug("%s", e.what());
			return nRet;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CPEParser::GetResourceID(E_RESOURCE_TYPE nType, std::vector<ST_RESOURCE_ID>& outID)
	{
		outID.clear();

		try
		{
			ST_IMAGE_SECTION_HEADER* pResourceSection = NULL;
			if (Is32Bit())
				pResourceSection = GetResourceSection(m_vecImageSectionHeader, m_ImageOptionalHeader32.DataDirectory[CORE_IMAGE_DIRECTORY_ENTRY_RESOURCE]);
			if (Is64Bit())
				pResourceSection = GetResourceSection(m_vecImageSectionHeader, m_ImageOptionalHeader64.DataDirectory[CORE_IMAGE_DIRECTORY_ENTRY_RESOURCE]);
			if (NULL == pResourceSection)
				return EC_NO_DATA;

			ST_RESOURCE_ENTRY stNameID;

			size_t i;
			for (i = 0; i < m_ResourceTypeDirectory.Entries.size(); i++)
			{
				if (nType != m_ResourceTypeDirectory.Entries[i].Id)
					continue;

				size_t tPos = pResourceSection->PointerToRawData + m_ResourceTypeDirectory.Entries[i].OffsetToDirectory;
				if (!ParseEntry(m_pFileContents, m_tFileSize, tPos, stNameID))
					throw exception_format("Resource Name entry parsing failure");

				break;
			}

			if(stNameID.Entries.empty())
				throw exception_format("Resource(id:%d) is not found.", nType);

			outID.resize(stNameID.Entries.size());

			for (i = 0; i < stNameID.Entries.size(); i++)
				_Convert(pResourceSection->PointerToRawData, stNameID.Entries[i], outID[i]);
		}
		catch (std::exception& e)
		{
			Log_Debug("%s - %s", __FUNCTION__, e.what());
			return EC_INVALID_DATA;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CPEParser::GetResource(E_RESOURCE_TYPE nType, DWORD dwID, std::vector<BYTE>& outData)
	{
		ST_RESOURCE_ID resID;
		resID.dwID = dwID;
		return GetResource(nType, resID, outData);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CPEParser::GetResource(E_RESOURCE_TYPE nType, const ST_RESOURCE_ID& resID, std::vector<BYTE>& outData)
	{
		try
		{
			ST_IMAGE_SECTION_HEADER* pResourceSection = NULL;
			if (Is32Bit())
				pResourceSection = GetResourceSection(m_vecImageSectionHeader, m_ImageOptionalHeader32.DataDirectory[CORE_IMAGE_DIRECTORY_ENTRY_RESOURCE]);
			if (Is64Bit())
				pResourceSection = GetResourceSection(m_vecImageSectionHeader, m_ImageOptionalHeader64.DataDirectory[CORE_IMAGE_DIRECTORY_ENTRY_RESOURCE]);
			if (NULL == pResourceSection)
				return EC_NO_DATA;

			ST_RESOURCE_ENTRY stNameID;

			size_t i;
			for (i = 0; i < m_ResourceTypeDirectory.Entries.size(); i++)
			{
				if (nType != m_ResourceTypeDirectory.Entries[i].Id)
					continue;

				size_t tPos = pResourceSection->PointerToRawData + m_ResourceTypeDirectory.Entries[i].OffsetToDirectory;
				if (!ParseEntry(m_pFileContents, m_tFileSize, tPos, stNameID))
					throw exception_format("Resource Name entry parsing failure");

				break;
			}

			int nIndex = -1;
			for (i = 0; i < stNameID.Entries.size(); i++)
			{
				ST_RESOURCE_ID stID;
				_Convert(pResourceSection->PointerToRawData, stNameID.Entries[i], stID);

				if (stID != resID)
					continue;

				nIndex = (int)i;
				break;
			}

			if (-1 == nIndex)
				throw exception_format("Matched resourceID is not found, type:%u, id:%u, named:%s", nType, resID.dwID, resID.strNamedID.c_str());

			ST_RESOURCE_ENTRY stLanguage;
			{
				size_t tPos = pResourceSection->PointerToRawData + stNameID.Entries[nIndex].OffsetToDirectory;
				if (!ParseEntry(m_pFileContents, m_tFileSize, tPos, stLanguage))
					throw exception_format("Resource entry parsing failiure");
			}

			ST_IMAGE_RESOURCE_DATA_ENTRY stDataEntry;
			{
				size_t tPos = pResourceSection->PointerToRawData + stLanguage.Entries[0].OffsetToDirectory;
				if (!CopyTo(stDataEntry, m_pFileContents, m_tFileSize, tPos))
					throw exception_format("IMAGE_RESOURCE parsing failure");
			}

			{
				size_t tPos = pResourceSection->PointerToRawData + stDataEntry.OffsetToData - pResourceSection->VirtualAddress;
				outData.resize(stDataEntry.Size);
				if (!CopyTo(outData, m_pFileContents, m_tFileSize, tPos))
					throw exception_format("IMAGE_RESOURCE parsing failure");
			}
		}
		catch (std::exception& e)
		{
			Log_Debug("%s - %s", __FUNCTION__, e.what());
			return EC_INVALID_DATA;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CPEParser::GetGroupIcon(const ST_RESOURCE_ID& resID, std::vector<ST_RESOURCE_GROUPICON_ENTRY>& vecIcons)
	{
		ECODE nRet = EC_SUCCESS;

		try
		{
			std::vector<BYTE> vecContext;
			nRet = GetResource(CORE_RT_GROUP_ICON, resID, vecContext);
			if (EC_SUCCESS != nRet)
				return nRet;

			size_t tPos = 0;
			ST_RESOURCE_GROUPICON_DIR stDir = { 0, };
			if (!CopyTo(stDir, &vecContext[0], vecContext.size(), tPos))
				throw exception_format("GroupIcon DIR parsing failure, id:%u, named:%s", resID.dwID, resID.strNamedID.c_str());

			std::vector<ST_RESOURCE_GROUPICON_ENTRY> vecIconEntry;
			size_t i;
			for (i = 0; i < stDir.NumOfImages; i++)
			{
				ST_RESOURCE_GROUPICON_ENTRY stEntry;
				if (!CopyTo(stEntry, &vecContext[0], vecContext.size(), tPos))
					throw exception_format("GroupIcon ENTRY parsing failure, %u of %u", i + 1, stDir.NumOfImages);

				vecIcons.push_back(stEntry);
			}
		}
		catch (std::exception& e)
		{
			Log_Debug("%s - %s", __FUNCTION__, e.what());
			return EC_INVALID_DATA;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CPEParser::GetDefaultGroupIcon(std::vector<ST_RESOURCE_GROUPICON_ENTRY>& vecIcons)
	{
		ECODE nRet = EC_SUCCESS;

		try
		{
			std::vector<ST_RESOURCE_ID> vecGroupIconID;
			nRet = GetResourceID(CORE_RT_GROUP_ICON, vecGroupIconID);
			if (EC_SUCCESS != nRet)
				throw exception_format("GetResourceID calling failure, %u", nRet);

			nRet = EC_NO_DATA;
			if (vecGroupIconID.empty())
			{
				Log_Debug("There is no GroupIcon");
				return nRet;
			}

			std::vector<BYTE> vecContext;
			nRet = GetResource(CORE_RT_GROUP_ICON, vecGroupIconID[0], vecContext);
			if (EC_SUCCESS != nRet)
				throw exception_format("GetResource calling failure, %u", nRet);

			size_t tPos = 0;
			ST_RESOURCE_GROUPICON_DIR stDir = { 0, };
			if (!CopyTo(stDir, &vecContext[0], vecContext.size(), tPos))
				throw exception_format("GroupIcon DIR copying failure, id:%u", vecGroupIconID[0].dwID);

			size_t i;
			for (i = 0; i < stDir.NumOfImages; i++)
			{
				ST_RESOURCE_GROUPICON_ENTRY stEntry;
				if (!CopyTo(stEntry, &vecContext[0], vecContext.size(), tPos))
					throw exception_format("GroupIcon ENTRY copying failure, %u of %u", i + 1, stDir.NumOfImages);

				vecIcons.push_back(stEntry);
			}
		}
		catch (std::exception& e)
		{
			Log_Debug("%s - %s", __FUNCTION__, e.what());
			return EC_INVALID_DATA;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CPEParser::GetData(core::E_IMAGE_NUMBEROF_DIRECTORY_ENTRIES nType, std::vector<BYTE>& vecData)
	{
		ST_IMAGE_DATA_DIRECTORY stDataDirectory = m_ImageOptionalHeader32.DataDirectory[nType];
		if (Is64Bit())
			stDataDirectory = m_ImageOptionalHeader64.DataDirectory[nType];

		if (0 == stDataDirectory.Size)
			return EC_NO_DATA;

		vecData.resize(stDataDirectory.Size);
		memcpy(&vecData[0], &m_pFileContents[stDataDirectory.VirtualAddress], stDataDirectory.Size);
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CPEParser::SaveIconToFile(const ST_RESOURCE_GROUPICON_ENTRY& stIcon, LPCWSTR pszFileName)
	{
		std::string strFileNameA = MBSFromWCS(pszFileName);
		return SaveIconToFile(stIcon, strFileNameA.c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CPEParser::GetCodeData(ST_SECTION_DATA& data)
	{
		ECODE nRet = EC_SUCCESS;
		try
		{
			DWORD dwEntryPoint;
			E_PE_TYPE nType = GetPEType();
			if (PE_TYPE_EXE == nType)
			{
				if (Is32Bit())
					dwEntryPoint = m_ImageOptionalHeader32.AddressOfEntryPoint;
				else
					dwEntryPoint = m_ImageOptionalHeader64.AddressOfEntryPoint;
			}
			else if (PE_TYPE_DLL == nType)
			{
				if (Is32Bit())
					dwEntryPoint = m_ImageOptionalHeader32.BaseOfCode;
				else
					dwEntryPoint = m_ImageOptionalHeader64.BaseOfCode;
			}
			else
			{
				nRet = EC_NOT_SUPPORTED;
				throw exception_format("Code section is not found.");
			}

			size_t i;
			for (i = 0; i < m_vecImageSectionHeader.size(); i++)
			{
				DWORD dwStartAddr = m_vecImageSectionHeader[i].VirtualAddress;
				DWORD dwEndAddr = dwStartAddr + m_vecImageSectionHeader[i].Misc.VirtualSize;

				if (dwStartAddr > dwEntryPoint)
					continue;

				if (dwEndAddr <= dwEntryPoint)
					continue;

				data.dwFileAddress = dwEntryPoint
					- m_vecImageSectionHeader[i].VirtualAddress
					+ m_vecImageSectionHeader[i].PointerToRawData;
				data.pData = m_pFileContents + data.dwFileAddress;
				data.tSize = m_vecImageSectionHeader[i].Misc.VirtualSize;
				return EC_SUCCESS;
			}

			throw exception_format("Code section is not found.");
		}
		catch (std::exception& e)
		{
			Log_Debug("%s", e.what());
			return nRet;
		}
		
		return EC_INVALID_DATA;
	}

	//////////////////////////////////////////////////////////////////////////
	bool CheckAddressInRange(DWORD dwAddress, DWORD dwStart, DWORD dwEnd)
	{
		return (dwStart <= dwAddress && dwAddress <= dwEnd);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CPEParser::GetExportFunction(std::vector<ST_EXPORT_FUNC>& vecExportFunctions)
	{
		size_t i;
		try
		{
			const ST_IMAGE_DATA_DIRECTORY* pExportEntry = NULL;
			if (Is32Bit())
				pExportEntry = &m_ImageOptionalHeader32.DataDirectory[CORE_IMAGE_DIRECTORY_ENTRY_EXPORT];
			else
				pExportEntry = &m_ImageOptionalHeader64.DataDirectory[CORE_IMAGE_DIRECTORY_ENTRY_EXPORT];

			if (NULL == pExportEntry->VirtualAddress)
				return EC_NO_DATA;

			const ST_IMAGE_SECTION_HEADER* pSectionHeader = GetImageSectionHeader(pExportEntry->VirtualAddress);
			DWORD dwExportTableAddr = pExportEntry->VirtualAddress - pSectionHeader->VirtualAddress + pSectionHeader->PointerToRawData;
			if (dwExportTableAddr >= m_tFileSize)
				throw exception_format(TEXT("Invalid ExportTableAddress(%p)."), dwExportTableAddr);

			const ST_IMAGE_EXPORT_DIRECTORY* pExportDir = (ST_IMAGE_EXPORT_DIRECTORY*)(m_pFileContents + dwExportTableAddr);
			//std::string name = (const char*)m_pFileContents + pExportDir->Name - pSectionHeader->VirtualAddress + pSectionHeader->PointerToRawData;
			DWORD dwFuncAddress = pExportDir->AddressOfFunctions - pSectionHeader->VirtualAddress + pSectionHeader->PointerToRawData;
			DWORD dwFuncName = pExportDir->AddressOfNames - pSectionHeader->VirtualAddress + pSectionHeader->PointerToRawData;
			DWORD dwFuncOrdinal = pExportDir->AddressOfNameOrdinals - pSectionHeader->VirtualAddress + pSectionHeader->PointerToRawData;

			DWORD dwEndOfFuncAddress = dwFuncAddress + 4 * pExportDir->NumberOfFunctions;
			if (dwEndOfFuncAddress >= m_tFileSize)
				throw exception_format(TEXT("Invalid dwEndOfFuncAddress(%p)."), dwEndOfFuncAddress);

			std::vector<DWORD> vecFuncAddress;
			vecFuncAddress.resize(pExportDir->NumberOfFunctions);
			for (i = 0; i < pExportDir->NumberOfFunctions; i++)
			{
				vecFuncAddress[i] = *(DWORD*)(m_pFileContents + dwFuncAddress + 4 * i);
				DWORD dwOffset = vecFuncAddress[i] - pSectionHeader->VirtualAddress + pSectionHeader->PointerToRawData;
			}
					

			DWORD dwEndOfFuncName = dwFuncName + 4 * pExportDir->NumberOfNames;
			if (dwEndOfFuncName >= m_tFileSize)
				throw exception_format(TEXT("Invalid dwEndOfFuncName(%p)."), dwEndOfFuncName);

			std::vector<DWORD> vecNamePointer;
			std::vector<WORD> vecOrdinals;
			std::vector<const char*> vecNames;
			vecNamePointer.resize(pExportDir->NumberOfNames);
			vecOrdinals.resize(pExportDir->NumberOfNames);
			for (i = 0; i < pExportDir->NumberOfNames; i++)
			{
				WORD wOrdinal = *(WORD*)(m_pFileContents + dwFuncOrdinal + 2 * i);
				vecOrdinals[i] = wOrdinal;
				vecNamePointer[i] = *(DWORD*)(m_pFileContents + dwFuncName + 4 * i);

				DWORD dwOffset = vecNamePointer[i] - pSectionHeader->VirtualAddress + pSectionHeader->PointerToRawData;
				vecNames.push_back((const char*)(m_pFileContents + dwOffset));

				ST_EXPORT_FUNC stExportFunc;
				stExportFunc.strFuncName = vecNames[i];
				if (CheckAddressInRange(vecFuncAddress[wOrdinal], pExportEntry->VirtualAddress, pExportEntry->VirtualAddress + pExportEntry->Size))
					continue;

				stExportFunc.dwAddress = vecFuncAddress[wOrdinal];
				stExportFunc.wOrdinal = wOrdinal + (WORD)pExportDir->Base;

				vecExportFunctions.push_back(stExportFunc);
			}
		}
		catch (std::exception& e)
		{
			Log_Debug("%s", e.what());
			return EC_INVALID_DATA;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CPEParser::GetImportFunction(std::vector<ST_IMPORT_FUNC>& vecImportFunctions)
	{
		try
		{
			const ST_IMAGE_DATA_DIRECTORY* pImportEntry = NULL;
			if (Is32Bit())
				pImportEntry = &m_ImageOptionalHeader32.DataDirectory[CORE_IMAGE_DIRECTORY_ENTRY_IMPORT];
			else
				pImportEntry = &m_ImageOptionalHeader64.DataDirectory[CORE_IMAGE_DIRECTORY_ENTRY_IMPORT];

			if (NULL == pImportEntry->VirtualAddress)
				return EC_NO_DATA;

			const ST_IMAGE_SECTION_HEADER* pSectionHeader = GetImageSectionHeader(pImportEntry->VirtualAddress);
			DWORD dwImportTableAddr = pImportEntry->VirtualAddress - pSectionHeader->VirtualAddress + pSectionHeader->PointerToRawData;
			if (dwImportTableAddr >= m_tFileSize)
				throw exception_format(TEXT("Invalid ImportTableAddress(%p)."), dwImportTableAddr);

			const ST_IMAGE_IMPORT_DESCRIPTOR* pImportDesc;

			size_t i;
			for (i = 0; true; i++)
			{
				DWORD dwEndAddr = (DWORD)dwImportTableAddr + (i+1) * sizeof(ST_IMAGE_IMPORT_DESCRIPTOR);
				if (dwEndAddr >= m_tFileSize)
					throw exception_format(TEXT("Invalid IMAGE_IMPORT_DESCRIPTOR(%p)."), dwEndAddr);

				pImportDesc = (ST_IMAGE_IMPORT_DESCRIPTOR*)(m_pFileContents + dwImportTableAddr + i * sizeof(ST_IMAGE_IMPORT_DESCRIPTOR));
				if (0 == pImportDesc->FirstThunk)
					break;

				const ST_IMAGE_SECTION_HEADER* pIATSectionHeader = GetImageSectionHeader(pImportDesc->OriginalFirstThunk);
				DWORD dwImportTableAddr = pImportEntry->VirtualAddress - pIATSectionHeader->VirtualAddress + pIATSectionHeader->PointerToRawData;
				if (dwImportTableAddr >= m_tFileSize)
					throw exception_format(TEXT("Invalid ImportTableAddress(%p)."), dwImportTableAddr);

				// IAT: Import Address Table, INT: Import Name Table
				DWORD dwName = pImportDesc->Name - pIATSectionHeader->VirtualAddress + pIATSectionHeader->PointerToRawData;
				DWORD dwINT  = pImportDesc->OriginalFirstThunk - pIATSectionHeader->VirtualAddress + pIATSectionHeader->PointerToRawData;
				DWORD dwIAT  = pImportDesc->FirstThunk - pIATSectionHeader->VirtualAddress + pIATSectionHeader->PointerToRawData;

				LPCSTR pszName = (LPCSTR)(m_pFileContents + dwName);

				size_t j;
				for (j = 0; true; j++)
				{
					size_t tOffset = j * 4;
					ST_IMAGE_THUNK_DATA32* pINTThunk = (ST_IMAGE_THUNK_DATA32*)(m_pFileContents + dwINT + tOffset);
					ST_IMAGE_THUNK_DATA32* pIATThunk = (ST_IMAGE_THUNK_DATA32*)(m_pFileContents + dwIAT + tOffset);

					if (0 == pINTThunk->u1.AddressOfData)
						break;

					if (0x80000000 & pINTThunk->u1.AddressOfData)		// Ordinal
					{
						ST_IMPORT_FUNC stImportFunc;
						stImportFunc.strModuleName = pszName;
						stImportFunc.strFuncName = "";
						stImportFunc.wOrdinal = pINTThunk->u1.AddressOfData & 0x0000FFFF;
						vecImportFunctions.push_back(stImportFunc);
					}
					else
					{
						ST_IMAGE_IMPORT_BY_NAME* pImport = (ST_IMAGE_IMPORT_BY_NAME*)
							(m_pFileContents + pINTThunk->u1.AddressOfData + pIATSectionHeader->PointerToRawData - pIATSectionHeader->VirtualAddress);

						ST_IMPORT_FUNC stImportFunc;
						stImportFunc.strModuleName = pszName;
						stImportFunc.strFuncName = (LPCSTR)pImport->Name;
						stImportFunc.wOrdinal = pImport->Hint;
						vecImportFunctions.push_back(stImportFunc);
					}
				}
			}
		}
		catch (std::exception& e)
		{
			Log_Debug("%s", e.what());
			return EC_INVALID_DATA;
		}
		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE CPEParser::SaveIconToFile(const ST_RESOURCE_GROUPICON_ENTRY& stIcon, LPCSTR pszFileName)
	{
		ECODE nRet = EC_SUCCESS;
		FILE* pFile = NULL;

		try
		{
			std::vector<BYTE> vecResource;
			if (EC_SUCCESS != GetResource(CORE_RT_ICON, stIcon.ResourceID, vecResource))
				throw exception_format("CORE_RT_ICON ID:%u is not found", stIcon.ResourceID);

			pFile = fopenA(pszFileName, "w+b");
			if (NULL == pFile)
			{
				nRet = GetLastError();
				throw exception_format("Cannot open file %s", pszFileName);
			}

			size_t tPos = 0;
			ST_BMP_INFO_HEADER info = { 0, };
			if (!CopyTo(info, &vecResource[0], vecResource.size(), tPos))
			{
				nRet = EC_INVALID_DATA;
				throw exception_format("Cannot read ST_BMP_INFO_HEADER Resource data, tPos:%u", tPos);
			}

			if (40 != info.Size)
			{
				fwrite(&vecResource[0], vecResource.size(), 1, pFile);
				fclose(pFile);
			}
			else
			{
				info.Height /= 2;

				DWORD dwPaletteSize = info.BitCount > 8 ? 0 : (DWORD)::pow(2.0, info.BitCount);

				ST_BMP_HEADER header;
				header.MAGIC[0] = 'B';
				header.MAGIC[1] = 'M';
				header.Tagging1 = header.Tagging2 = 0;
				header.TotalFileSize = (DWORD)(sizeof(ST_BMP_HEADER) + vecResource.size());
				header.RawDataPoint = (DWORD)(sizeof(ST_BMP_HEADER) + sizeof(ST_BMP_INFO_HEADER) + dwPaletteSize);
				fwrite(&header, sizeof(header), 1, pFile);
				fwrite(&info, sizeof(info), 1, pFile);
				fwrite(&vecResource[sizeof(info)], vecResource.size() - sizeof(info), 1, pFile);
				fclose(pFile);
			}	while (0);
		}
		catch (std::exception& e)
		{
			Log_Debug("%s - %s", __FUNCTION__, e.what());
			if (pFile)
				fclose(pFile);
			return nRet;
		}

		return EC_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	void CPEParser::_Convert(size_t tOffset, const ST_IMAGE_RESOURCE_DIRECTORY_ENTRY& entry, ST_RESOURCE_ID& outID)
	{
		if (entry.NameIsString)
		{
			size_t tPos = tOffset + entry.NameOffset;
			outID.dwID = 0xFFFFFFFF;
			outID.strNamedID = ParseString(m_pFileContents, m_tFileSize, tPos);
		}
		else
		{
			outID.dwID = entry.Id;
			outID.strNamedID = "";
		}
	}

	//////////////////////////////////////////////////////////////////////////
	const ST_IMAGE_SECTION_HEADER * CPEParser::GetImageSectionHeader(DWORD dwVirtualAddr)
	{
		size_t i;
		for (i = 0; i < m_vecImageSectionHeader.size(); i++)
		{
			if (dwVirtualAddr < m_vecImageSectionHeader[i].VirtualAddress)
				continue;

			DWORD dwEndAddr = m_vecImageSectionHeader[i].VirtualAddress + m_vecImageSectionHeader[i].SizeOfRawData;
			if (dwEndAddr  <= dwVirtualAddr )
				continue;

			return &m_vecImageSectionHeader[i];
		}
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	const ST_IMAGE_DOS_HEADER& CPEParser::GetImageDosHeader(void)
	{
		return m_DosHeader;
	}

	//////////////////////////////////////////////////////////////////////////
	const ST_IMAGE_NT_HEADERS& CPEParser::GetImageNTHeader(void)
	{
		return m_ImageNTHeader;
	}

	//////////////////////////////////////////////////////////////////////////
	const ST_IMAGE_OPTIONAL_HEADER32& CPEParser::GetImageOptionalHeader32(void)
	{
		return m_ImageOptionalHeader32;
	}

	//////////////////////////////////////////////////////////////////////////
	const ST_IMAGE_OPTIONAL_HEADER64& CPEParser::GetImageOptionalHeader64(void)
	{
		return m_ImageOptionalHeader64;
	}
}
