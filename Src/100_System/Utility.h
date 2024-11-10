#pragma once

namespace core
{
	bool operator<(const core::ST_VERSIONINFO& lhs, const core::ST_VERSIONINFO& rhs);
	bool operator<(const core::ST_SYSTEMTIME& lhs, const core::ST_SYSTEMTIME& rhs);

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