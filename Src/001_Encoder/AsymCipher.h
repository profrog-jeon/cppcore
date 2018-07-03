#pragma once

#include <vector>
#include "Type.h"

namespace core
{
	struct ST_ASYM_CIPHER_INFO
	{
		E_ASYM_CIPHER_TYPE nType;
		DWORD dwPlainBlockSize;
		DWORD dwCipherBlockSize;
	};


	struct IAsymCipher
	{
		virtual ~IAsymCipher(void) {}
		virtual ECODE Init(std::vector<BYTE> vecKey, ST_ASYM_CIPHER_INFO* pInfo = NULL) = 0;
		virtual ECODE Update(LPCBYTE pSrc, size_t tBlockCount, LPBYTE pDest) = 0;
		virtual ECODE Final(void) = 0;
	};

	struct IAsymAlgorithm
	{
		virtual ~IAsymAlgorithm(void) {}

		virtual ECODE GenerateKey(DWORD dwKeySize, const char* pszSeed, std::vector<BYTE> vecPrivKey, std::vector<BYTE> vecPubKey) = 0;
		virtual size_t GetPlainTextLength(void) = 0;
		virtual size_t GetCipherTextLength(void) = 0;
	};

	class CAsymEncoder : public IAsymCipher
	{
		IAsymAlgorithm* m_pAlgorithm;

	public:
		CAsymEncoder(IAsymAlgorithm* pAlgorithm);
		~CAsymEncoder();

		ECODE Init(std::vector<BYTE> vecPubKey, ST_ASYM_CIPHER_INFO* pInfo = NULL);
		ECODE Update(LPCBYTE pSrc, size_t tBlockCount, LPBYTE pDest);
		ECODE Final(void);
	};

	class CAsymDecoder : public IAsymCipher
	{
		IAsymAlgorithm* m_pAlgorithm;

	public:
		CAsymDecoder(IAsymAlgorithm* pAlgorithm);
		~CAsymDecoder();

		ECODE Init(std::vector<BYTE> vecPrivKey, ST_ASYM_CIPHER_INFO* pInfo = NULL);
		ECODE Update(LPCBYTE pSrc, size_t tBlockCount, LPBYTE pDest);
		ECODE Final(void);
	};
}
