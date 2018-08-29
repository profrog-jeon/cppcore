#pragma once

#include "cryptopp700/randpool.h"
#include "cryptopp700/rsa.h"
#include "cryptopp700/hex.h"
#include "cryptopp700/files.h"
#include "cryptopp700/filters.h"
#include "cryptopp700/aes.h"
#include "cryptopp700/modes.h"
#include "cryptopp700/default.h"
#include "cryptopp700/cryptlib.h"
#include "cryptopp700/elgamal.h"
#include "cryptopp700/osrng.h"

namespace core
{
	class CCryptoPPSingleton
	{
		CCryptoPPSingleton(void);
		~CCryptoPPSingleton(void)	{}

	public:
		CryptoPP::RandomPool RandPool;

		static CCryptoPPSingleton* GetInstance(void)
		{
			static CCryptoPPSingleton instance;
			return &instance;
		}
	};

	inline CCryptoPPSingleton* CryptoPP(void)
	{
		return CCryptoPPSingleton::GetInstance();
	}
}
