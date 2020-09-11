#pragma once

#include "../__Common/Type.h"

namespace fmt_internal
{
	//////////////////////////////////////////////////////////////////////////
	struct ST_XML_ATTR
	{
		std::tstring strKey;
		std::tstring strValue;
	};

	//////////////////////////////////////////////////////////////////////////
	struct ST_XML_NODE
	{
		std::tstring strKey;
		std::tstring strValue;
		std::vector<ST_XML_ATTR> vecAttr;
		std::vector<ST_XML_NODE> vecChild;
	};

	//////////////////////////////////////////////////////////////////////////
	bool ParseXmlContext(const std::tstring& strContext, ST_XML_NODE& stOutRoot, std::tstring& strOutErrMsg);
	std::tstring EncodeXmlString(std::tstring strContext);
	std::tstring DecodeXmlString(std::tstring strContext);
}

