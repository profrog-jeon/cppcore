#pragma once

#include <string>
#include <vector>
#include <map>

#include "FmtTypes.h"
#include "IChannel.h"

//////////////////////////////////////////////////////////////////////////
namespace fmt_internal
{
	struct ST_UBJ_NODE
	{
		std::string strKey;		// UTF8
		std::string strValue;	// UTF8
		std::vector<ST_UBJ_NODE> Children;
	};

	void WriteUBJsonKey(core::IChannel& Channel, const std::string& refKey);
	void WriteUBJsonKey(core::IChannel& Channel, const std::wstring& refKey);
	void WriteUBJsonValue(core::IChannel& Channel, const char& refValue);
	void WriteUBJsonValue(core::IChannel& Channel, const short& refValue);
	void WriteUBJsonValue(core::IChannel& Channel, const int32_t& refValue);
	void WriteUBJsonValue(core::IChannel& Channel, const int64_t& refValue);
	void WriteUBJsonValue(core::IChannel& Channel, const BYTE& refValue);
	void WriteUBJsonValue(core::IChannel& Channel, const WORD& refValue);
	void WriteUBJsonValue(core::IChannel& Channel, const DWORD& refValue);
	void WriteUBJsonValue(core::IChannel& Channel, const QWORD& refValue);
	void WriteUBJsonValue(core::IChannel& Channel, const float& refValue);
	void WriteUBJsonValue(core::IChannel& Channel, const double& refValue);
	void WriteUBJsonValue(core::IChannel& Channel, const std::string& refValue);
	void WriteUBJsonValue(core::IChannel& Channel, const std::wstring& refValue);
	void WriteUBJsonValue(core::IChannel& Channel, const std::vector<BYTE>& refValue);
	void WriteUBJsonConst(core::IChannel& Channel, std::string strValue);
	void WriteUBJsonConst(core::IChannel& Channel, std::wstring strValue);

	std::string ReadUBJValue(core::IChannel& Channel);
	std::string ReadUBJValue(core::IChannel& Channel, char cType);

	ECODE ParseUBJson(core::IChannel& Channel, ST_UBJ_NODE& outRoot, std::string& outErrMsg);
	ECODE ParseUBJson(core::IChannel& Channel, ST_UBJ_NODE& outRoot, std::wstring& outErrMsg);
}
