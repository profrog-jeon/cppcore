#pragma once

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	enum E_IMAGE_SIGNATURE
	{
		CORE_IMAGE_DOS_SIGNATURE                = 0x4D5A,      // MZ
		CORE_IMAGE_OS2_SIGNATURE                = 0x4E45,      // NE
		CORE_IMAGE_OS2_SIGNATURE_LE             = 0x4C45,      // LE
		CORE_IMAGE_NT_SIGNATURE                 = 0x50450000,  // PE00
	};

	//////////////////////////////////////////////////////////////////////////
	enum E_RESOURCE_TYPE
	{
		CORE_RT_CURSOR      	= 1,
		CORE_RT_BITMAP      	= 2,
		CORE_RT_ICON        	= 3,
		CORE_RT_MENU        	= 4,
		CORE_RT_DIALOG      	= 5,
		CORE_RT_STRING      	= 6,
		CORE_RT_FONTDIR     	= 7,
		CORE_RT_FONT        	= 8,
		CORE_RT_ACCELERATOR 	= 9,
		CORE_RT_RCDATA      	= 10,
		CORE_RT_MESSAGETABLE	= 11,
		CORE_RT_GROUP_CURSOR 	= CORE_RT_CURSOR + CORE_RT_MESSAGETABLE,
		CORE_RT_GROUP_ICON   	= CORE_RT_ICON + CORE_RT_MESSAGETABLE,
		CORE_RT_VERSION      	= 16,
		CORE_RT_DLGINCLUDE   	= 17,
		CORE_RT_PLUGPLAY     	= 19,
		CORE_RT_VXD          	= 20,
		CORE_RT_ANICURSOR    	= 21,
		CORE_RT_ANIICON      	= 22,
		CORE_RT_HTML         	= 23,
		CORE_RT_MANIFEST		= 24,
	};

#pragma pack(push, 1)
	//////////////////////////////////////////////////////////////////////////
	//	256x256, 32-bit color, PNG compressed 
	//	256x256, 8-bit color, PNG compressed 
	//	256x256, 4-bit color, PNG compressed 
	//	48x48, 32-bit color, uncompressed 
	//	48x48, 8-bit color, uncompressed 
	//	48x48, 4-bit color, uncompressed 
	//	32x32, 32-bit color, uncompressed 
	//	32x32, 8-bit color, uncompressed 
	//	32x32, 4-bit color, uncompressed 
	//	16x16, 32-bit color, uncompressed 
	//	16x16, 8-bit color, uncompressed 
	//	16x16, 4-bit color, uncompressed 

	//////////////////////////////////////////////////////////////////////////
	struct ST_BMP_HEADER
	{
		char	MAGIC[2];			// BM
		DWORD	TotalFileSize;
		WORD	Tagging1;			// Reserved for image application custom data1
		WORD	Tagging2;			// Reserved for image application custom data2
		DWORD	RawDataPoint;		// 
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_BMP_INFO_HEADER
	{
		DWORD Size;  // Size of InfoHeader =40  
		DWORD Width;  // Bitmap Width  
		DWORD Height;  // Bitmap Height  
		WORD Planes;  // Number of Planes (=1)  

		//1 = monochrome palette. NumColors = 2 
		//4 = 4bit palletized. NumColors = 16 
		//8 = 8bit palletized. NumColors = 256 
		//16 = 16bit RGB. NumColors = 65536 (?) 
		//24 = 24bit RGB. NumColors = 16M  
		WORD BitCount;  // Bits per Pixel 
		
		//0 = BI_RGB no compression 
		//1 = BI_RLE8 8bit RLE encoding 
		//2 = BI_RLE4 4bit RLE encoding  
		DWORD Compression;	// Type of Compression 
		DWORD ImageSize;  // (compressed) Size of Image  

		//It is valid to set this =0 if Compression = 0  
		DWORD XpixelsPerM;  // horizontal resolution: Pixels/meter  
		DWORD YpixelsPerM;  // vertical resolution: Pixels/meter  
		DWORD ColorsUsed;  // Number of actually used colors  
		DWORD ColorsImportant;  // Number of important colors 0 = all  
		// ColorTable : 4 * NumColors bytes  present only if Info.BitsPerPixel <= 8  
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_RESOURCE_GROUPICON_DIR
	{
		WORD Reserved;			// Reserved. Must always be 0.
		WORD ImageType;			// Specifies image type: 1 for icon (.ICO) image, 2 for cursor (.CUR) image. Other values are invalid. 
		WORD NumOfImages;		// Specifies number of images in the file.
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_RESOURCE_GROUPICON_ENTRY
	{
		BYTE Width;				// Specifies image width in pixels. Can be any number between 0 and 255. Value 0 means image width is 256 pixels.
		BYTE Height;			// Specifies image height in pixels. Can be any number between 0 and 255. Value 0 means image height is 256 pixels.
		BYTE NumOfColorPalette;	// Specifies number of colors in the color palette. Should be 0 if the image does not use a color palette.
		BYTE Reserved;			// Reserved. Should be 0.[Notes 2] 
		WORD Planes;			// In ICO format: Specifies color planes. Should be 0 or 1.[Notes 3] 
		WORD BitsPerPixel;		// In ICO format: Specifies bits per pixel. [Notes 4] 
		DWORD ImageSize;		// Specifies the size of the image's data in bytes
		WORD ResourceID;		// ICO resource ID
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_IMAGE_DOS_HEADER {      // DOS .EXE header
		char   e_magic[2];            // Magic number
		WORD   e_cblp;                // Bytes on last page of file
		WORD   e_cp;                  // Pages in file
		WORD   e_crlc;                // Relocations
		WORD   e_cparhdr;             // Size of header in paragraphs
		WORD   e_minalloc;            // Minimum extra paragraphs needed
		WORD   e_maxalloc;            // Maximum extra paragraphs needed
		WORD   e_ss;                  // Initial (relative) SS value
		WORD   e_sp;                  // Initial SP value
		WORD   e_csum;                // Checksum
		WORD   e_ip;                  // Initial IP value
		WORD   e_cs;                  // Initial (relative) CS value
		WORD   e_lfarlc;              // File address of relocation table
		WORD   e_ovno;                // Overlay number
		WORD   e_res[4];              // Reserved words
		WORD   e_oemid;               // OEM identifier (for e_oeminfo)
		WORD   e_oeminfo;             // OEM information; e_oemid specific
		WORD   e_res2[10];            // Reserved words
		DWORD  e_lfanew;              // File address of new exe header
	};

	//////////////////////////////////////////////////////////////////////////
	// Machine
#define CORE_IMAGE_FILE_MACHINE_UNKNOWN           0
#define CORE_IMAGE_FILE_MACHINE_I386              0x014c  // Intel 386.
#define CORE_IMAGE_FILE_MACHINE_R3000             0x0162  // MIPS little-endian, 0x160 big-endian
#define CORE_IMAGE_FILE_MACHINE_R4000             0x0166  // MIPS little-endian
#define CORE_IMAGE_FILE_MACHINE_R10000            0x0168  // MIPS little-endian
#define CORE_IMAGE_FILE_MACHINE_WCEMIPSV2         0x0169  // MIPS little-endian WCE v2
#define CORE_IMAGE_FILE_MACHINE_ALPHA             0x0184  // Alpha_AXP
#define CORE_IMAGE_FILE_MACHINE_SH3               0x01a2  // SH3 little-endian
#define CORE_IMAGE_FILE_MACHINE_SH3DSP            0x01a3
#define CORE_IMAGE_FILE_MACHINE_SH3E              0x01a4  // SH3E little-endian
#define CORE_IMAGE_FILE_MACHINE_SH4               0x01a6  // SH4 little-endian
#define CORE_IMAGE_FILE_MACHINE_SH5               0x01a8  // SH5
#define CORE_IMAGE_FILE_MACHINE_ARM               0x01c0  // ARM Little-Endian
#define CORE_IMAGE_FILE_MACHINE_THUMB             0x01c2
#define CORE_IMAGE_FILE_MACHINE_AM33              0x01d3
#define CORE_IMAGE_FILE_MACHINE_POWERPC           0x01F0  // IBM PowerPC Little-Endian
#define CORE_IMAGE_FILE_MACHINE_POWERPCFP         0x01f1
#define CORE_IMAGE_FILE_MACHINE_IA64              0x0200  // Intel 64
#define CORE_IMAGE_FILE_MACHINE_MIPS16            0x0266  // MIPS
#define CORE_IMAGE_FILE_MACHINE_ALPHA64           0x0284  // ALPHA64
#define CORE_IMAGE_FILE_MACHINE_MIPSFPU           0x0366  // MIPS
#define CORE_IMAGE_FILE_MACHINE_MIPSFPU16         0x0466  // MIPS
#define CORE_IMAGE_FILE_MACHINE_AXP64             IMAGE_FILE_MACHINE_ALPHA64
#define CORE_IMAGE_FILE_MACHINE_TRICORE           0x0520  // Infineon
#define CORE_IMAGE_FILE_MACHINE_CEF               0x0CEF
#define CORE_IMAGE_FILE_MACHINE_EBC               0x0EBC  // EFI Byte Code
#define CORE_IMAGE_FILE_MACHINE_AMD64             0x8664  // AMD64 (K8)
#define CORE_IMAGE_FILE_MACHINE_M32R              0x9041  // M32R little-endian
#define CORE_IMAGE_FILE_MACHINE_CEE               0xC0EE

	//////////////////////////////////////////////////////////////////////////
	// Characteristics
#define CORE_IMAGE_FILE_RELOCS_STRIPPED           0x0001  // Relocation info stripped from file.
#define CORE_IMAGE_FILE_EXECUTABLE_IMAGE          0x0002  // File is executable  (i.e. no unresolved externel references).
#define CORE_IMAGE_FILE_LINE_NUMS_STRIPPED        0x0004  // Line nunbers stripped from file.
#define CORE_IMAGE_FILE_LOCAL_SYMS_STRIPPED       0x0008  // Local symbols stripped from file.
#define CORE_IMAGE_FILE_AGGRESIVE_WS_TRIM         0x0010  // Agressively trim working set
#define CORE_IMAGE_FILE_LARGE_ADDRESS_AWARE       0x0020  // App can handle >2gb addresses
#define CORE_IMAGE_FILE_BYTES_REVERSED_LO         0x0080  // Bytes of machine word are reversed.
#define CORE_IMAGE_FILE_32BIT_MACHINE             0x0100  // 32 bit word machine.
#define CORE_IMAGE_FILE_DEBUG_STRIPPED            0x0200  // Debugging info stripped from file in .DBG file
#define CORE_IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP   0x0400  // If Image is on removable media, copy and run from the swap file.
#define CORE_IMAGE_FILE_NET_RUN_FROM_SWAP         0x0800  // If Image is on Net, copy and run from the swap file.
#define CORE_IMAGE_FILE_SYSTEM                    0x1000  // System File.
#define CORE_IMAGE_FILE_DLL                       0x2000  // File is a DLL.
#define CORE_IMAGE_FILE_UP_SYSTEM_ONLY            0x4000  // File should only be run on a UP machine
#define CORE_IMAGE_FILE_BYTES_REVERSED_HI         0x8000  // Bytes of machine word are reversed.

#define CORE_IMAGE_SUBSYSTEM_UNKNOWN              0   // Unknown subsystem.
#define CORE_IMAGE_SUBSYSTEM_NATIVE               1   // Image doesn't require a subsystem.
#define CORE_IMAGE_SUBSYSTEM_WINDOWS_GUI          2   // Image runs in the Windows GUI subsystem.
#define CORE_IMAGE_SUBSYSTEM_WINDOWS_CUI          3   // Image runs in the Windows character subsystem.
#define CORE_IMAGE_SUBSYSTEM_OS2_CUI              5   // image runs in the OS/2 character subsystem.
#define CORE_IMAGE_SUBSYSTEM_POSIX_CUI            7   // image runs in the Posix character subsystem.
#define CORE_IMAGE_SUBSYSTEM_NATIVE_WINDOWS       8   // image is a native Win9x driver.
#define CORE_IMAGE_SUBSYSTEM_WINDOWS_CE_GUI       9   // Image runs in the Windows CE subsystem.
#define CORE_IMAGE_SUBSYSTEM_EFI_APPLICATION      10  //
#define CORE_IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER  11   //
#define CORE_IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER   12  //
#define CORE_IMAGE_SUBSYSTEM_EFI_ROM              13
#define CORE_IMAGE_SUBSYSTEM_XBOX                 14
#define CORE_IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION 16

	//////////////////////////////////////////////////////////////////////////
	struct ST_IMAGE_FILE_HEADER {
		WORD    Machine;
		WORD    NumberOfSections;
		DWORD   TimeDateStamp;
		DWORD   PointerToSymbolTable;
		DWORD   NumberOfSymbols;
		WORD    SizeOfOptionalHeader;
		WORD    Characteristics;
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_IMAGE_DATA_DIRECTORY {
		DWORD   VirtualAddress;
		DWORD   Size;
	};

	//////////////////////////////////////////////////////////////////////////
	enum E_IMAGE_NUMBEROF_DIRECTORY_ENTRIES
	{
		CORE_IMAGE_DIRECTORY_ENTRY_EXPORT         =  0,   // Export Directory
		CORE_IMAGE_DIRECTORY_ENTRY_IMPORT         =  1,   // Import Directory
		CORE_IMAGE_DIRECTORY_ENTRY_RESOURCE       =  2,   // Resource Directory
		CORE_IMAGE_DIRECTORY_ENTRY_EXCEPTION      =  3,   // Exception Directory
		CORE_IMAGE_DIRECTORY_ENTRY_SECURITY       =  4,   // Security Directory
		CORE_IMAGE_DIRECTORY_ENTRY_BASERELOC      =  5,   // Base Relocation Table
		CORE_IMAGE_DIRECTORY_ENTRY_DEBUG          =  6,   // Debug Directory
		//CORE_IMAGE_DIRECTORY_ENTRY_COPYRIGHT      =  7   // (X86 usage)
		CORE_IMAGE_DIRECTORY_ENTRY_ARCHITECTURE   =  7,   // Architecture Specific Data
		CORE_IMAGE_DIRECTORY_ENTRY_GLOBALPTR      =  8,   // RVA of GP
		CORE_IMAGE_DIRECTORY_ENTRY_TLS            =  9,   // TLS Directory
		CORE_IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG    = 10,   // Load Configuration Directory
		CORE_IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT   = 11,   // Bound Import Directory in headers
		CORE_IMAGE_DIRECTORY_ENTRY_IAT            = 12,   // Import Address Table
		CORE_IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT   = 13,   // Delay Load Import Descriptors
		CORE_IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR = 14,   // COM Runtime descriptor
		CORE_IMAGE_DIRECTORY_END_MARKER           = 15,
		CORE_IMAGE_NUMBEROF_DIRECTORY_ENTRIES
	};

	struct ST_IMAGE_OPTIONAL_HEADER32 {
		// Standard fields.
		WORD    Magic;
		BYTE    MajorLinkerVersion;
		BYTE    MinorLinkerVersion;
		DWORD   SizeOfCode;
		DWORD   SizeOfInitializedData;
		DWORD   SizeOfUninitializedData;
		DWORD   AddressOfEntryPoint;
		DWORD   BaseOfCode;
		DWORD   BaseOfData;

		// NT additional fields.
		DWORD   ImageBase;
		DWORD   SectionAlignment;
		DWORD   FileAlignment;
		WORD    MajorOperatingSystemVersion;
		WORD    MinorOperatingSystemVersion;
		WORD    MajorImageVersion;
		WORD    MinorImageVersion;
		WORD    MajorSubsystemVersion;
		WORD    MinorSubsystemVersion;
		DWORD   Win32VersionValue;
		DWORD   SizeOfImage;
		DWORD   SizeOfHeaders;
		DWORD   CheckSum;
		WORD    Subsystem;
		WORD    DllCharacteristics;
		DWORD   SizeOfStackReserve;
		DWORD   SizeOfStackCommit;
		DWORD   SizeOfHeapReserve;
		DWORD   SizeOfHeapCommit;
		DWORD   LoaderFlags;
		DWORD   NumberOfRvaAndSizes;
		ST_IMAGE_DATA_DIRECTORY DataDirectory[CORE_IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_IMAGE_OPTIONAL_HEADER64 {
		WORD        Magic;
		BYTE        MajorLinkerVersion;
		BYTE        MinorLinkerVersion;
		DWORD       SizeOfCode;
		DWORD       SizeOfInitializedData;
		DWORD       SizeOfUninitializedData;
		DWORD       AddressOfEntryPoint;
		DWORD       BaseOfCode;
		ULONGLONG   ImageBase;
		DWORD       SectionAlignment;
		DWORD       FileAlignment;
		WORD        MajorOperatingSystemVersion;
		WORD        MinorOperatingSystemVersion;
		WORD        MajorImageVersion;
		WORD        MinorImageVersion;
		WORD        MajorSubsystemVersion;
		WORD        MinorSubsystemVersion;
		DWORD       Win32VersionValue;
		DWORD       SizeOfImage;
		DWORD       SizeOfHeaders;
		DWORD       CheckSum;
		WORD        Subsystem;
		WORD        DllCharacteristics;
		ULONGLONG   SizeOfStackReserve;
		ULONGLONG   SizeOfStackCommit;
		ULONGLONG   SizeOfHeapReserve;
		ULONGLONG   SizeOfHeapCommit;
		DWORD       LoaderFlags;
		DWORD       NumberOfRvaAndSizes;
		ST_IMAGE_DATA_DIRECTORY DataDirectory[CORE_IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_IMAGE_NT_HEADERS{
		char Signature[4];
		ST_IMAGE_FILE_HEADER FileHeader;
	};

	//////////////////////////////////////////////////////////////////////////
#define MAX_IMAGE_SIZEOF_SHORT_NAME              8
	struct ST_IMAGE_SECTION_HEADER
	{
		char    Name[MAX_IMAGE_SIZEOF_SHORT_NAME];
		union {
			DWORD   PhysicalAddress;
			DWORD   VirtualSize;
		}	Misc;
		DWORD   VirtualAddress;
		DWORD   SizeOfRawData;
		DWORD   PointerToRawData;
		DWORD   PointerToRelocations;
		DWORD   PointerToLinenumbers;
		WORD    NumberOfRelocations;
		WORD    NumberOfLinenumbers;
		DWORD   Characteristics;
	};

	//////////////////////////////////////////////////////////////////////////
	#define CORE_IMAGE_DEBUG_TYPE_UNKNOWN          0
	#define CORE_IMAGE_DEBUG_TYPE_COFF             1
	#define CORE_IMAGE_DEBUG_TYPE_CODEVIEW         2
	#define CORE_IMAGE_DEBUG_TYPE_FPO              3
	#define CORE_IMAGE_DEBUG_TYPE_MISC             4
	#define CORE_IMAGE_DEBUG_TYPE_EXCEPTION        5
	#define CORE_IMAGE_DEBUG_TYPE_FIXUP            6
	#define CORE_IMAGE_DEBUG_TYPE_OMAP_TO_SRC      7
	#define CORE_IMAGE_DEBUG_TYPE_OMAP_FROM_SRC    8
	#define CORE_IMAGE_DEBUG_TYPE_BORLAND          9
	#define CORE_IMAGE_DEBUG_TYPE_RESERVED10       10
	#define CORE_IMAGE_DEBUG_TYPE_CLSID            11

	//////////////////////////////////////////////////////////////////////////	
	struct ST_IMAGE_DEBUG_DIRECTORY
	{
		DWORD   Characteristics;
		DWORD   TimeDateStamp;
		WORD    MajorVersion;
		WORD    MinorVersion;
		DWORD   Type;
		DWORD   SizeOfData;
		DWORD   AddressOfRawData;
		DWORD   PointerToRawData;		
	};

	//////////////////////////////////////////////////////////////////////////	
	struct ST_IMAGE_DEBUG_PDB_INFO
	{
		char     Signature[4];		//CodeView signature, equal to ¡°RSDS¡±
		BYTE      Guid[16];			// A unique identifier, which changes with every rebuild of the executable and PDB file.
		DWORD     Age;				// Ever-incrementing value, which is initially set to 1 and incremented every time when a part of the PDB file is updated without rewriting the whole file.
		char      PdbFileName[MAX_PATH];	// Null-terminated name of the PDB file. It can also contain full or partial path to the file.
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_IMAGE_EXPORT_DIRECTORY
	{
		DWORD   Characteristics;
		DWORD   TimeDateStamp;
		WORD    MajorVersion;
		WORD    MinorVersion;
		DWORD   Name;
		DWORD   Base;
		DWORD   NumberOfFunctions;
		DWORD   NumberOfNames;
		DWORD   AddressOfFunctions;     // RVA from base of image
		DWORD   AddressOfNames;         // RVA from base of image
		DWORD   AddressOfNameOrdinals;  // RVA from base of image
	};

	//////////////////////////////////////////////////////////////////////////	
	struct ST_IMAGE_LOAD_CONFIG_DIRECTORY32
	{
		DWORD   Size;
		DWORD   TimeDateStamp;
		WORD    MajorVersion;
		WORD    MinorVersion;
		DWORD   GlobalFlagsClear;
		DWORD   GlobalFlagsSet;
		DWORD   CriticalSectionDefaultTimeout;
		DWORD   DeCommitFreeBlockThreshold;
		DWORD   DeCommitTotalFreeThreshold;
		DWORD   LockPrefixTable;            // VA
		DWORD   MaximumAllocationSize;
		DWORD   VirtualMemoryThreshold;
		DWORD   ProcessHeapFlags;
		DWORD   ProcessAffinityMask;
		WORD    CSDVersion;
		WORD    Reserved1;
		DWORD   EditList;                   // VA
		DWORD   SecurityCookie;             // VA
		DWORD   SEHandlerTable;             // VA
		DWORD   SEHandlerCount;
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_IMAGE_LOAD_CONFIG_DIRECTORY64
	{
		DWORD      Size;
		DWORD      TimeDateStamp;
		WORD       MajorVersion;
		WORD       MinorVersion;
		DWORD      GlobalFlagsClear;
		DWORD      GlobalFlagsSet;
		DWORD      CriticalSectionDefaultTimeout;
		ULONGLONG  DeCommitFreeBlockThreshold;
		ULONGLONG  DeCommitTotalFreeThreshold;
		ULONGLONG  LockPrefixTable;         // VA
		ULONGLONG  MaximumAllocationSize;
		ULONGLONG  VirtualMemoryThreshold;
		ULONGLONG  ProcessAffinityMask;
		DWORD      ProcessHeapFlags;
		WORD       CSDVersion;
		WORD       Reserved1;
		ULONGLONG  EditList;                // VA
		ULONGLONG  SecurityCookie;          // VA
		ULONGLONG  SEHandlerTable;          // VA
		ULONGLONG  SEHandlerCount;
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_IMAGE_IMPORT_DESCRIPTOR
	{
		union {
			DWORD   Characteristics;            // 0 for terminating null import descriptor
			DWORD   OriginalFirstThunk;         // RVA to original unbound IAT (PIMAGE_THUNK_DATA)
		};
		DWORD   TimeDateStamp;                  // 0 if not bound,
		// -1 if bound, and real date\time stamp
		//     in IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT (new BIND)
		// O.W. date/time stamp of DLL bound to (Old BIND)

		DWORD   ForwarderChain;                 // -1 if no forwarders
		DWORD   Name;
		DWORD   FirstThunk;                     // RVA to IAT (if bound this IAT has actual addresses)
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_IMAGE_THUNK_DATA64 {
		union {
			ULONGLONG ForwarderString;  // PBYTE 
			ULONGLONG Function;         // PDWORD
			ULONGLONG Ordinal;
			ULONGLONG AddressOfData;    // PIMAGE_IMPORT_BY_NAME
		} u1;
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_IMAGE_THUNK_DATA32 {
		union {
			DWORD ForwarderString;      // PBYTE 
			DWORD Function;             // PDWORD
			DWORD Ordinal;
			DWORD AddressOfData;        // PIMAGE_IMPORT_BY_NAME
		} u1;
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_IMAGE_IMPORT_BY_NAME
	{
		WORD    Hint;
		BYTE    Name[1];
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_IMAGE_RESOURCE_DIRECTORY {
		DWORD   Characteristics;
		DWORD   TimeDateStamp;
		WORD    MajorVersion;
		WORD    MinorVersion;
		WORD    NumberOfNamedEntries;
		WORD    NumberOfIdEntries;
		//  IMAGE_RESOURCE_DIRECTORY_ENTRY DirectoryEntries[];
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_IMAGE_RESOURCE_DIRECTORY_ENTRY {
		union {
			struct {
				DWORD NameOffset:31;
				DWORD NameIsString:1;
			};
			DWORD   Name;
			WORD    Id;
		};
		union {
			DWORD   OffsetToData;
			struct {
				DWORD   OffsetToDirectory:31;
				DWORD   DataIsDirectory:1;
			};
		};
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_IMAGE_RESOURCE_DIRECTORY_STRING {
		WORD    Length;
		CHAR    NameString[ 1 ];
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_IMAGE_RESOURCE_DIR_STRING_U {
		WORD    Length;
		WCHAR   NameString[ 1 ];
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_IMAGE_RESOURCE_DATA_ENTRY {
		DWORD   OffsetToData;
		DWORD   Size;
		DWORD   CodePage;
		DWORD   Reserved;
	};

#pragma pack(pop)
}