#pragma once

#include <map>
#include <vector>
#include <string>

#include "../__Common/Type.h"
#include "ExeParserSuper.h"

//const int gc_nFileVersion[4] __attribute__((section ("FileVersion"))) = { 9, 8, 7, 6 };
//const int gc_nProductVersion[4] __attribute__((section ("ProductVersion"))) = { 9, 8, 7, 6 };

namespace core
{
	// BYTE ei_class;			/* 1:little endian, 2:big endian */
	#define	ELFCLASSNONE 0		/* Invalid class */
	#define	ELFCLASS32	 1             /* 32 bit objects */
	#define	ELFCLASS64	 2             /* 64 bit objects */
	#define	ELFCLASSNUM	 3             /* ELF class number */

	// BYTE ei_data;			/* 1:little endian, 2:big endian */
	#define ELFDATANONE	0		/* Invalid data encoding */
	#define ELFDATA2LSB	1             /* LSB 가 가장 낮은 주소를 차지 한다. */
	#define ELFDATA2MSB	2             /* MSG가 가장 낮은 주소를 차지 한다. */

	// WORD	e_type;
	#define ET_NONE   0
	#define ET_REL    1
	#define ET_EXEC   2
	#define ET_DYN    3
	#define ET_CORE   4
	#define ET_LOPROC 0xff00
	#define ET_HIPROC 0xffff

	// WORD	e_machine;		/* Architecture */
	#define EM_NONE  0
	#define EM_M32   1     /* AT&T WE32100 */
	#define EM_SPARC 2     /* SPARC */
	#define EM_386   3      /* Intel 80386 */
	#define EM_68K   4      /* Motorola 68000 */
	#define EM_88K   5      /* Motorola 88000 */
	#define EM_486   6      /* reserved */ 
	#define EM_860   7      /* Intel 80860 */
	#define EM_MIPS	 8     /* MIPS R3000 (officially, big-endian only) */
	#define EM_MIPS_RS4_BE 10	/* MIPS R4000 big-endian */
	#define EM_PARISC      15	/* HPPA */
	#define EM_SPARC32PLUS 18/* Sun's "v8plus" */
	#define EM_PPC	   20	/* PowerPC */
	#define EM_SH	   42	/* SuperH */
	#define EM_SPARCV9   43	/* SPARC v9 64-bit */
	#define EM_IA_64	   50	/* HP/Intel IA-64 */
	#define EM_X8664       62  /* AMD x86-64 */
	#define EM_ALPHA	   0x9026 /* Alpha */
	#define EM_S390        0xA390 /* IBM S390 */

	enum E_PT_TYPE
	{
		PT_NULL		= 0,			// Unused entry
		PT_LOAD		= 1,			// Loadable segment
		PT_DYNAMIC	= 2,			// Dynamic linking tables
		PT_INTERP	= 3,			// Program interpreter path name
		PT_NOTE		= 4,			// Note sections
		PT_SHLIB	= 5,			// Reserved
		PT_PHDR		= 6,			// Program header table
		PT_LOOS		= 0x60000000,	// Environment-specific use
		PT_HIOS		= 0x6FFFFFFF,
		PT_LOPROC	= 0x70000000,	// Processor-specific uses
		PT_HIPROC	= 0x7FFFFFFF,
	};

	struct ST_ELF_IDENTITY
	{
		BYTE ei_magic[4];		/* 0x7F, 'E', 'L', 'F' */
		BYTE ei_class;			/* 1:32bit, 2:64bit */
		BYTE ei_data;			/* 1:little endian, 2:big endian */
		BYTE ei_version;		/* 0:Unknown, 1:current, 2:next */
		BYTE ei_osabi;			/* 0:SystemV, 1:HP-UX, 2:NetBSD, 3:Linux, 6:Solaris, 7:AIX, 8:IRIX, 9:FreeBSD, C:OpenBSD, D:OpenVMS */
		BYTE ei_abiversion;		/* ? */
		BYTE ei_pad[7];
	};

	struct ST_ELF_HEADER32
	{
		WORD	e_type;			/* Object file type, 1:reallocation 2: executable 3: shared obj 4: core */
		WORD	e_machine;		/* Architecture */
		DWORD	e_version;		/* Object file version */
		DWORD	e_entry;		/* Entry point virtual address */
		DWORD	e_phoff;		/* Program header table file offset */
		DWORD	e_shoff;		/* Section header table file offset */
		DWORD	e_flags;		/* Processor-specific flags */
		WORD	e_ehsize;		/* ELF header size in bytes */
		WORD	e_phentsize;	/* Program header table entry size */
		WORD	e_phnum;		/* Program header table entry count */
		WORD	e_shentsize;	/* Section header table entry size */
		WORD	e_shnum;		/* Section header table entry count */
		WORD	e_shstrndx;		/* Section header string table index */
	};

	struct ST_ELF_HEADER64
	{
		WORD	e_type;			/* Object file type, 1:reallocation 2: executable 3: shared obj 4: core */
		WORD	e_machine;		/* Architecture */
		DWORD	e_version;		/* Object file version */
		QWORD	e_entry;		/* Entry point virtual address */
		QWORD	e_phoff;		/* Program header table file offset */
		QWORD	e_shoff;		/* Section header table file offset */
		DWORD	e_flags;		/* Processor-specific flags */
		WORD	e_ehsize;		/* ELF header size in bytes */
		WORD	e_phentsize;	/* Program header table entry size */
		WORD	e_phnum;		/* Program header table entry count */
		WORD	e_shentsize;	/* Section header table entry size */
		WORD	e_shnum;		/* Section header table entry count */
		WORD	e_shstrndx;		/* Section header string table index */
	};

	struct ST_ELF_PROGRAM_HEADER32
	{
		E_PT_TYPE	p_type;			/* Segment type */
		DWORD	p_flags;		/* Segment flags */
		DWORD	p_offset;		/* Segment file offset */
		DWORD	p_vaddr;		/* Segment virtual address */
		DWORD	p_paddr;		/* Segment physical address */
		QWORD	p_filesz;		/* Segment size in file */
		QWORD	p_memsz;		/* Segment size in memory */
		QWORD	p_align;		/* Segment alignment */
	};

	struct ST_ELF_PROGRAM_HEADER64
	{
		E_PT_TYPE	p_type;			/* Segment type */
		DWORD	p_flags;		/* Segment flags */
		QWORD	p_offset;		/* Segment file offset */
		QWORD	p_vaddr;		/* Segment virtual address */
		QWORD	p_paddr;		/* Segment physical address */
		QWORD	p_filesz;		/* Segment size in file */
		QWORD	p_memsz;		/* Segment size in memory */
		QWORD	p_align;		/* Segment alignment */
	};

	enum E_SHT_TYPE
	{
		SHT_NULL		= 0,
		SHT_PROGBITS	= 1,
		SHT_SYMTAB		= 2,
		SHT_STRTAB		= 3,
		SHT_RELA		= 4,
		SHT_HASH		= 5,
		SHT_DYNAMIC		= 6,
		SHT_NOTE		= 7,
		SHT_NOBITS		= 8,
		SHT_REL			= 9,
		SHT_SHLIB		= 10,
		SHT_DYNSYM		= 11,
		SHT_LOPROC		= 0x70000000,
		SHT_HIPROC		= 0x7fffffff,
		SHT_LOUSER		= 0x80000000,
		SHT_HIUSER		= 0xffffffff,
	};

	struct ST_ELF_SECTION_HEADER32
	{
		DWORD	sh_name;		/* Section name (string tbl index) */
		E_SHT_TYPE	sh_type;		/* Section type */
		DWORD	sh_flags;		/* Section flags */
		DWORD	sh_addr;		/* Section virtual addr at execution */
		DWORD	sh_offset;		/* Section file offset */
		DWORD	sh_size;		/* Section size in bytes */
		DWORD	sh_link;		/* Link to another section */
		DWORD	sh_info;		/* Additional section information */
		DWORD	sh_addralign;		/* Section alignment */
		DWORD	sh_entsize;		/* Entry size if section holds table */
	};

	struct ST_ELF_SECTION_HEADER64
	{
		DWORD	sh_name;		/* Section name (string tbl index) */
		E_SHT_TYPE	sh_type;		/* Section type */
		QWORD	sh_flags;		/* Section flags */
		QWORD	sh_addr;		/* Section virtual addr at execution */
		QWORD	sh_offset;		/* Section file offset */
		QWORD	sh_size;		/* Section size in bytes */
		DWORD	sh_link;		/* Link to another section */
		DWORD	sh_info;		/* Additional section information */
		QWORD	sh_addralign;		/* Section alignment */
		QWORD	sh_entsize;		/* Entry size if section holds table */
	};

	enum E_DT_TYPE
	{
		DT_NULL		= 0			, // ignored		
		DT_NEEDED	= 1			, // d_val		
		DT_PLTRELSZ	= 2			, // d_val		
		DT_PLTGOT	= 3			, // d_ptr		
		DT_HASH		= 4			, // d_ptr		
		DT_STRTAB	= 5			, // d_ptr		
		DT_SYMTAB	= 6			, // d_ptr		
		DT_RELA		= 7			, // d_ptr		
		DT_RELASZ	= 8			, // d_val		
		DT_RELAENT	= 9			, // d_val		
		DT_STRSZ	= 10		, // d_val		
		DT_SYMENT	= 11		, // d_val		
		DT_INIT		= 12		, // d_ptr		
		DT_FINI		= 13		, // d_ptr		
		DT_SONAME	= 14		, // d_val		
		DT_RPATH	= 15		, // d_val		
		DT_SYMBOLIC	= 16		, // ignored		
		DT_REL		= 17		, // d_ptr		
		DT_RELSZ	= 18		, // d_val		
		DT_RELENT	= 19		, // d_val		
		DT_PLTREL	= 20		, // d_val		
		DT_DEBUG	= 21		, // d_ptr		
		DT_TEXTREL	= 22		, // ignored		
		DT_JMPREL	= 23		, // d_ptr		

		DT_BIND_NOW	= 24		, // ignored		
		DT_INIT_ARRAY= 25		, // d_ptr		
		DT_FINI_ARRAY= 26		, // d_ptr		
		DT_INIT_ARRAYSZ= 27		, // d_val		
		DT_FINI_ARRAYSZ= 28		, // d_val		

		DT_LOPROC	= 0x70000000, // unspecified	
		DT_HIPROC	= 0x7fffffff, // unspecified	

		__DT_FOR_INT64 = 0xFFFFFFFFFFFFFFFFULL,
	};

	struct ST_ELF_DYNAMIC_TABLE64
	{
		E_DT_TYPE d_tag;

		union
		{
			QWORD d_val;
			QWORD d_ptr;
		}	d_un;
	};

	class CELFParser : public CExeParserSuper
	{
		typedef std::map<std::string, ST_ELF_SECTION_HEADER64>				CELFSectionHeader64;
		typedef std::map<std::string, ST_ELF_SECTION_HEADER64>::iterator	CELFSectionHeader64It;

		std::map<std::string, ST_VERSIONINFO> m_mapVersionInfo;

		CELFSectionHeader64 m_mapSection64;

	public:
		CELFParser(void);

		ECODE OpenWorker(HANDLE hFile);
		void Close(void);

		ECODE GetFileVersion(ST_VERSIONINFO& outVersionInfo);
		ECODE GetProductVersion(ST_VERSIONINFO& outVersionInfo);

	private:
		ECODE Parse32Bit(HANDLE hFile, const ST_ELF_IDENTITY& stIdentity);
		ECODE Parse64Bit(HANDLE hFile, const ST_ELF_IDENTITY& stIdentity);

		ECODE QueryVersion(HANDLE hFile, const char* pszSectioName, ST_VERSIONINFO& stVersionInfo);

	};

	template<typename T>
	T ReadLSB(const T& inData)
	{
		return inData;
	}

	template<typename T>
	T ReadMSB(const T& inData)
	{
		size_t tSize = sizeof(T);

		T tRet = 0;
		size_t i;
		for(i=0; i<tSize; i++)
			tRet |= ((inData >> (i*8)) & 0xFF) << ((tSize - i - 1) * 8);

		return tRet;
	}
}
