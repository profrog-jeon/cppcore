#pragma once

#include "AsymCipher.h"

namespace core
{
	class CAsymRSA : public IAsymAlgorithm
	{
		void* m_pHandle;

	public:
		CAsymRSA();
		~CAsymRSA();

		ECODE GenerateKey(DWORD dwKeySize, const char* pszSeed, std::vector<BYTE> vecPrivKey, std::vector<BYTE> vecPubKey);

		ECODE Init(E_CIPHER_METHOD nMethod, const std::vector<BYTE>& vecKey);
		size_t GetPlainTextLength(void);
		size_t GetCipherTextLength(void);
	};
}
