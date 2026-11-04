#include "fable_game.h"
#include <stdio.h>
#include <string.h>

static unsigned long storage[(sizeof(CGameEventPackageSet) + 3) / 4];
static unsigned char expected[sizeof(CGameEventPackageSet)];
static unsigned char packageBytes[sizeof(CGameEventPackage)];

int main()
{
    CGameEventPackageSet* set = reinterpret_cast<CGameEventPackageSet*>(storage);
    const unsigned long indices[] = {0, 1, 17, 49};
    int cases = 0;
    for (unsigned int i = 0; i < sizeof(indices) / sizeof(indices[0]); ++i)
    for (unsigned int pattern = 0; pattern < 4; ++pattern)
    {
        memset(storage, 0xa5, sizeof(storage));
        memset(expected, 0xa5, sizeof(expected));
        for (unsigned int j = 0; j < sizeof(packageBytes); ++j)
            packageBytes[j] = static_cast<unsigned char>(j * 37 + pattern * 53);
        storage[0] = indices[i];
        memcpy(expected, &indices[i], sizeof(unsigned long));
        set->AddPackage(reinterpret_cast<const CGameEventPackage*>(packageBytes));
        memcpy(expected + 4 + indices[i] * 0x648, packageBytes, sizeof(packageBytes));
        unsigned long next = indices[i] + 1;
        memcpy(expected, &next, sizeof(next));
        if (memcmp(storage, expected, sizeof(expected))) return 1;
        ++cases;
        set->Clear();
        memset(expected, 0, sizeof(unsigned long));
        if (memcmp(storage, expected, sizeof(expected))) return 2;
        ++cases;
    }
    printf("PACKAGE_SET_MUTATIONS_PASS cases=%d\n", cases);
    return 0;
}
