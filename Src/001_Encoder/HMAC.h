#pragma once

#include <vector>

#include "../__Common/Type.h"
#include "Type.h"

namespace core
{
	HANDLE		InitHMAC(E_HASH_TYPE nHashType, std::string strKey);
	HANDLE		InitHMAC(E_HASH_TYPE nHashType, LPCBYTE pKey, size_t tKeyLength);
	void		UpdateHMAC(HANDLE hHash, LPCBYTE pData, size_t tSize);
	void		FinalHMAC(HANDLE hHash, std::vector<BYTE>& outBuff);
}
