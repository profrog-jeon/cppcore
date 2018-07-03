#pragma once

namespace core
{
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
}
