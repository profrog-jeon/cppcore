#include "stdafx.h"
#include "CmdLineHelper.h"
#include "Utility.h"

namespace core
{
	inline static std::tstring MakeCmdLineWorker(const std::vector<std::tstring>& vecArgs)
	{
		if (vecArgs.empty())
			return TEXT("");

		std::tstring strCmdLine = vecArgs[0];
		for (size_t i = 1; i < vecArgs.size(); i++)
		{
			const std::tstring& strArg = vecArgs[i];

			if (std::tstring::npos == strArg.find(TEXT(' ')))
				strCmdLine += TEXT(" ") + strArg;
			else
				strCmdLine += TEXT(" ") + WrapQuotation(strArg.c_str(), strArg.length());
		}

		return strCmdLine;
	}

	inline static std::tstring MakeCmdLineWorker(std::tstring strProcExe, const std::vector<std::tstring>& vecArgs)
	{
		std::tstring strCmdLine = strProcExe;
		if (std::tstring::npos != strProcExe.find(TEXT(' ')))
			strCmdLine = WrapQuotation(strCmdLine);

		std::tstring strArgs = MakeCmdLineWorker(vecArgs);
		if (!strArgs.empty())
			strCmdLine += TEXT(" ") + strArgs;

		return strCmdLine;
	}

	std::tstring MakeCmdLine(const std::vector<std::tstring>& vecArgs)
	{
		return MakeCmdLineWorker(vecArgs);
	}

	std::tstring MakeCmdLine(const std::tstring& strProcExe, std::vector<std::tstring> vecArgs)
	{
		return MakeCmdLineWorker(strProcExe, vecArgs);
	}

	std::tstring BuildCmdLine(const std::vector<std::tstring>& vecArgs)
	{
		return MakeCmdLineWorker(vecArgs);
	}

	std::tstring BuildCmdLine(const std::tstring& strProcExe, std::vector<std::tstring> vecArgs)
	{
		return MakeCmdLineWorker(strProcExe, vecArgs);
	}

	inline static void TokenizeCmdLineWorker(const std::tstring& strCmdLine, std::vector<std::tstring>& outArgs)
	{
		size_t tPos = 0;

		while (tPos < strCmdLine.length())
		{
			size_t tBeginPos = strCmdLine.find_first_not_of(TEXT(" \r\n\t"), tPos);
			if (std::tstring::npos == tBeginPos)
				break;

			size_t tEndPos = std::tstring::npos;
			if (TEXT('\"') == strCmdLine.at(tBeginPos))
			{
				tEndPos = strCmdLine.find(TEXT('\"'), tBeginPos + 1);
				if (std::tstring::npos != tEndPos)
					tEndPos++;
			}
			else
				tEndPos = strCmdLine.find_first_of(TEXT(" \r\n\t"), tBeginPos);

			if (std::tstring::npos != tEndPos)
			{
				outArgs.push_back(strCmdLine.substr(tBeginPos, tEndPos - tBeginPos));
				tPos = tEndPos + 1;
			}
			else
			{
				outArgs.push_back(strCmdLine.substr(tBeginPos));
				break;
			}
		}

		for (std::tstring& strArg : outArgs)
			StripQuotation(strArg);
	}

	void ParseCmdLine(const std::tstring& strCmdLine, std::vector<std::tstring>& outArgs)
	{
		TokenizeCmdLineWorker(strCmdLine, outArgs);
	}

	void TokenizeCmdLine(const std::tstring& strCmdLine, std::vector<std::tstring>& outArgs)
	{
		TokenizeCmdLineWorker(strCmdLine, outArgs);
	}
}