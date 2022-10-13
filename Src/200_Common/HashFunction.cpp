#include "stdafx.h"
#include "HashFunction.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	std::string BuildHash(E_HASH_TYPE nType, const std::vector<BYTE>& vecData)
	{
		HANDLE hHash = InitHash(nType);
		UpdateHash(hHash, vecData.data(), vecData.size());
		return FinalHash(hHash);
	}
}