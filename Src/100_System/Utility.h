#pragma once

namespace core
{
	class CCurrentDirectorySettter
	{
		std::wstring m_strOriginalDir;

	public:
		CCurrentDirectorySettter(std::string strWorkingDir);
		CCurrentDirectorySettter(std::wstring strWorkingDir);
		~CCurrentDirectorySettter(void);
	};
}