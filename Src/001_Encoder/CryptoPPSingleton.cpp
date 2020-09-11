#include "stdafx.h"
#include "CryptoPPSingleton.h"

#include <time.h>

namespace core
{
	using namespace CryptoPP;

	CCryptoPPSingleton::CCryptoPPSingleton()
	{
		std::string seed = IntToString(time(NULLPTR));
		seed.resize(16, ' ');
		RandPool.IncorporateEntropy((byte *)seed.c_str(), seed.length());

		// Fetch the Symmetric	Cipher interface, not the RandomNumberGenerator interface, to key the underlying cipher
		CryptoPP::OFB_Mode<CryptoPP::AES>::Encryption GlobalRNG;
		OFB_Mode<AES>::Encryption& aesg = dynamic_cast<OFB_Mode<AES>::Encryption&>(dynamic_cast<RandomNumberGenerator&>(GlobalRNG));
		aesg.SetKeyWithIV((byte *)seed.data(), 16, (byte *)seed.data());
	}
}
