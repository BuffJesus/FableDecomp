#include "fable_game.h"
#include <stdio.h>

static unsigned long storage[(sizeof(CGameEventPackageSet) + 3) / 4];

int main()
{
    const CGameEventPackageSet* set = reinterpret_cast<const CGameEventPackageSet*>(storage);
    const unsigned long indices[] = {0, 1, 17, 49};
    for (unsigned int i = 0; i < sizeof(indices) / sizeof(indices[0]); ++i)
    {
        const CGameEventPackage* expected = reinterpret_cast<const CGameEventPackage*>(
            reinterpret_cast<const unsigned char*>(set) + 4 + indices[i] * 0x648);
        if (set->GetPackage(indices[i]) != expected) return 1;
    }
    printf("PACKAGE_SET_GET_PACKAGE_PASS cases=4\n");
    return 0;
}
