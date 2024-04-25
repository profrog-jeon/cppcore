#pragma once

namespace core
{
	std::string MakeCmdLine(const std::vector<std::string>& vecArgs);
	std::wstring MakeCmdLine(const std::vector<std::wstring>& vecArgs);
	std::string MakeCmdLine(const std::string& strProcExe, std::vector<std::string> vecArgs);
	std::wstring MakeCmdLine(const std::wstring& strProcExe, std::vector<std::wstring> vecArgs);

	// same with MakeCmdLine, just for diversity
	std::string BuildCmdLine(const std::vector<std::string>& vecArgs);
	std::wstring BuildCmdLine(const std::vector<std::wstring>& vecArgs);
	std::string BuildCmdLine(const std::string& strProcExe, std::vector<std::string> vecArgs);
	std::wstring BuildCmdLine(const std::wstring& strProcExe, std::vector<std::wstring> vecArgs);

	void ParseCmdLine(const std::string& strCmdLine, std::vector<std::string>& outArgs);
	void ParseCmdLine(const std::wstring& strCmdLine, std::vector<std::wstring>& outArgs);

	// same with ParseCmdLine, just for diversity
	void TokenizeCmdLine(const std::string& strCmdLine, std::vector<std::string>& outArgs);
	void TokenizeCmdLine(const std::wstring& strCmdLine, std::vector<std::wstring>& outArgs);
}