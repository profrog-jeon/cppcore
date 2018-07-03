#include "stdafx.h"
#include "StringSymCipherFunction.h"
#include "SymCipherFunction.h"
#include "StringFunction.h"
#include "Base64.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	std::tstring EncodeString(const ST_SYM_CIPHER_KEY& stCipherKey, std::string strUTF8)
	{
		size_t tReqSize = EncodeData(stCipherKey, (LPCBYTE)strUTF8.c_str(), strUTF8.length(), NULL);
		std::vector<BYTE> vecEncData;
		vecEncData.resize(tReqSize);
		EncodeData(stCipherKey, (LPCBYTE)strUTF8.c_str(), strUTF8.length(), &vecEncData[0]);
		return EncodeBase64(&vecEncData[0], tReqSize);
	}

	//////////////////////////////////////////////////////////////////////////
	std::string DecodeString(const ST_SYM_CIPHER_KEY& stCipherKey, std::tstring strEncString)
	{
		if( strEncString.empty() )
			return "";

		size_t tReqSize = DecodeBase64(strEncString, NULL);
		if( 0 == tReqSize )
			return "";

		std::vector<BYTE> vecEncData;
		vecEncData.resize(tReqSize);
		DecodeBase64(strEncString, &vecEncData[0]);

		tReqSize = DecodeData(stCipherKey, &vecEncData[0], vecEncData.size(), NULL);

		std::string strRestored;
		strRestored.resize(tReqSize);
		DecodeData(stCipherKey, &vecEncData[0], vecEncData.size(), (LPBYTE)strRestored.c_str());


		// truncate padding bytes
		size_t i;
		for(i=tReqSize-1; i>0; i--)
		{
			if( strRestored.at(i) )
				break;
		}
		strRestored.resize(i+1);
		return strRestored;
	}

	//////////////////////////////////////////////////////////////////////////
	struct __internal_ST_DEFAULT_KEY_CONTAINER
	{
		ST_SYM_CIPHER_KEY stKey;
		__internal_ST_DEFAULT_KEY_CONTAINER(void)
		{
			GenerateCipherKey(SYM_CIPHER_TYPE_AES256, SYM_CIPHER_MODE_OFB, "profrog_default", stKey);
		}
	};
	const __internal_ST_DEFAULT_KEY_CONTAINER g_stDefaultKeyContainer;

	//////////////////////////////////////////////////////////////////////////
	std::tstring DefaultEncodeString(std::string strUTF8)
	{
		return EncodeString(g_stDefaultKeyContainer.stKey, strUTF8);
	}

	//////////////////////////////////////////////////////////////////////////
	std::string DefaultDecodeString(std::tstring strEncrypted)
	{
		return DecodeString(g_stDefaultKeyContainer.stKey, strEncrypted);
	}
}
