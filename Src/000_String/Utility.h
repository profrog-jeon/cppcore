#pragma once

#ifdef UNICODE
#define MakeHexDumpStr	MakeHexDumpStrW
#else
#define MakeHexDumpStr	MakeHexDumpStrA
#endif

namespace core
{
	bool IsReadableChar(char cChar);
	bool IsReadableChar(wchar_t cChar);

	std::string& MakeFormalPath(std::string& strInformalPath);
	std::wstring& MakeFormalPath(std::wstring& strInformalPath);
    std::string MakeFormalPath(LPCSTR pszInformalPath);
    std::wstring MakeFormalPath(LPCWSTR pszInformalPath);
    
	std::string ExtractDirectory(std::string strFullPath);
	std::wstring ExtractDirectory(std::wstring strFullPath);
	std::string ExtractFileName(std::string strFullPath);
	std::wstring ExtractFileName(std::wstring strFullPath);
	std::string ExtractFileNameWithoutExt(std::string strFilename);
	std::wstring ExtractFileNameWithoutExt(std::wstring strFilename);
	std::string ExtractFileExt(std::string strFullPath);
	std::wstring ExtractFileExt(std::wstring strFullPath);

	std::string MakeHexDumpStrA(LPCBYTE pData, size_t tSize);
	std::wstring MakeHexDumpStrW(LPCBYTE pData, size_t tSize);
	std::string MakeHexDumpStrA(std::string strHexDump);
	std::wstring MakeHexDumpStrW(std::wstring strHexDump);
}
