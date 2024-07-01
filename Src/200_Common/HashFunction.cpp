#include "stdafx.h"
#include "HashFunction.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	std::string BuildHash(E_HASH_TYPE nType, const std::vector<BYTE>& vecData)
	{
		HANDLE hHash = InitHash(nType);
		if (NULL == hHash)
		{
			Log_Error(TEXT("InitHash(%s:%u) failure"), GetHashTypeString(nType), nType);
			return "";
		}
		UpdateHash(hHash, vecData.data(), vecData.size());
		return FinalHash(hHash);
	}
}