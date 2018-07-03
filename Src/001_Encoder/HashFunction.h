#pragma once

#include <vector>

#include "../__Common/Type.h"
#include "Type.h"

namespace core
{
	HANDLE		InitHash(E_HASH_TYPE dwHashType);
	void		UpdateHash(HANDLE hHash, const unsigned char* pData, size_t tSize);
	void		FinalHash(HANDLE hHash, std::vector<BYTE>& outBuff);
	std::string	FinalHash(HANDLE hHash);
}
