#include "stdafx.h"
#include "Hash.h"
#include "md5.h"
#include "SHA1.h"
#include "sha256.h"
#include "ssdeep/fuzzy.cpp"
#include "has160.h"
#include "sha512.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	struct __internal_ST_HASH_INFO
	{
		DWORD dwHashType;
		union
		{
			MD5_CTX* pMd5;
			CSHA1* pSha1;
			_SHA256_CTX* pSha256;
			_SHA512_CTX* pSha512;
			KISA_HAS160* pHas160;
			fuzzy_state* pSsdeep;
		}	Context;
	};

	//////////////////////////////////////////////////////////////////////////
	HANDLE InitHash(E_HASH_TYPE dwHashType)
	{
		__internal_ST_HASH_INFO stHashInfo = { 0, };
		stHashInfo.dwHashType = dwHashType;

		switch (dwHashType)
		{
		case HASH_TYPE_MD5:
			stHashInfo.Context.pMd5 = new MD5_CTX;
			MD5Init(stHashInfo.Context.pMd5);
			break;

		case HASH_TYPE_SHA1:
			stHashInfo.Context.pSha1 = new CSHA1;
			stHashInfo.Context.pSha1->Reset();
			break;

		case HASH_TYPE_SHA256:
			stHashInfo.Context.pSha256 = new _SHA256_CTX;
			E_SHA256_Init(stHashInfo.Context.pSha256);
			break;
		case HASH_TYPE_SHA512:
			stHashInfo.Context.pSha512 = new _SHA512_CTX;
			E_SHA512_Init(stHashInfo.Context.pSha512);
			break;
		case HASH_TYPE_HAS160:
			stHashInfo.Context.pHas160 = new KISA_HAS160();
			KISA_HAS160_init(stHashInfo.Context.pHas160);
			break;

		case HASH_TYPE_SSDEEP:
			stHashInfo.Context.pSsdeep = fuzzy_new();
			break;

		default:
			return NULL;
		}

		return new __internal_ST_HASH_INFO(stHashInfo);
	}

	//////////////////////////////////////////////////////////////////////////
	void UpdateHash(HANDLE hHash, LPCBYTE pData, size_t tSize)
	{
		__internal_ST_HASH_INFO* pHashInfo = (__internal_ST_HASH_INFO*)hHash;

		switch (pHashInfo->dwHashType)
		{
		case HASH_TYPE_MD5:
			MD5Update(pHashInfo->Context.pMd5, (LPBYTE)pData, (DWORD)tSize);
			break;

		case HASH_TYPE_SHA1:
			pHashInfo->Context.pSha1->Input(pData, (DWORD)tSize);
			break;

		case HASH_TYPE_SHA256:
			E_SHA256_Update(pHashInfo->Context.pSha256, pData, tSize);
			break;

		case HASH_TYPE_SHA512:
			E_SHA512_Update(pHashInfo->Context.pSha512, pData, tSize);
			break;
		case HASH_TYPE_HAS160:
			KISA_HAS160_update(pHashInfo->Context.pHas160, pData, (DWORD)tSize);
			break;

		case HASH_TYPE_SSDEEP:
			fuzzy_update(pHashInfo->Context.pSsdeep, pData, tSize);
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void FinalHash(HANDLE hHash, std::vector<BYTE>& outBuff)
	{
		__internal_ST_HASH_INFO* pHashInfo = (__internal_ST_HASH_INFO*)hHash;

		switch (pHashInfo->dwHashType)
		{
		case HASH_TYPE_MD5:
		{
			MD5Final(pHashInfo->Context.pMd5);
			outBuff.resize(16);
			memcpy(&outBuff[0], pHashInfo->Context.pMd5->digest, 16);
			delete pHashInfo->Context.pMd5;
		}
		break;

		case HASH_TYPE_SHA1:
		{
			unsigned int dwSHA1[5] = { 0, };
			pHashInfo->Context.pSha1->Result(dwSHA1);

			outBuff.resize(20);
			int i;
			for (i = 0; i < 5; i++)
			{
				outBuff[i * 4 + 0] = (dwSHA1[i] >> 24) & 0xFF;
				outBuff[i * 4 + 1] = (dwSHA1[i] >> 16) & 0xFF;
				outBuff[i * 4 + 2] = (dwSHA1[i] >> 8) & 0xFF;
				outBuff[i * 4 + 3] = (dwSHA1[i] >> 0) & 0xFF;
			}
			delete pHashInfo->Context.pSha1;
		}
		break;

		case HASH_TYPE_SHA256:
		{
			unsigned char btSHA256[SHA256_DIGEST_LENGTH] = { 0, };
			E_SHA256_Final(btSHA256, pHashInfo->Context.pSha256);
			outBuff.resize(SHA256_DIGEST_LENGTH);
			memcpy(&outBuff[0], btSHA256, SHA256_DIGEST_LENGTH);
			delete pHashInfo->Context.pSha256;
		}
		break;
		case HASH_TYPE_SHA512:
		{
			unsigned char btSHA512[SHA512_DIGEST_LENGTH] = { 0, };
			E_SHA512_Final(btSHA512, pHashInfo->Context.pSha512);
			outBuff.resize(SHA512_DIGEST_LENGTH);
			memcpy(&outBuff[0], btSHA512, SHA512_DIGEST_LENGTH);
			delete pHashInfo->Context.pSha512;
		}
		break;
		case HASH_TYPE_HAS160:
		{
			unsigned char btHAS160[20] = { 0, };
			KISA_HAS160_final(pHashInfo->Context.pHas160, btHAS160);
			outBuff.resize(20);
			memcpy(&outBuff[0], btHAS160, 20);
			delete pHashInfo->Context.pHas160;
		}
		break;

		case HASH_TYPE_SSDEEP:
		{
			char szSSDEEP[FUZZY_MAX_RESULT + 1] = { 0, };
			fuzzy_digest(pHashInfo->Context.pSsdeep, szSSDEEP, 0);
			fuzzy_free(pHashInfo->Context.pSsdeep);

			size_t tSSDEEPLen = strlen(szSSDEEP);
			outBuff.resize(tSSDEEPLen);
			memcpy(&outBuff[0], szSSDEEP, tSSDEEPLen);
		}
		break;
		}

		delete pHashInfo;
	}

	//////////////////////////////////////////////////////////////////////////
	std::string FinalHash(HANDLE hHash)
	{
		__internal_ST_HASH_INFO* pHashInfo = (__internal_ST_HASH_INFO*)hHash;
		std::string strOutput;

		switch (pHashInfo->dwHashType)
		{
		case HASH_TYPE_MD5:
		{
			MD5Final(pHashInfo->Context.pMd5);
			strOutput = StringFromHexA(pHashInfo->Context.pMd5->digest, 16);
			delete pHashInfo->Context.pMd5;
		}
		break;

		case HASH_TYPE_SHA1:
		{
			unsigned int dwSHA1[5] = { 0, };
			if (pHashInfo->Context.pSha1->Result(dwSHA1))
				strOutput = StringFromHexA(dwSHA1, 5);
			delete pHashInfo->Context.pSha1;
		}
		break;

		case HASH_TYPE_SHA256:
		{
			unsigned char btSHA256[SHA256_DIGEST_LENGTH] = { 0, };
			E_SHA256_Final(btSHA256, pHashInfo->Context.pSha256);
			strOutput = StringFromHexA(btSHA256, SHA256_DIGEST_LENGTH);
			delete pHashInfo->Context.pSha256;
		}
		break;
		case HASH_TYPE_SHA512:
		{
			unsigned char btSHA512[SHA512_DIGEST_LENGTH] = { 0, };
			E_SHA512_Final(btSHA512, pHashInfo->Context.pSha512);
			strOutput = StringFromHexA(btSHA512, SHA512_DIGEST_LENGTH);
			delete pHashInfo->Context.pSha512;
		}
		break;
		case HASH_TYPE_HAS160:
		{
			unsigned char btHAS160[20] = { 0, };
			KISA_HAS160_final(pHashInfo->Context.pHas160, btHAS160);
			strOutput = StringFromHexA(btHAS160, sizeof(btHAS160));
			delete pHashInfo->Context.pHas160;
		}
		break;

		case HASH_TYPE_SSDEEP:
		{
			char szSSDEEP[FUZZY_MAX_RESULT + 1] = { 0, };
			fuzzy_digest(pHashInfo->Context.pSsdeep, szSSDEEP, 0);
			fuzzy_free(pHashInfo->Context.pSsdeep);
			strOutput = szSSDEEP;
		}
		break;
		}

		delete pHashInfo;
		return strOutput;
	}

}

