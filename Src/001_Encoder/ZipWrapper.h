#pragma once

#include "../../../cppcore/Inc/cppcore.h"

struct zip;
typedef struct zip zip_t;

#ifdef UNICODE
#define ST_ZIP_ENTRY		ST_ZIP_ENTRYW
#else
#define ST_ZIP_ENTRY		ST_ZIP_ENTRYA
#endif

namespace core
{
	struct ST_ZIP_ENTRYA
	{
		std::string strFile;
		QWORD qwIndex;
		QWORD qwSize;
		QWORD qwCompressedSize;
		QWORD qwModificationTime;
		DWORD dwCRC;
	};

	struct ST_ZIP_ENTRYW
	{
		std::wstring strFile;
		QWORD qwIndex;
		QWORD qwSize;
		QWORD qwCompressedSize;
		QWORD qwModificationTime;
		DWORD dwCRC;
	};

	class CZipWrapper
	{
		zip_t* m_pZip;

	public:
		CZipWrapper(void);
		~CZipWrapper(void);

		ECODE Create(std::string strFile);
		ECODE Create(std::wstring strFile);
		ECODE Open(std::string strFile);
		ECODE Open(std::wstring strFile);

		bool FlushAndClose(void);
		void DiscardAndClose(void);

		ECODE QueryFiles(std::string strSrcPattern, std::vector<ST_ZIP_ENTRYA>& vecEntries);
		ECODE QueryFiles(std::wstring strSrcPattern, std::vector<ST_ZIP_ENTRYW>& vecEntries);

		ECODE AddFile(std::string strZipPath, std::string strSrcFile, LPCSTR pszPassword = NULL);
		ECODE AddFile(std::wstring strZipPath, std::wstring strSrcFile, LPCSTR pszPassword = NULL);
		ECODE AddFileByMemory(std::string strZipPath, const void* pData, size_t tDataSize, LPCSTR pszPassword = NULL);
		ECODE AddFileByMemory(std::wstring strZipPath, const void* pData, size_t tDataSize, LPCSTR pszPassword = NULL);

		ECODE DelFile(std::string strSrcPattern, LPCSTR pszPassword = NULL);
		ECODE DelFile(std::wstring strSrcPattern, LPCSTR pszPassword = NULL);
		ECODE ExtractToDirectory(std::string strSrcPattern, std::string strTargetDir, LPCSTR pszPassword = NULL);
		ECODE ExtractToDirectory(std::wstring strSrcPattern, std::wstring strTargetDir, LPCSTR pszPassword = NULL);

	private:
		LPCSTR GetLastErrorStr(void);
	};


	ECODE ZipFromFile(std::string strZipFile, std::string strPathInZip, std::string strFile, LPCSTR pszPassword = NULL);
	ECODE ZipFromFile(std::wstring strZipFile, std::wstring strPathInZip, std::wstring strFile, LPCSTR pszPassword = NULL);
	ECODE ZipFromBuffer(std::string strZipFile, std::string strPathInZip, const void* pData, size_t tDataSize, LPCSTR pszPassword = NULL);
	ECODE ZipFromBuffer(std::wstring strZipFile, std::wstring strPathInZip, const void* pData, size_t tDataSize, LPCSTR pszPassword = NULL);
	ECODE ZipFromFiles(std::string strZipFile, const std::vector<std::string>& vecPathInZip, const std::vector<std::string>& vecFile, LPCSTR pszPassword = NULL);
	ECODE ZipFromFiles(std::wstring strZipFile, const std::vector<std::wstring>& vecPathInZip, const std::vector<std::wstring>& vecFile, LPCSTR pszPassword = NULL);
}
