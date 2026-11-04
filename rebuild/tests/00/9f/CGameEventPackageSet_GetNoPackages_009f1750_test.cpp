#include "fable_game.h"
#include <stdio.h>

static unsigned long storage[(sizeof(CGameEventPackageSet) + 3) / 4];

int main()
{
    CGameEventPackageSet* set = reinterpret_cast<CGameEventPackageSet*>(storage);
    const unsigned long counts[] = {0, 1, 2, 49, 50, 0x7fffffffUL, 0x80000000UL, 0xffffffffUL};
    for (unsigned int i = 0; i < sizeof(counts) / sizeof(counts[0]); ++i)
    {
        storage[0] = counts[i];
        if (set->GetNoPackages() != counts[i]) return 1;
    }
    printf("PACKAGE_SET_GET_NO_PACKAGES_PASS cases=8\n");
    return 0;
}
