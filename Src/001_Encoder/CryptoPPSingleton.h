#pragma once

#include "../../Inc/cryptopp700/randpool.h"
#include "../../Inc/cryptopp700/rsa.h"
#include "../../Inc/cryptopp700/hex.h"
#include "../../Inc/cryptopp700/files.h"
#include "../../Inc/cryptopp700/filters.h"
#include "../../Inc/cryptopp700/aes.h"
#include "../../Inc/cryptopp700/modes.h"
#include "../../Inc/cryptopp700/default.h"
#include "../../Inc/cryptopp700/cryptlib.h"
#include "../../Inc/cryptopp700/elgamal.h"
#include "../../Inc/cryptopp700/osrng.h"

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
