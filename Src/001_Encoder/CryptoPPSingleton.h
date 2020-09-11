#pragma once

#include "../../Inc/randpool.h"
#include "../../Inc/rsa.h"
#include "../../Inc/hex.h"
#include "../../Inc/files.h"
#include "../../Inc/filters.h"
#include "../../Inc/aes.h"
#include "../../Inc/modes.h"
#include "../../Inc/default.h"
#include "../../Inc/cryptlib.h"
#include "../../Inc/elgamal.h"
#include "../../Inc/osrng.h"

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
