#pragma once

namespace core
{
	class CCurrentDirectorySettter
	{
		std::wstring m_strOriginalDir;
		bool m_bDirChanged;

	public:
		CCurrentDirectorySettter(std::string strWorkingDir);
		CCurrentDirectorySettter(std::wstring strWorkingDir);
		~CCurrentDirectorySettter(void);

		bool DirChanged(void);
	};
}