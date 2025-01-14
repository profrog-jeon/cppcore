#pragma once

#include <vector>
#include "../100_System/100_System.h"
#include "PEParserData.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	struct ST_RESOURCE_ENTRY
	{
		ST_IMAGE_RESOURCE_DIRECTORY dir;
		std::vector<ST_IMAGE_RESOURCE_DIRECTORY_ENTRY> Entries;
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_PE_PARSE_INFO
	{
		size_t tFileSize;
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_RESOURCE_ID
	{
		DWORD dwID;
		std::string strNamedID;		// ID or Named

		bool operator!=(const ST_RESOURCE_ID& left)
		{
			if( strNamedID != left.strNamedID )
				return true;
			if( dwID != left.dwID )
				return true;
			return false;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	enum E_PE_TYPE
	{
		PE_TYPE_UNDEFINED	= 0,
		PE_TYPE_EXE			,
		PE_TYPE_DLL			,
		PE_TYPE_SYS			,
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_SECTION_DATA
	{
		DWORD dwFileAddress;
		LPCBYTE pData;
		size_t tSize;
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_EXPORT_FUNC
	{
		std::string strFuncName;
		DWORD dwAddress;
		WORD wOrdinal;
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_IMPORT_FUNC
	{
		std::string strModuleName;
		std::string strFuncName;
		WORD wOrdinal;
	};

	//////////////////////////////////////////////////////////////////////////
	class CPEParser : public CExeParserSuper
	{
	protected:
		HANDLE		m_hFileMapping;
		LPCBYTE		m_pFileContents;
		size_t		m_tFileSize;

		ST_IMAGE_DOS_HEADER		m_DosHeader;
		ST_IMAGE_NT_HEADERS		m_ImageNTHeader;
		ST_IMAGE_OPTIONAL_HEADER32 m_ImageOptionalHeader32;
		ST_IMAGE_OPTIONAL_HEADER64 m_ImageOptionalHeader64;
		std::vector<ST_IMAGE_SECTION_HEADER> m_vecImageSectionHeader;

		ST_RESOURCE_ENTRY		m_ResourceTypeDirectory;

	public:
					CPEParser(void);
					~CPEParser(void);

		bool		Parse(std::string strFilePath);
		bool		Parse(std::wstring strFilePath);
		bool		Parse(LPCBYTE pContents, size_t tContentsSize);

		ECODE		OpenWorker(HANDLE hFile);
		void		Close(void);

		bool		Is64Bit(void);
		bool		Is32Bit(void);
		E_PE_TYPE	GetPEType(void);

		size_t		GetFileSize(void);
		ECODE		GetFileVersion(ST_VERSIONINFO& outVersion);
		ECODE		GetProductVersion(ST_VERSIONINFO& outVersion);
		ECODE		GetResourceID(E_RESOURCE_TYPE nType, std::vector<ST_RESOURCE_ID>& outID);
		ECODE		GetResource(E_RESOURCE_TYPE nType, DWORD dwID, std::vector<BYTE>& outData);
		ECODE		GetResource(E_RESOURCE_TYPE nType, const ST_RESOURCE_ID& resID, std::vector<BYTE>& outData);
		ECODE		GetGroupIcon(const ST_RESOURCE_ID& resID, std::vector<ST_RESOURCE_GROUPICON_ENTRY>& vecIcons);
		ECODE		GetDefaultGroupIcon(std::vector<ST_RESOURCE_GROUPICON_ENTRY>& vecIcons);
		ECODE		GetData(core::E_IMAGE_NUMBEROF_DIRECTORY_ENTRIES nType, std::vector<BYTE>& vecData);

		ECODE		GetSection(std::string strName, ST_SECTION_DATA& data);
		ECODE		GetSection(std::wstring strName, ST_SECTION_DATA& data);
		ECODE		GetCodeData(ST_SECTION_DATA& data);
		ECODE		GetExportFunction(std::vector<ST_EXPORT_FUNC>& vecExportFunctions);
		ECODE		GetImportFunction(std::vector<ST_IMPORT_FUNC>& vecExportFunctions);

		ECODE		SaveIconToFile(const ST_RESOURCE_GROUPICON_ENTRY& vecIcons, LPCSTR pszFileName);
		ECODE		SaveIconToFile(const ST_RESOURCE_GROUPICON_ENTRY& vecIcons, LPCWSTR pszFileName);

		const ST_IMAGE_DOS_HEADER& GetImageDosHeader(void);
		const ST_IMAGE_NT_HEADERS& GetImageNTHeader(void);

		const ST_IMAGE_OPTIONAL_HEADER32& GetImageOptionalHeader32(void);
		const ST_IMAGE_OPTIONAL_HEADER64& GetImageOptionalHeader64(void);

	private:
		bool		Parse(void);
		bool		ParseRDataSection(const ST_IMAGE_SECTION_HEADER& section);
		bool		ParseResourceSection(const ST_IMAGE_SECTION_HEADER& section);
		void		_Convert(size_t tOffset, const ST_IMAGE_RESOURCE_DIRECTORY_ENTRY& entry, ST_RESOURCE_ID& outID);

		const ST_IMAGE_SECTION_HEADER* GetImageSectionHeader(DWORD dwVirtualAddr);
	};
}
