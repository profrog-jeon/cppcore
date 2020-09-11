#pragma once

#include "../__Common/Type.h"

#ifdef UNICODE
#define FlushINI			FlushINIA
#define DestroyINIHandle	DestroyINIHandleA
#else
#define FlushINI			FlushINIW
#define DestroyINIHandle	DestroyINIHandleW
#endif
namespace core
{
	HANDLE CreateINIHandle(std::string strPath);
	HANDLE CreateINIHandle(std::wstring strPath);

	std::string GetINIValue(HANDLE hINI, std::string strSection, std::string strKey, std::string strDefault);
	std::wstring GetINIValue(HANDLE hINI, std::wstring strSection, std::wstring strKey, std::wstring strDefault);
	ECODE SetINIValue(HANDLE hINI, std::string strSection, std::string strKey, std::string strValue);
	ECODE SetINIValue(HANDLE hINI, std::wstring strSection, std::wstring strKey, std::wstring strValue);

	ECODE FlushINIA(HANDLE hINI);
	ECODE FlushINIW(HANDLE hINI);
	ECODE DestroyINIHandleA(HANDLE hINI);
	ECODE DestroyINIHandleW(HANDLE hINI);
}
