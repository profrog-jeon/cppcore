#pragma once

#include <string>
#include <vector>
#include <map>

#include "FmtTypes.h"
#include "IChannel.h"

#ifdef UNICODE
#define ST_YAML_NODE ST_YAML_NODEW
#else
#define ST_YAML_NODE ST_YAML_NODEA
#endif

//////////////////////////////////////////////////////////////////////////
namespace fmt_internal
{
	struct ST_YAML_NODEW
	{
		int nIndentationLevel;
		std::wstring strKey;
		std::wstring strValue;
		std::vector<ST_YAML_NODEW> Children;

		ST_YAML_NODEW(void);
		~ST_YAML_NODEW(void);

		void PrintOut(void);
	};

	struct ST_YAML_NODEA
	{
		int nIndentationLevel;
		std::string strKey;
		std::string strValue;
		std::vector<ST_YAML_NODEA> Children;

		ST_YAML_NODEA(void);
		~ST_YAML_NODEA(void);

		void PrintOut(void);
	};

	class CYAMLParser
	{
		core::IChannel& m_Channel;
		size_t m_tIndentationWidth;

	public:
		CYAMLParser(core::IChannel& Channel);
		~CYAMLParser(void);

		ECODE Parse(ST_YAML_NODE& outRoot, std::tstring& outErrMsg);

	private:
		void CalcIndentation(std::tstring& refLine, int& outIndentationLevel);
		bool ParseNode(std::tstring strLine, ST_YAML_NODE& outNode);
		bool ReadLine(std::tstring& outLine);
	};

	template<typename T>
	std::tstring EncodeYamlString(T value)
	{
		return core::StringFrom(value);
	}

	template<typename T>
	T DecodeYamlString(std::tstring strContext)
	{
		return core::ValueFrom<T>(strContext);
	}

	void RemoveYamlComment(std::tstring& refLine);
	std::tstring EncodeYamlString(std::tstring strContext);
	std::tstring DecodeYamlString(std::tstring strContext);
}
