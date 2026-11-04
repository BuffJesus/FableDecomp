#include "fable_game.h"
#include <stdio.h>

static unsigned long storage[(sizeof(CGameEventPackageSet) + 3) / 4];

int main()
{
    CGameEventPackageSet* set = reinterpret_cast<CGameEventPackageSet*>(storage);
    const unsigned long counts[] = {0, 1, 2, 49, 50, 0x7fffffffUL, 0x80000000UL, 0xffffffffUL};
    const unsigned long indices[] = {0, 1, 17, 49};
    int cases = 0;
    for (unsigned int i = 0; i < sizeof(counts) / sizeof(counts[0]); ++i)
    {
        storage[0] = counts[i];
        if (set->GetNoPackages() != counts[i]) return 1;
        ++cases;
        for (unsigned int j = 0; j < sizeof(indices) / sizeof(indices[0]); ++j)
        {
            const CGameEventPackage* expected = reinterpret_cast<const CGameEventPackage*>(
                reinterpret_cast<const unsigned char*>(set) + 4 + indices[j] * 0x648);
            if (set->GetPackage(indices[j]) != expected) return 2;
            ++cases;
        }
    }
    printf("PACKAGE_SET_GETTERS_PASS cases=%d\n", cases);
    return 0;
}
