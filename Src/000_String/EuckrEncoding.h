#pragma once

#ifdef _MSC_VER
#define MBSFromEUCKR		EUCKRFromEUCKR
#else
#define MBSFromEUCKR		UTF8FromEUCKR
#endif

#ifdef UNICODE
#define TCSFromEUCKR		WCSFromEUCKR
#else
#define TCSFromEUCKR		MBSFromEUCKR
#endif

namespace core
{
	void SetUnreadableMaskChar(char cMask = '?');

	std::wstring		WCSFromEUCKR(std::string strEUCKR);
	std::string			EUCKRFromEUCKR(std::string strEUCKR);
	std::string			UTF8FromEUCKR(std::string strEUCKR);

	bool IsInvalidEUCKR(const char* pSrc, size_t tSrcSize);
	bool IsInvalidEUCKR(const std::string& strContext);
	void MaskUnreadableEUCKR(char* pSrc, size_t tSrcSize);
	std::string& MaskUnreadableEUCKR(std::string& strContext);
}
