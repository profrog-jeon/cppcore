#include "stdafx.h"
#include "UBJSONFunctions.h"

//////////////////////////////////////////////////////////////////////////
namespace fmt_internal
{
	void WriteUBJsonKey(core::IChannel& Channel, const std::string& refKey)
	{
		char cKeyLen = (char)refKey.length();
		Channel.Access((void*)"i", 1);
		Channel.Access((void*)&cKeyLen, sizeof(cKeyLen));
		WriteUBJsonConst(Channel, refKey);
		Channel.Access((void*)":", 1);
	}

	void WriteUBJsonKey(core::IChannel& Channel, const std::wstring& refKey)
	{
		char cKeyLen = (char)refKey.length();
		Channel.Access((void*)"i", 1);
		Channel.Access((void*)&cKeyLen, sizeof(cKeyLen));
		WriteUBJsonConst(Channel, refKey);
		Channel.Access((void*)":", 1);
	}

	void WriteUBJsonValue(core::IChannel& Channel, const char& refValue)
	{
		Channel.Access((void*)"i", 1);
		Channel.Access((void*)& refValue, sizeof(refValue));
	}

	void WriteUBJsonValue(core::IChannel& Channel, const short& refValue)
	{
		Channel.Access((void*)"I", 1);
		Channel.Access((void*)&refValue, sizeof(refValue));
	}

	void WriteUBJsonValue(core::IChannel& Channel, const int& refValue)
	{
		Channel.Access((void*)"l", 1);
		Channel.Access((void*)&refValue, sizeof(refValue));
	}

	void WriteUBJsonValue(core::IChannel& Channel, const long long& refValue)
	{
		Channel.Access((void*)"L", 1);
		Channel.Access((void*)&refValue, sizeof(refValue));
	}

	void WriteUBJsonValue(core::IChannel& Channel, const BYTE& refValue)
	{
		Channel.Access((void*)"U", 1);
		Channel.Access((void*)&refValue, sizeof(refValue));
	}

	void WriteUBJsonValue(core::IChannel& Channel, const WORD& refValue)
	{
		Channel.Access((void*)"I", 1);
		Channel.Access((void*)&refValue, sizeof(refValue));
	}

	void WriteUBJsonValue(core::IChannel& Channel, const DWORD& refValue)
	{
		Channel.Access((void*)"l", 1);
		Channel.Access((void*)&refValue, sizeof(refValue));
	}

	void WriteUBJsonValue(core::IChannel& Channel, const QWORD& refValue)
	{
		Channel.Access((void*)"L", 1);
		Channel.Access((void*)&refValue, sizeof(refValue));
	}

	void WriteUBJsonValue(core::IChannel& Channel, const float& refValue)
	{
		Channel.Access((void*)"d", 1);
		Channel.Access((void*)&refValue, sizeof(refValue));
	}

	void WriteUBJsonValue(core::IChannel& Channel, const double& refValue)
	{
		Channel.Access((void*)"D", 1);
		Channel.Access((void*)&refValue, sizeof(refValue));
	}

	void WriteUBJsonValue(core::IChannel& Channel, const std::string& refValue)
	{
		std::string strValueU8 = UTF8FromMBS(refValue);
		const DWORD dwLength = (DWORD)strValueU8.length();

		Channel.Access((void*)"S", 1);
		Channel.Access((void*)"l", 1);
		Channel.Access((void*)&dwLength, sizeof(dwLength));
		Channel.Access((void*)strValueU8.c_str(), strValueU8.length());
	}

	void WriteUBJsonValue(core::IChannel& Channel, const std::wstring& refValue)
	{
		std::string strValueU8 = UTF8FromWCS(refValue);
		const DWORD dwLength = (DWORD)strValueU8.length();

		Channel.Access((void*)"S", 1);
		Channel.Access((void*)"l", 1);
		Channel.Access((void*)&dwLength, sizeof(dwLength));
		Channel.Access((void*)strValueU8.c_str(), strValueU8.length());
	}

	void WriteUBJsonValue(core::IChannel& Channel, const std::vector<BYTE>& refValue)
	{
		const DWORD dwSize = (DWORD)refValue.size();

		Channel.Access((void*)"B", 1);
		Channel.Access((void*)"l", 1);
		Channel.Access((void*)&dwSize, sizeof(dwSize));
		if (!refValue.empty())
			Channel.Access((void*)refValue.data(), refValue.size());
	}

	void WriteUBJsonConst(core::IChannel& Channel, std::string strValue)
	{
		std::string strValueU8 = UTF8FromMBS(strValue);
		Channel.Access((void*)strValueU8.c_str(), strValueU8.length());
	}

	void WriteUBJsonConst(core::IChannel& Channel, std::wstring strValue)
	{
		std::string strValueU8 = UTF8FromWCS(strValue);
		Channel.Access((void*)strValueU8.c_str(), strValueU8.length());
	}
}
