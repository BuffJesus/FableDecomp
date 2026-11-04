#include "fable_game.h"
#include <new>
#include <stdio.h>
#include <string.h>

static unsigned long storage[(sizeof(CGameEventPackageSet) + 3) / 4];
static unsigned char expected[sizeof(CGameEventPackageSet)];

int main()
{
    memset(storage, 0xa5, sizeof(storage));
    memset(expected, 0xa5, sizeof(expected));
    CGameEventPackageSet* set = new (storage) CGameEventPackageSet;
    if (reinterpret_cast<void*>(set) != storage) return 1;
    memset(expected, 0, 4);
    int cases = 1;
    for (unsigned int p = 0; p < 50; ++p)
    {
        unsigned char* package = expected + 4 + p * 0x648;
        memset(package + 4, 0, 4);
        ++cases;
        for (unsigned int e = 0; e < 40; ++e)
        {
            unsigned char* event = package + 8 + e * 0x28;
            event[4] = 0xff;
            event[0x26] = 0;
            ++cases;
        }
    }
    if (memcmp(storage, expected, sizeof(expected))) return 2;
    printf("PACKAGE_SET_CTOR_PASS cases=%d\n", cases);
    return 0;
}
