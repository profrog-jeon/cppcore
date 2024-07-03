#include "stdafx.h"
#include "Random.h"
#include "Environment.h"

namespace core
{
    struct __INTERNAL_ST_RANDOM_INITIALIZER
    {
        int nRandomSeed;
        __INTERNAL_ST_RANDOM_INITIALIZER(void)
            : nRandomSeed(0)
        {
            nRandomSeed = GetTickCount() ^ GetCurrentProcessId();
            ::srand(nRandomSeed);
        }
    };

    static __INTERNAL_ST_RANDOM_INITIALIZER g_RandomInitializer;

    int Rand(void)
    {
        return ::rand();
    }
}
