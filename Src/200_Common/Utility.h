#pragma once

#include "../001_Encoder/Type.h"

namespace core
{
    struct ST_FUNC_LOG
    {
        std::string m_strName;
        ST_FUNC_LOG(std::string strName);
		ST_FUNC_LOG(std::wstring strName);
        ~ST_FUNC_LOG(void);
    };

	struct ST_FUNC_CONSOLE_LOG
	{
		std::string m_strName;
		ST_FUNC_CONSOLE_LOG(std::string strName);
		ST_FUNC_CONSOLE_LOG(std::wstring strName);
		~ST_FUNC_CONSOLE_LOG(void);
	};
    
	char ReverseByteOrder(char btData);
	WORD ReverseByteOrder(WORD wData);
	DWORD ReverseByteOrder(DWORD dwData);

	E_BOM_TYPE ReadBOM(HANDLE hFile);
	ECODE WriteBOM(HANDLE hFile, E_BOM_TYPE nType);

	ECODE ReadFileContents(std::string strFilePath, std::string& strContents, E_BOM_TYPE nEncodeType = BOM_UNDEFINED);
	ECODE ReadFileContents(std::wstring strFilePath, std::wstring& strContents, E_BOM_TYPE nEncodeType = BOM_UNDEFINED);
	ECODE ReadFileContentsA(std::string strFilePath, std::string& strContents, E_BOM_TYPE nEncodeType = BOM_UNDEFINED);
	ECODE ReadFileContentsA(std::wstring strFilePath, std::string& strContents, E_BOM_TYPE nEncodeType = BOM_UNDEFINED);
	ECODE ReadFileContentsW(std::string strFilePath, std::wstring& strContents, E_BOM_TYPE nEncodeType = BOM_UNDEFINED);
	ECODE ReadFileContentsW(std::wstring strFilePath, std::wstring& strContents, E_BOM_TYPE nEncodeType = BOM_UNDEFINED);
	ECODE ReadFileContents(std::string strFilePath, std::vector<BYTE>& outContents);
	ECODE ReadFileContents(std::wstring strFilePath, std::vector<BYTE>& outContents);

	ECODE WriteFileContents(std::string strFilePath, const std::string strContents, bool bWithBOM = true);
	ECODE WriteFileContents(std::wstring strFilePath, const std::wstring strContents, bool bWithBOM = true);
	ECODE WriteFileContentsT(std::string strFilePath, const std::string strContents, bool bWithBOM = true);
	ECODE WriteFileContentsT(std::wstring strFilePath, const std::wstring strContents, bool bWithBOM = true);
	ECODE WriteFileContentsA(std::string strFilePath, const std::string strContents, bool bWithBOM = true);
	ECODE WriteFileContentsA(std::wstring strFilePath, const std::string strContents, bool bWithBOM = true);
	ECODE WriteFileContentsW(std::string strFilePath, const std::wstring strContents, bool bWithBOM = true);
	ECODE WriteFileContentsW(std::wstring strFilePath, const std::wstring strContents, bool bWithBOM = true);
	ECODE WriteFileContents(std::string strFilePath, const std::vector<BYTE>& vecContents);
	ECODE WriteFileContents(std::wstring strFilePath, const std::vector<BYTE>& vecContents);
	ECODE WriteFileContents(std::string strFilePath, const void* pContents, size_t tContentsSize);
	ECODE WriteFileContents(std::wstring strFilePath, const void* pContents, size_t tContentsSize);

	// returns EC_INVALID_DATA; when the file is not encoded.
	ECODE ReadFileContents(std::string strFilePath, std::string& strContents, std::string strKey, E_SYM_CIPHER_TYPE* pOutType = NULL, E_SYM_CIPHER_MODE* pOutMode = NULL);
	ECODE ReadFileContents(std::wstring strFilePath, std::wstring& strContents, std::string strKey, E_SYM_CIPHER_TYPE* pOutType = NULL, E_SYM_CIPHER_MODE* pOutMode = NULL);
	ECODE WriteFileContents(std::string strFilePath, const std::string strContents, E_SYM_CIPHER_TYPE nType, E_SYM_CIPHER_MODE nMode, std::string strKey);
	ECODE WriteFileContents(std::wstring strFilePath, const std::wstring strContents, E_SYM_CIPHER_TYPE nType, E_SYM_CIPHER_MODE nMode, std::string strKey);

	std::string&  MakeAbsolutePath(std::string strParentPath, std::string& strRelativePath);
	std::wstring& MakeAbsolutePath(std::wstring strParentPath, std::wstring& strRelativePath);
	std::string   MakeAbsolutePath(LPCSTR pszParentPath, LPCSTR pszRelativePath);
	std::wstring  MakeAbsolutePath(LPCWSTR pszParentPath, LPCWSTR pszRelativePath);

	std::string&  MakeAbsPathByModulePath(std::string& strRelativePath);
	std::wstring& MakeAbsPathByModulePath(std::wstring& strRelativePath);
	std::string   MakeAbsPathByModulePath(LPCSTR pszRelativePath);
	std::wstring  MakeAbsPathByModulePath(LPCWSTR pszRelativePath);

	std::string&  MakeAbsPathByCurPath(std::string& strRelativePath);
	std::wstring& MakeAbsPathByCurPath(std::wstring& strRelativePath);
	std::string   MakeAbsPathByCurPath(LPCSTR pszRelativePath);
	std::wstring  MakeAbsPathByCurPath(LPCWSTR pszRelativePath);

	std::string& StripQuotation(std::string& strContext);
	std::wstring& StripQuotation(std::wstring& strContext);
	std::string StripQuotation(LPCSTR pszContext, size_t tContextLen);
	std::wstring StripQuotation(LPCWSTR pszContext, size_t tContextLen);

	std::string& WrapQuotation(std::string& strContext);
	std::wstring& WrapQuotation(std::wstring& strContext);
	std::string WrapQuotation(LPCSTR pszContext, size_t tContextLen);
	std::wstring WrapQuotation(LPCWSTR pszContext, size_t tContextLen);

	std::string BuildUniqFileName(std::string strTempFile);
	std::wstring BuildUniqFileName(std::wstring strTempFile);
}
